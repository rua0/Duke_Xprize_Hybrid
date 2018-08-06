#include <Wire.h>
//not using this any more
//#include <Servo.h>
#include <string.h>
//included in myfunc.h
//#include <Adafruit_PWMServoDriver.h>
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//used "" to use relative path
#include "myfunc.h"
#include "myGloab.h"
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
