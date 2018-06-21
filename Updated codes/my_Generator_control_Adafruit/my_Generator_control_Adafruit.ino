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

void setup() {
  // initiate PWM signal
  pwm.begin();
  pwm.setPWMFreq(FREQ);
  
  // setup servos
  //yao: not necessary, if using adafruit board
//  chokeServo.attach(CHOKE_PIN);
//  throttleServo.attach(THROTTLE_PIN);
//  escServo.attach(ESC_PIN);
  
  Serial.begin(9600);

  Serial.println("Initiating Ignition Sequence");
  
  // CLOSED THROTTLE, CLOSED CHOKE
  setServoLow(THROTTLE_PIN);
  setServoLow(CHOKE_PIN);

  //initializeMotor();

  setServoLow(ESC_PIN);
  
  Serial.println("Motor Stopped");

  //no point since scheduler.h lib is not used
//  yield();
}

void loop() {
  
//     if(MODE == 0) {
//        Serial.println("Waiting for command: 1/2/3 - Start Motor, 4/5 - Idle Motor, 6 - Stop Motor (Choke)");
//     }
//     if(Serial.available()){
//        MODE = Serial.parseInt();
//     }
     
     switch (MODE) {
       case 1:
         Serial.println("Initiating Ignition Sequence");
         // CLOSED THROTTLE, CLOSED CHOKE
         //commented out for now
         //setServoHigh(SPARK_PIN);
         sendPulse(THROTTLE_PIN, 40);
         setServoLow(CHOKE_PIN);
         
         setServoHigh(ESC_PIN);
         
         
         
         sendPulse(ESC_PIN, 40);
         break;
      
       case 2:
         Serial.println("Moving to Start Sequence Phase 2...");
         sendPulse(ESC_PIN, 40);
         
         sendPulse(THROTTLE_PIN, 40);
         sendPulse(CHOKE_PIN, 50);
         
         break;
         
       case 3:
         Serial.println("Moving to Start Sequence Phase 3...");
         sendPulse(ESC_PIN, 40);
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 50);
         break;
         
       case 4:
         Serial.println("Initiating Idle Sequence...");
         sendPulse(ESC_PIN, 0.2);
         sendPulse(THROTTLE_PIN, 0.65);
         sendPulse(CHOKE_PIN, 0.5);
         break;
         
       case 5:
         Serial.println("Moving to Idle Sequence Phase 2..");
         sendPulse(ESC_PIN, 0.0);
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 50);
         break;
         
       case 6:
         Serial.println("Initiating Stop Sequence and Disabling Ignition");
         //commented out for now
         //setServoLow(SPARK_PIN);
 
         sendPulse(ESC_PIN, 0.0);
         sendPulse(THROTTLE_PIN, 0.0);
         sendPulse(CHOKE_PIN, 100);
         break;
      
         
       default:
         break;
       
     }
     if(MODE!=6){
      MODE+=1;
     }
     delay(1500);
}
