#include "objdef.h"

#define DEBUG_OVER_SERIAL

const UINT32 VENDOR_ID = 0x04210909;
UINT32 PRODUCT_CODE = 0x00030004;
UINT32 REVISION_NUMBER = 0x00010000;
UINT32 SERIAL_NUMBER = 0x000010000;
UINT32 DEVICE_PROFILE_TYPE = ((0<<24)|(3<<16)|(401));
char DEVICE_NAME[] = "DIO044 4Ch. Dig. IO ";
UINT8 DEVICE_NAME_LEN = 0x14;
char DEVICE_HW_VERSION[] = "1.00";
UINT8 DEVICE_HW_VERSION_LEN = 0x4;
char DEVICE_SW_VERSION[] = "1.00";
UINT8 DEVICE_SW_VERSION_LEN = 0x4;
//extern TSYNCMANPAR MBXMEM sSyncManOutPar;
//extern TSYNCMANPAR MBXMEM sSyncManInPar;

/*-----------------------------------------------------------------------------------------
------
------    Generic EtherCAT device object
------
-----------------------------------------------------------------------------------------*/

/******************************************************************************
** Object 0x1000
******************************************************************************/
UINT32 u32Devicetype = (DEVICE_PROFILE_TYPE);
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1000 = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1000[] = "Device type";

