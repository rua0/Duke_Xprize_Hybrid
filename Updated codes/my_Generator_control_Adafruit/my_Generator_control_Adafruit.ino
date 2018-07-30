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
  
  Serial.begin(2000000);

  Serial.println("Initiating Ignition Sequence");
  
  // CLOSED THROTTLE, CLOSED CHOKE
  setServoLow(THROTTLE_PIN);
  setServoLow(CHOKE_PIN);
  sendPulse(CHOKE_PIN,25);
  
  //initializeMotor();

  setServoLow(ESC_PIN);
  
  Serial.println("Motor Stopped");

  Serial.println("Type: $(MODE) $(Parameter, if any)");

  //no point since scheduler.h lib is not used
//  yield();
}

#define PRINT_VALUE
float thr_in=35;
float thr=0;
int thri=0;

void loop() {
    //test();
    //20 is the min to start it 
    
   
//  while(1){
//    if(Serial.available()){
//      thri=Serial.parseFloat();
//      Serial.println(thri);
//      //THROTTLE_PIN
//      //ESC_PIN
//      sendPulse(THROTTLE_PIN,thri);
//    }
//    
//  }
  //IGNORE low esc is 7 if started
  //IGNORE low esc is 11 if not started
    //take CMD if available
    //take CMD only if only mode number is received
     if(Serial.available()){
//        Serial.print("Available ");
//        Serial.println(Serial.available());
        MODE = Serial.parseInt();
     }
     
     if(MODE!=prev_MODE||prev_MODE==5){
        //MODE changed
        prev_MODE=MODE;
        Serial.print("Mode: ");
        Serial.println(MODE);
     switch (MODE) {
       case 0:
         Serial.println("Waiting for command: 1/2/3 - Start Motor, 4/5 - Idle Motor, 6 - Stop Motor (Choke)");
         break;
       case 1:
         Serial.println("Initiating Sequence: Warm-up");
         #ifdef PRINT_VALUE
            Serial.println("THR: 70, CHO:5 0, Motor: 35");
         #endif
         // High THROTTLE, CLOSED CHOKE
         //motor running slowly
         sendPulse(ESC_PIN, 38);
         sendPulse(THROTTLE_PIN, 70);
         sendPulse(CHOKE_PIN, 50);
         delay(300);
         sendPulse(CHOKE_PIN, 30);
         delay(2500);
         
         delay(1000);
         break;
      
       case 2:
         Serial.println("Start Sequence Phase 2...");
         #ifdef PRINT_VALUE
            Serial.println("THR: 50, CHO: 50, Motor: 35");
         #endif
         //sendPulse(ESC_PIN, 35);//not necessary if moved from phase one but fuck it
         //choke 50, throttle 50
         sendPulse(THROTTLE_PIN, 50);
         sendPulse(CHOKE_PIN, 50);
         
         break;
         
       case 3:
       //open choke and increase throttle
         Serial.println("Start Sequence Phase 3...");
         #ifdef PRINT_VALUE
            Serial.println("THR: 65, CHO: 50, Motor: 25");
         #endif
         //sendPulse(ESC_PIN, 25);//not necessary if moved from phase one but fuck it
         //continue to increase the throttle
         //choke half throttle 65%
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 50);
         break;
         
       case 4:
       //disengage ESC
         Serial.println("Idle Sequence 1...");
         #ifdef PRINT_VALUE
            Serial.println("THR: 65, CHO: 60, Motor: 20");
         #endif
         sendPulse(ESC_PIN, 28);
         sendPulse(THROTTLE_PIN, 65);
         sendPulse(CHOKE_PIN, 60);
         break;
         
       case 5:
         Serial.println("Moving to Idle Sequence Phase 2..");
         
         if(Serial.available()){
            thr_in = Serial.parseFloat();
            Serial.print("throttle read: ");
            Serial.println(thr_in);
          }
         #ifdef PRINT_VALUE
            Serial.println("THR: ");
            Serial.print(thr_in);
            Serial.println(", CHO: 60, Motor: 0");
         #endif
         sendPulse(ESC_PIN, 0.0);
         sendPulse(THROTTLE_PIN, thr_in);
         sendPulse(CHOKE_PIN, 100);
         break;
         
       case 6:
         Serial.println("Initiating Stop Sequence and Disabling Ignition");
         //commented out for now
         //setServoLow(SPARK_PIN);
          #ifdef PRINT_VALUE
            Serial.println("THR: 0, CHO: 100, Motor: 0");
         #endif
         sendPulse(ESC_PIN, 0);
         sendPulse(THROTTLE_PIN, 0);
         sendPulse(CHOKE_PIN, 100);
         break;
      
         
       default:
         break;
       
     }
     }
     delay(800);
     if(MODE<=3&&MODE>0){
      MODE+=1;
     }
     //delay(1500);
}
