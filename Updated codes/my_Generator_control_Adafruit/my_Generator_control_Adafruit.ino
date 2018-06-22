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
  sendPulse(CHOKE_PIN,80);
  //initializeMotor();

  setServoLow(ESC_PIN);
  
  Serial.println("Motor Stopped");

  //no point since scheduler.h lib is not used
//  yield();
}

//print tick mapping
void test(){
  //measure the numbers
  double minPL,maxPL, minPercent, maxPercent; 
  minPL = CHOKE_MIN_PL;
  maxPL = CHOKE_MAX_PL;
  minPercent = CHOKE_MIN;
  maxPercent = CHOKE_MAX;
  int minTick = round(minPL/TICK_LENGTH);
  int maxTick = round(maxPL/TICK_LENGTH);
  Serial.print("minTick is");
  Serial.println(minTick);
  Serial.print("maxTick is");
  Serial.println(maxTick);
}

//incremental test
void inc_test(){
  for(int i=0;i<=100;i+=10){
    Serial.print("i is ");
    Serial.println(i);
    //pwm.setPWM(THROTTLE_PIN,0,i);
    sendPulse(CHOKE_PIN, i);
//    sendPulse(THROTTLE_PIN, i);
    delay(500);
//        
  }
}

int thr,cho,pt;
char str[100];

//interactive tests
void inter_test(){
  thr=Serial.parseInt();
  cho=Serial.parseInt();
  sprintf(str,"throttle value is %d, choke value is %d",thr,cho);
  Serial.println(str);
  sendPulse(THROTTLE_PIN, thr);
  sendPulse(CHOKE_PIN,cho);
}

//interactive tests
void inter_pt_test(){
  pt=Serial.parseInt();
  sprintf(str,"pulse tick is %d",pt);
  Serial.println(str);
  pwm.setPWM(CHOKE_PIN,0,pt);
}

void loop() {
    test();
//    while(1){
//      
//      if(Serial.available()){
//        //debug pwm
//
//        //debug together
//        inter_test();
////        inter_pt_test();
//        delay(1000);
//      }
//      inc_test();
////      setServoLow(CHOKE_PIN);
////      setServoLow(THROTTLE_PIN);
////      setServoHigh(CHOKE_PIN);
////      setServoHigh(THROTTLE_PIN);
////      
//    }
     if(MODE == 0) {
        Serial.println("Waiting for command: 1/2/3 - Start Motor, 4/5 - Idle Motor, 6 - Stop Motor (Choke)");
     }
     if(Serial.available()){
        MODE = Serial.parseInt();
     }
     
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
       //open choke and increase throttle
         Serial.println("Moving to Start Sequence Phase 3...");
         sendPulse(ESC_PIN, 40);
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 50);
         break;
         
       case 4:
       //disengage ESC
         Serial.println("Initiating Idle Sequence...");
         sendPulse(ESC_PIN, 20);
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 50);
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
 
         sendPulse(ESC_PIN, 0);
         sendPulse(THROTTLE_PIN, 0);
         sendPulse(CHOKE_PIN, 100);
         break;
      
         
       default:
         break;
       
     }
//     if(MODE!=6){
//      MODE+=1;
//     }
     delay(1500);
}
