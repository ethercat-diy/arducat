#include <ecatappl.h>
#include <ecatcoe.h>
#include <ecatslv.h>
#include <ecat_def.h>
#include <esc.h>
#include <ethercat.h>
#include <mailbox.h>
#include <objdef.h>
#include <sdoserv.h>
#include <SPI.h>



void setup() {
  // initialize digital pin 13 as an output.
  //pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial.print("setup");
  delay(1000);            //wait for 10ms
  ethercat.HW_Init();
  ethercat.MainInit();

}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);              // wait for a second
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);              // wait for a second
        ethercat.MainLoop();
        delayMicroseconds(100);            //wait for 100us

}
