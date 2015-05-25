/**
\defgroup el9800hw el9800hw.c: EL9800 hardware abstraction
\brief Hardware access implementation for EL9800 onboard PIC18/PIC24 connected via SPI to ESC\n
\brief Changes to version V5.0:
\brief V5.01 HW1: Invalid ESC access function was used
\brief Changes to version V4.40:
\brief V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.
\brief Changes to version V4.30:\n
\brief V4.40 : File renamed from spihw.c to el9800hw.c
\brief Changes to version V4.20:\n
\brief V4.30 ESM: if mailbox Syncmanger is disabled and bMbxRunning is true the SyncManger settings need to be revalidate
\brief V4.30 EL9800: EL9800_x hardware initialization is moved to el9800.c
\brief V4.30 SYNC: change synchronisation control function. Add usage of 0x1C32:12 [SM missed counter].
\brief Calculate bus cycle time (0x1C32:02 ; 0x1C33:02) CalcSMCycleTime()
\brief V4.30 PDO: rename PDO specific functions (COE_xxMapping -> PDO_xxMapping and COE_Application -> ECAT_Application)
\brief V4.30 ESC: change requested address in GetInterruptRegister() to prevent acknowledge events.
\brief (e.g. reading an SM config register acknowledge SM change event)
\brief GENERIC: renamed several variables to identify used SPI if multiple interfaces are available
\brief V4.20 MBX 1: Add Mailbox queue support
\brief V4.20 SPI 1: include SPI RxBuffer dummy read
\brief V4.20 DC 1: Add Sync0 Handling
\brief V4.20 PIC24: Add EL9800_4 (PIC24) required source code
\brief V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd\n
\brief Changes to version V4.02:\n
\brief V4.03 SPI 1: In ISR_GetInterruptRegister the NOP-command should be used.\n
\brief Changes to version V4.01:\n
\brief V4.02 SPI 1: In HW_OutputMapping the variable u16OldTimer shall not be set,
\brief              otherwise the watchdog might exceed too early.\n
\brief Changes to version V4.00:\n
\brief V4.01 SPI 1: DI and DO were changed (DI is now an input for the uC, DO is now an output for the uC)\n
\brief V4.01 SPI 2: The SPI has to operate with Late-Sample = FALSE on the Eva-Board\n
\brief Changes to version V3.20:\n
\brief V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to
\brief               the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 APPL 1: The watchdog checking should be done by a microcontroller
\brief                  timer because the watchdog trigger of the ESC will be reset too
\brief                  if only a part of the sync manager data is written\n
\brief V4.00 APPL 4: The EEPROM access through the ESC is added\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup el9800hw
\file    el9800hw.c
\brief Implementation.
*/

//---------------------------------------------------------------------------------------
/**
\addtogroup el9800hw
@{
*/
//---------------------------------------------------------------------------------------

/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"



#include "ethercat.h"
#include <SPI.h>

/*--------------------------------------------------------------------------------------
--------------------Ethercat pin definitions--------------------------------------------
SSEL = PH2
SIRQ = PE6(INT6)
SYNC = PE7(INT7)


----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/

#define    WAIT_SPI_IF                        //spi.end();//while( SPI1_IF );//while( !SPI1_IF );
#define    SELECT_SPI                      PORTH &= ~(1<<2);//digitalWrite(15, LOW);//{(LPC_GPIO3->DATA) &= ~(SPI_ACTIVE);}
#define    DESELECT_SPI                    PORTH |= (1<<2);//digitalWrite(15, HIGH);//{(LPC_GPIO3->DATA) |= (SPI_DEACTIVE);}
#define    INIT_SSPIF                        //{(SPI1_IF)=0;}
#define SPI1_CON0_VALUE                    	0x000000C7//0x027E
#define SPI1_CON0_VALUE_16BIT            	0x000000CF//0x047E
#define SPI1_CON1_VALUE                    	0x00000002//Write at end
#define SPI1_CPSR_VALUE                    	0x00000002//Write at end
#define SPI_DEACTIVE                    (1<<4)
#define SPI_ACTIVE                      (1<<4)


/*-----------------------------------------------------------------------------------------
------
------    Global Interrupt setting
------
-----------------------------------------------------------------------------------------*/

#define DISABLE_GLOBAL_INT           noInterrupts();
#define ENABLE_GLOBAL_INT            interrupts();
#define    DISABLE_AL_EVENT_INT        DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT            ENABLE_GLOBAL_INT


/*-----------------------------------------------------------------------------------------
------
------    ESC Interrupt
------
-----------------------------------------------------------------------------------------*/

#define	   INIT_ESC_INT		attachInterrupt(6,SIRQ_IRQHandler,FALLING );