/******************************************************************************
** Object 0x1001
******************************************************************************/
UINT16 u16ErrorRegister = 0x0;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1001 = {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1001[] = "Error register";

/******************************************************************************
** Object 0x1008
******************************************************************************/
CHAR *acDevicename = DEVICE_NAME;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1008 = {DEFTYPE_VISIBLESTRING, BYTE2BIT(DEVICE_NAME_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1008[] = "Device name";

/******************************************************************************
** Object 0x1009
******************************************************************************/
CHAR *acHardwareversion = DEVICE_HW_VERSION;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1009 = {DEFTYPE_VISIBLESTRING,BYTE2BIT(DEVICE_HW_VERSION_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1009[] = "Hardware version";

/******************************************************************************
** Object 0x100A
******************************************************************************/
CHAR *acSoftwareversion = DEVICE_SW_VERSION;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x100A = {DEFTYPE_VISIBLESTRING,BYTE2BIT(DEVICE_SW_VERSION_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x100A[] = "Software version";


/******************************************************************************
** Object 0x1018
******************************************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   u32VendorID;
   UINT32   u32Productcode;
   UINT32   u32Revision;
   UINT32   u32Serialnumber;
} OBJ_STRUCT_PACKED_END
TOBJ1018;

TOBJ1018 sIdentity = {4, (VENDOR_ID), (PRODUCT_CODE), (REVISION_NUMBER), (SERIAL_NUMBER)};
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1018[5] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: Vendor ID */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Product code */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Revision */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 004: Serial number */
OBJCONST UCHAR OBJMEM aName0x1018[] = "Identity\000Vendor ID\000Product code\000Revision\000Serial number\000\377";


/******************************************************************************
** Object 0x10F1    Error Settings
******************************************************************************/
TOBJ10F1 sErrorSettings
= {2, 0x00, MAX_SM_EVENT_MISSED}
;
//object declaration and initialization in objdef.h
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x10F1[3] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: Local Error Reaction */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 002: Sync Error Counter Limit*/
OBJCONST UCHAR OBJMEM aName0x10F1[] = "Error Settings\000Local Error Reaction\000Sync Error Counter Limit\000\377";

/******************************************************************************
** Object 0x1C00
******************************************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT8   aEntries[4];
} OBJ_STRUCT_PACKED_END
TOBJ1C00;

TOBJ1C00 sSyncmanagertype = {0x04, {0x01, 0x02, 0x03, 0x04}};
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C00[2] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ}};
OBJCONST UCHAR OBJMEM aName0x1C00[] = "Sync manager type";



/******************************************************************************
** Object 0x1C32
******************************************************************************/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C32[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 001: Sync mode */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Shift time */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}, /* SubIndex 004: Sync modes supported */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 005: Minimum cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 006: Calc and copy time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* Subindex 007 reserved for future use*/
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 008: Get Cycle Time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 009: Delay time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* SubIndex 010: Sync0 time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 011: SM event missed counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 012: Cycle exceeded counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 013: Shift too short counter */
   {0x0000, 0, 0}, /* Subindex 014 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 015 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 016 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 017 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 018 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 019 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 020 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 021 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 022 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 023 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 024 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 025 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 026 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 027 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 028 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 029 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 030 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 031 doesn't exist */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ}}; /* SubIndex 032: Sync error */
/*ECATCHANGE_START(V5.01) COE1*/
OBJCONST UCHAR OBJMEM aName0x1C32[] = "SM output parameter\000Sync mode\000Cycle time\000Shift time\000Sync modes supported\000Minimum cycle time\000Calc and copy time\000Reserved\000Get Cycle Time\000Delay time\000Sync0 time\000SM event missed counter\000Cycle exceeded counter\000Shift too short counter\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000Sync error\000\377";
/*ECATCHANGE_START(V5.01) COE1*/
/******************************************************************************
** Object 0x1C33
******************************************************************************/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C33[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 001: Sync mode */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Shift time */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}, /* SubIndex 004: Sync modes supported */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 005: Minimum cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 006: Calc and copy time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* Subindex 007 reserved for future use*/
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 008: Get Cycle Time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 009: Delay time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* SubIndex 010: Sync0 time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 011: SM event missed counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 012: Cycle exceeded counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 013: Shift too short counter */
   {0x0000, 0, 0}, /* Subindex 014 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 015 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 016 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 017 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 018 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 019 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 020 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 021 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 022 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 023 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 024 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 025 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 026 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 027 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 028 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 029 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 030 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 031 doesn't exist */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ}}; /* SubIndex 032: Sync error */
/*ECATCHANGE_START(V5.01) COE1*/
OBJCONST UCHAR OBJMEM aName0x1C33[] = "SM input parameter\000Sync mode\000Cycle time\000Shift time\000Sync modes supported\000Minimum cycle time\000Calc and copy time\000Reserved\000Get Cycle Time\000Delay time\000Sync0 time\000Cycle exceeded counter\000SM event missed counter\000Shift too short counter\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000Sync error\000\377";
/*ECATCHANGE_START(V5.01) COE1*/
/******************************************************************************
** Object Dictionary
******************************************************************************/
//TOBJECT    OBJMEM * ObjDicList = NULL;

TOBJECT    OBJMEM GenObjDic[] = {
    /* Object 0x1000 */
   {NULL,NULL,  0x1000, {DEFTYPE_UNSIGNED32, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1000, aName0x1000, &u32Devicetype, NULL, NULL, 0x0000 },
   /* Object 0x1001 */
   {NULL,NULL,  0x1001, {DEFTYPE_UNSIGNED8, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1001, aName0x1001, &u16ErrorRegister, NULL, NULL, 0x0000 },
/* Object 0x1008 */
   {NULL,NULL,  0x1008, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1008, aName0x1008, acDevicename, NULL, NULL, 0x0000 },
   /* Object 0x1009 */
   {NULL,NULL,  0x1009, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1009, aName0x1009, acHardwareversion, NULL, NULL, 0x0000 },
   /* Object 0x100A */
   {NULL,NULL,  0x100A, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x100A, aName0x100A, acSoftwareversion, NULL, NULL, 0x0000 },
   /* Object 0x1018 */
   {NULL,NULL,  0x1018, {DEFTYPE_IDENTITY, 4 | (OBJCODE_REC << 8)}, asEntryDesc0x1018, aName0x1018, &sIdentity, NULL, NULL, 0x0000 },
    /* Object 0x10F1 */
   {NULL,NULL,  0x10F1, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x10F1, aName0x10F1, &sErrorSettings, NULL, NULL, 0x0000 },
   /* Object 0x1C00 */
   {NULL,NULL, 0x1C00, {DEFTYPE_UNSIGNED8, 4 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C00, aName0x1C00, &sSyncmanagertype, NULL, NULL, 0x0000 },
   /* Object 0x1C32 */
   {NULL,NULL, 0x1C32, {DEFTYPE_SMPAR, 32 | (OBJCODE_REC << 8)}, asEntryDesc0x1C32, aName0x1C32, &ethercat.sSyncManOutPar, NULL, NULL, 0x0000 },
   /* Object 0x1C33 */
   {NULL,NULL, 0x1C33, {DEFTYPE_SMPAR, 32 | (OBJCODE_REC << 8)}, asEntryDesc0x1C33, aName0x1C33, &ethercat.sSyncManInPar, NULL, NULL, 0x0000 },
  /*end of entries*/
   {NULL,NULL,    0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};




/*PDO assign entry description*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asPDOAssignEntryDesc[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ|ACCESS_WRITE},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ|ACCESS_WRITE}};



/******************************************************************************
** Object 0x0800: ENUM
******************************************************************************/
CHAR sEnum0800_Value00[] = "\000\000\000\000Signed presentation"; /* Value = 0x00, Text = Signed presentation */
CHAR sEnum0800_Value01[] = "\001\000\000\000Unsigned presentation"; /* Value = 0x01, Text = Unsigned presentation */
CHAR *apEnum0800[2] = { sEnum0800_Value00, sEnum0800_Value01};

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x0800[] =
   {{DEFTYPE_UNSIGNED8, 8, ACCESS_READ | OBJACCESS_NOPDOMAPPING},
    {DEFTYPE_OCTETSTRING, 8*SIZEOF(sEnum0800_Value00), ACCESS_READ | OBJACCESS_NOPDOMAPPING},
   {DEFTYPE_OCTETSTRING, 8*SIZEOF(sEnum0800_Value01), ACCESS_READ | OBJACCESS_NOPDOMAPPING}};


/******************************************************************************
*                    Object 0x1600: digital output RxPDO
******************************************************************************/


OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1600[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: SubIndex 001 */
}; /* SubIndex 009: SubIndex 009 */
OBJCONST UCHAR OBJMEM aName0x1600[] = "DO RxPDO-Map\000\377";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   aEntries[1];
} OBJ_STRUCT_PACKED_END
TOBJ1600;

TOBJ1600 sDORxPDOMap
 = {1, {0x62000110}}
;


/******************************************************************************
*                    Object 0x1A00: digital input TxPDO
******************************************************************************/

OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1A00[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: SubIndex 001 */
}; 
OBJCONST UCHAR OBJMEM aName0x1A00[] = "DI TxPDO-Map\000\377";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   aEntries[1];
} OBJ_STRUCT_PACKED_END
TOBJ1A00;

TOBJ1A00 sDITxPDOMap
 = {1, {0x60000110}}
;


/******************************************************************************
*                    Object 0x1C12: RxPDO assignment
******************************************************************************/

OBJCONST UCHAR OBJMEM aName0x1C12[] = "RxPDO assign";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT16   aEntries[1];
} OBJ_STRUCT_PACKED_END
TOBJ1C12;

TOBJ1C12 sRxPDOassign
= {0x01, {0x1600}}
//= {0x00}
;


/******************************************************************************
*                    Object 0x1C13: TxPDO assignment
******************************************************************************/

OBJCONST UCHAR OBJMEM aName0x1C13[] = "TxPDO assign";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT16   aEntries[1];
} OBJ_STRUCT_PACKED_END
TOBJ1C13;


TOBJ1C13 sTxPDOassign
= {0x01, {0x1A00}}
//= {0x00}
;



/******************************************************************************
*                    Object 0x6000: digital input object
******************************************************************************/

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[2] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x16, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 001: AO01 */
   }; 
OBJCONST UCHAR OBJMEM aName0x6000[] = "DI Inputs\000DI01~04\000\377";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT16   i16DigitalInput[1];
} OBJ_STRUCT_PACKED_END
TOBJ6000;

TOBJ6000 sDIInputs
= {1,{0x00}}
;

/******************************************************************************
*                    Object 0x6200: digital output object
******************************************************************************/

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6200[2] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x16, ACCESS_READ | ACCESS_WRITE | OBJACCESS_TXPDOMAPPING}, /* SubIndex 001: AO01 */
   }; 
