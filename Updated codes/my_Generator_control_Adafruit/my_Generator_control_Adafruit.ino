//Author: Yao
//Email: yy123@duke.edu
//dependency: <Adafruit_PWMServoDriver.h> library

#include <string.h>
#include "myController.h"

EngineTypeDef h_eng;//engine handler

void setup() {
  
  Serial.begin(2000000);
  Serial.println("Started");
  //handler of engine gives high level interfaces
  //init low level drivers
  h_eng.init();

}

void loop() {
  //maybe put in a timer interrupt
  //acquire sensor data
  h_eng.acq();
  //execute and update output data
  h_eng.exe();
  delay(100);
}