/*-----------------------------------------------------------------------------------------
------
------    SYNC0 Interrupt
------
-----------------------------------------------------------------------------------------*/

#define	   INIT_SYNC0_INT		//{(LPC_GPIO1->IE)|=(1<<9);(LPC_GPIO1->IS)|=(1<<9);(LPC_GPIO1->IEV)&=~(1<<9);NVIC_EnableIRQ(EINT1_IRQn);}
#define    DISABLE_SYNC0_INT    detachInterrupt(7);//            {(LPC_GPIO1->IE)&=~(1<<9);}//disable interrupt source INT3
#define    ENABLE_SYNC0_INT     attachInterrupt(7,SYNC_IRQHandler,FALLING );//           {(LPC_GPIO1->IE)|=(1<<9);} //enable interrupt source INT3

/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/


#define INIT_ECAT_TIMER                


#define STOP_ECAT_TIMER 

#define START_ECAT_TIMER 



/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
//UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
//UINT16            nAlEventMask;        //current ALEventMask (content of register 0x204:0x205)
//TSYNCMAN        TmpSyncMan;

/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/
void SIRQ_IRQHandler();
void SYNC_IRQHandler();
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::GetInterruptRegister(void)
{
    UINT8            EscMbxReadEcatCtrl;
    DISABLE_AL_EVENT_INT;
    /* select the SPI */
    SELECT_SPI;

    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* write to SPI1_BUF register starts the SPI access*/
	EscALEvent.Byte[0] = SPI.transfer((UINT8) (0x0000 >> 5));
    //SPI1_BUF = (UINT8) (0x0000 >> 5);

    /* write to SPI1_BUF register starts the SPI access
       read the sm mailbox read ecatenable byte */
	EscALEvent.Byte[1] = SPI.transfer((UINT8) (((0x0000 & 0x1F) << 3) | ESC_RD));
 
    /* write to SPI1_BUF register starts the SPI access
       read the sm mailbox read ecatenable byte (last byte) */
	EscMbxReadEcatCtrl = SPI.transfer(0xFF);

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */

    DESELECT_SPI;

    ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::ISR_GetInterruptRegister(void)
{
    /* SPI should be deactivated to interrupt a possible transmission */
    DESELECT_SPI;

    /* select the SPI */
    SELECT_SPI;
    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* write to SPI1_BUF register starts the SPI access,
       read the sm mailbox read ecatenable byte
       (has to be synchronous to the al event flags) */
    EscALEvent.Byte[0] = SPI.transfer(0);
    /* get first byte of AL Event register */
    //EscALEvent.Byte[0] = SPI1_BUF;
    /* write to SPI1_BUF register starts the SPI access
       send NOP command */
    EscALEvent.Byte[1] = SPI.transfer(0);

    /* if the SPI transmission rate is higher than 15 MBaud, the Busy detection shall be
       done here */

    DESELECT_SPI;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::AddressingEsc( UINT16 Address, UINT8 Command )
{
    UBYTETOWORD tmp;
    VARVOLATILE UINT8 dummy;
    tmp.Word = ( Address << 3 ) | Command;
    /* select the SPI */
    SELECT_SPI;

    /* reset transmission flag */
    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* send the first address/command byte to the ESC */
    EscALEvent.Byte[0] = SPI.transfer(tmp.Byte[1]);    
    /* send the second address/command byte to the ESC */
    EscALEvent.Byte[1] = SPI.transfer(tmp.Byte[0]); 

    /* if the SPI transmission rate is higher than 15 MBaud, the Busy detection shall be
       done here */
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
        Shall be implemented if interrupts are supported else this function is equal to "AddressingEsc()"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::ISR_AddressingEsc( UINT16 Address, UINT8 Command )
{
    VARVOLATILE UINT8 dummy;
    UBYTETOWORD tmp;
    tmp.Word = ( Address << 3 ) | Command;

    /* select the SPI */
    SELECT_SPI;

    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* send the first address/command byte to the ESC */
    dummy = SPI.transfer(tmp.Byte[1]); 
    /* send the second address/command byte to the ESC */
    dummy = SPI.transfer(tmp.Byte[0]);  
    /* if the SPI transmission rate is higher than 15 MBaud, the Busy detection shall be
       done here */
}

/** @} */

/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 Ethercat::HW_Init(void)
{
    UINT16 intMask;

	SPI.begin();
      /* initialize the SPI registers for the ESC SPI */
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	SPI.setBitOrder(MSBFIRST); 
	SPI.setDataMode(SPI_MODE3);	//CPOL=CPHA=1
	//Initial SSEL pin 
	PORTH |= (1<<2);
	DDRH |= (1<<2);
	
    do
    {
        intMask = 0x93;
        HW_EscWriteWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);

    INIT_ESC_INT
    HW_ResetALEventMask(0);
    ENABLE_ESC_INT();

    INIT_SYNC0_INT
    ENABLE_SYNC0_INT;

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    /* enable all interrupts */
    ENABLE_GLOBAL_INT;

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_Release(void)
{
	SPI.end();
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_GetALEventRegister()"
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::HW_GetALEventRegister_Isr(void)
{
     ISR_GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (disabled interrupt shall be zero)

 \brief    This function makes an logical and with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_ResetALEventMask(UINT16 intMask)
{
    UINT16 mask;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask &= intMask;
    DISABLE_AL_EVENT_INT;
    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nAlEventMask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (enabled interrupt shall be one)

  \brief    This function makes an logical or with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_SetALEventMask(UINT16 intMask)
{
    UINT16 mask;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask |= intMask;
    DISABLE_AL_EVENT_INT;
    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nAlEventMask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param RunLed            desired EtherCAT Run led state
 \param ErrLed            desired EtherCAT Error led state

  \brief    This function updates the EtherCAT run and error led
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_SetLed(UINT8 RunLed,UINT8 ErrLed)
{
//      LED_ECATGREEN = RunLed;
//      LED_ECATRED   = ErrLed;
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i = Len;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, in that case the address has to be reinitialized,
           in that case the status flag will indicate an error because
           the reading operation was interrupted without setting the last
           sent byte to 0xFF */
        DISABLE_AL_EVENT_INT;
         AddressingEsc( Address, ESC_RD );

        /* when reading the last byte the DI pin shall be 1 */
		*pTmpData++ = SPI.transfer(0xFF); 
        /* enable the ESC interrupt to get the AL Event ISR the chance to interrupt,
           if the next byte is the last the transmission shall not be interrupted,
           otherwise a sync manager could unlock the buffer, because the last was
           read internally */
        ENABLE_AL_EVENT_INT;
        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */
        DESELECT_SPI;
        /* next address */
        Address++;
        /* reset transmission flag */
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

\brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_EscRead()"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    UINT8 data = 0;

   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     ISR_AddressingEsc( Address, ESC_RD );
    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        if ( i == 0 )
        {
            /* when reading the last byte the DI pin shall be 1 */
            data = 0xFF;
        }

        /* start transmission */
		*pTmpData++ = SPI.transfer(data); 
    }

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;

    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, so every byte will be written separate */
        DISABLE_AL_EVENT_INT;
        /* HBu 24.01.06: wrong parameter ESC_RD */
         AddressingEsc( Address, ESC_WR );
        /* start transmission */
        dummy = SPI.transfer(*pTmpData++); 
        ENABLE_AL_EVENT_INT;
        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */

        DESELECT_SPI;
        /* next address */
        Address++;

    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  The SPI PDI requires an extra ESC write access functions from interrupts service routines.
        The behaviour is equal to "HW_EscWrite()"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT16 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     ISR_AddressingEsc( Address, ESC_WR );
    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
        /* start transmission */
        dummy = SPI.transfer(*pTmpData);

        pTmpData++;
    }

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI;
}



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function disables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_DisableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    UINT16 smStatus = SM_SETTING_PDI_DISABLE;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));

    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is disabled*/
    do
    {
        HW_EscReadWord(smStatus, Offset);
    }while(!(smStatus & SM_SETTING_PDI_DISABLE));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function enables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EnableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    UINT16 smStatus = 0x0000;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));

    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is enabled*/
    do
    {
        HW_EscReadWord(smStatus,Offset);
    }while((smStatus & SM_SETTING_PDI_DISABLE));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param channel    requested Sync Manger channel information

 \return    Pointer to the SYNC Manager channel description

 \brief  This function is called to read the SYNC Manager channel descriptions of the
             process data SYNC Managers.
*////////////////////////////////////////////////////////////////////////////////////////

TSYNCMAN ESCMEM * Ethercat::HW_GetSyncMan(UINT8 channel)
{
    // get a temporary structure of the Sync Manager:
    HW_EscRead( (MEM_ADDR *) &TmpSyncMan, ESC_SYNCMAN_REG_OFFSET + (channel * SYNCMAN_REG_SIZE), SYNCMAN_REG_SIZE );

    return &TmpSyncMan;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the PDI interrupt from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////

//void __attribute__ ((__interrupt__, no_auto_psv)) EscIsr & SYNC0(void)
void SIRQ_IRQHandler()
{
    	ethercat.PDI_Isr();
}
void SYNC_IRQHandler()
{
    	ethercat.Sync0_Isr();
}