OBJCONST UCHAR OBJMEM aName0x6200[] = "DO Outputs\000DO01~04\000\377";
 //

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT16   i16DigitalOutput[1];
} OBJ_STRUCT_PACKED_END
TOBJ6200;

TOBJ6200 sDOOutputs
= {1,{0x00}}
;


/******************************************************************************
*                    Object 0x6003: Filter Constant
******************************************************************************/

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6003[9] = {
   {DEFTYPE_UNSIGNED8, 0x4, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x32, ACCESS_READ | ACCESS_WRITE}, /* SubIndex 001: Filter constant 01 */
   {DEFTYPE_UNSIGNED32, 0x32, ACCESS_READ | ACCESS_WRITE}, /* SubIndex 002: Filter constant 01 */
   {DEFTYPE_UNSIGNED32, 0x32, ACCESS_READ | ACCESS_WRITE}, /* SubIndex 003: Filter constant 01 */
   {DEFTYPE_UNSIGNED32, 0x32, ACCESS_READ | ACCESS_WRITE}, /* SubIndex 004: Filter constant 01 */
   }; 
OBJCONST UCHAR OBJMEM aName0x6003[] = "Filter constant\000Filter constant01\000Filter constant02\000Filter constant03\000Filter constant04\000\377";
 

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   FilterConstant[4];
} OBJ_STRUCT_PACKED_END
TOBJ6003;

TOBJ6003 sFilterConstant
= {8,0x00,0x00,0x00,0x00}
;



/* ECATCHANGE_START(V5.01) EL9800 2*/
UINT8 ReadObject0x1802( UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
/* ECATCHANGE_END(V5.01) EL9800 2*/

/*if _PIC18 is enabled the object dictionary is fixed defined in coeappl.c*/
TOBJECT    OBJMEM ApplicationObjDic[] = {
   /* Enum 0x0800 */
   {NULL,NULL, 0x0800, {DEFTYPE_ENUM, 0x02 | (OBJCODE_REC << 8)}, asEntryDesc0x0800, 0, apEnum0800 },
   /* Object 0x1600 */
   {NULL,NULL,  0x1600, {DEFTYPE_PDOMAPPING, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x1600, aName0x1600, &sDORxPDOMap, NULL, NULL, 0x0000 },
   /* Object 0x1A00 */
   {NULL,NULL,   0x1A00, {DEFTYPE_PDOMAPPING, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x1A00, aName0x1A00, &sDITxPDOMap, NULL, NULL, 0x0000 },
    /* Object 0x1C12 */
   {NULL,NULL,   0x1C12, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
   /* Object 0x1C13 */
   {NULL,NULL,   0x1C13, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
   /* Object 0x6000 */
   {NULL,NULL,   0x6000, {DEFTYPE_RECORD, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x6000, aName0x6000, &sDIInputs, NULL, NULL, 0x0000 },
   /* Object 0x6200 */
   {NULL,NULL,   0x6200, {DEFTYPE_RECORD, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x6200, aName0x6200, &sDOOutputs, NULL, NULL, 0x0000 },
   /* Object 0x6003 */
   {NULL,NULL,   0x6003, {DEFTYPE_RECORD, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x6003, aName0x6003, &sFilterConstant, NULL, NULL, 0x0000 },
   {NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
    //
	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    Ethercat::APPL_AckErrorInd(UINT16 stateTrans)
{ 
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
           all general settings were checked to start the mailbox handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
             all general settings were checked to start the input handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case the application need to be complete 
            the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartInputHandler(UINT16 *pIntMask)
{
	//cx
	//NVIC_EnableIRQ(TIMER_16_0_IRQn);
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopInputHandler(void)
{
	//NVIC_DisableIRQ(TIMER_16_0_IRQn);
	//LPC_GPIO3->DATA |= (1<<2);
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::APPL_GenerateMapping(UINT16* pInputSize,UINT16* pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
                pPDOEntry = (UINT32 *)((UINT8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));    //goto PDO entry
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            //assigned PDO was not found in object dictionary. return invalid mapping//
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }
    OutputSize = (OutputSize + 7) >> 3;

    if(result == 0)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
					pPDOEntry = (UINT32 *)((UINT8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));    //goto PDO entry
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                //assigned PDO was not found in object dictionary. return invalid mapping
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
  #ifdef DEBUG_OVER_SERIAL
    Serial.println("Ethercat::APPL_GenerateMapping");
    Serial.print("InputSize=");
    Serial.println(InputSize);
    Serial.print("OutputSize=");
    Serial.println(OutputSize);
  #endif
    return result;

}

//========Output mapping================
//LPC_GPIO_TypeDef* tableDOPort1[4] = {LPC_GPIO1,LPC_GPIO1,LPC_GPIO2,LPC_GPIO2};
//const UINT8 tableDOBit1[4] = {0,10,2,1};
//LPC_GPIO_TypeDef* tableDOPort2[4] = {LPC_GPIO1,LPC_GPIO0,LPC_GPIO2,LPC_GPIO2};
//const UINT8 tableDOBit2[4] = {8,2,7,8};
//LPC_GPIO_TypeDef* tableDIInd[4] = {LPC_GPIO1,LPC_GPIO1,LPC_GPIO3,LPC_GPIO2};
//const UINT8 tableDIBitInd[4] = {6,7,3,6};
//========Input mapping================
//UINT8 inputState = 0;
//UINT8 inputPin = 0;
//UINT32 inputCnt[4] = {0,0,0,0};
/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data
\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::APPL_InputMapping(UINT16* pData)
{
  *pData = sDIInputs.i16DigitalInput[0];
/*    UINT16 j = 0;
	*pData = inputState;
	for(j=0;j<4;j++)
	{
		if (inputState&(1<<j))
			tableDIInd[j]->DATA &= ~(1<<tableDIBitInd[j]);
		else
			tableDIInd[j]->DATA |= (1<<tableDIBitInd[j]);
	}
*/
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
s*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::APPL_OutputMapping(UINT16* pData)
{
  
    UINT16 j = 0;
    UINT16 *pTmpData = (UINT16 *)pData;
	UINT16 temp;

   for (j = 0; j < sRxPDOassign.u16SubIndex0; j++)
   {
      switch (sRxPDOassign.aEntries[j])
      {
      case 0x1600:
         sDOOutputs.i16DigitalOutput[0] = *pTmpData++;
         break;
      /* TxPDO 3 */
      }
   }
   /*
   temp = sDOOutputs.i8DigitalOutput[0];
   	
	for(j=0;j<4;j++)
	{
		if (temp&(1<<j))
		{
			tableDOPort1[j]->DATA |= (1<<tableDOBit1[j]);
			tableDOPort2[j]->DATA &= ~(1<<tableDOBit2[j]);
		}
		else
		{
			tableDOPort1[j]->DATA &= ~(1<<tableDOBit1[j]);
			tableDOPort2[j]->DATA |= (1<<tableDOBit2[j]);
		}
	}
*/
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::APPL_Application(void)
{
}
/*
void hwRW(void)
{
	UINT32 temp1,temp2,i;
	UINT8 inputDiff;

	temp1 = LPC_GPIO1->DATA;
	temp2 = LPC_GPIO2->DATA;

	inputPin = (((temp1&(1<<4))>>4)<<0)|(((temp2&(1<<3))>>3)<<1)|(((temp1&(1<<2))>>2)<<2)|(((temp1&(1<<1))>>1)<<3);
	inputDiff = inputPin^inputState;
	for(i=0;i<4;i++)
	{
		if(inputDiff&(1<<i))
			inputCnt[i]++;
		else
			inputCnt[i] = 0;
		if(inputCnt[i]>sFilterConstant.FilterConstant[i])
		{
			inputState = (inputState&(~(1<<i)))|(inputPin&(1<<i));
			inputCnt[i] = 0;
		}
	}

}
*/
