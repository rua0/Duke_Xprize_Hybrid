#include "myfunc.h"
#include "myGloab.h"

//extern Adafruit_PWMServoDriver pwm
//extern int percent;
//extern double pulseLength;
//extern char serialString[100];
//extern int MODE;
int percent = 0;
double pulseLength = 1500;
char serialString[100];
int MODE = 0;
int prev_MODE = -1;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//should work
// maps input value to output value for given input range and output range
double mapVal(double val, double fromMin, double fromMax, double toMin, double toMax){
  double out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}

//should work
// sends pwm pulse based on decimal percentage of throttle/amount of openness
// if ESC, percent = how fast motor is spinning
// if CHOKE SERVO, percent  = how open it is
// if THROTTLE SERVO, percent = how closed it is
void sendPulse(int servoNum, double percent){
  double minPL,maxPL, minPercent, maxPercent;  
 // Servo myServo;
  if (servoNum == CHOKE_PIN){
    minPL = CHOKE_MIN_PL;
    maxPL = CHOKE_MAX_PL;
    minPercent = CHOKE_MIN;
    maxPercent = CHOKE_MAX;
    
  }
  else if (servoNum == THROTTLE_PIN){
    minPL = THROTTLE_MIN_PL;
    maxPL = THROTTLE_MAX_PL;
    minPercent = THROTTLE_MIN;
    maxPercent = THROTTLE_MAX;
    
  }
  else if (servoNum == ESC_PIN){
    minPL = ESC_MIN_PL;
    maxPL = ESC_MAX_PL;    
    minPercent = ESC_MIN;
    maxPercent = ESC_MAX;
    
  }
//  Serial.println("minPL maxPL");
//  Serial.print(minPL);
//  Serial.println(maxPL);

  int minTick = round(minPL/TICK_LENGTH);
  int maxTick = round(maxPL/TICK_LENGTH);
  double pulseTick = mapVal(percent,minPercent,maxPercent,minTick,maxTick);
//  myServo.writeMicroseconds(pulseLength);
//yao: replaced with adafruit lib func
//  Serial.print("pulseTick is");
//  Serial.println(pulseTick);
  pwm.setPWM(servoNum,0,pulseTick);
}


//should work
// send servo to min position
// if ESC, motor off
// if CHOKE SERVO, closes
// if THROTTLE SERVO, opens
void setServoLow(int servoNum){
 //double minPL;
double minPL,maxPL, minPercent, maxPercent;  
 // Servo myServo;
  if (servoNum == CHOKE_PIN){
    minPL = CHOKE_MIN_PL;
    maxPL = CHOKE_MAX_PL;
    minPercent = CHOKE_MIN;
    maxPercent = CHOKE_MAX;
    
  }
  else if (servoNum == THROTTLE_PIN){
    minPL = THROTTLE_MIN_PL;
    maxPL = THROTTLE_MAX_PL;
    minPercent = THROTTLE_MIN;
    maxPercent = THROTTLE_MAX;
   
  }
  else if (servoNum == ESC_PIN){
    minPL = ESC_MIN_PL;
    maxPL = ESC_MAX_PL;    
    minPercent = ESC_MIN;
    maxPercent = ESC_MAX;
    
  }
  int minTick = round(minPL/TICK_LENGTH);
  int maxTick = round(maxPL/TICK_LENGTH);
  double pulseTick = mapVal(percent,minPercent,maxPercent,minTick,maxTick);
//  myServo.writeMicroseconds(pulseLength);
//yao: replaced with adafruit lib func
//  Serial.print("pulseTick is");
//  Serial.println(pulseLength);
  pwm.setPWM(servoNum,0,pulseTick);
  //myServo.writeMicroseconds(minPL*pow(10,3));
  
}


// send servo to max position
// if ESC, motor at max rpm
// if CHOKE SERVO, opens
// if THROTTLE SERVO, closes
void setServoHigh(int servoNum){
  double minPL,maxPL, minPercent, maxPercent;  
 // Servo myServo;
  if (servoNum == CHOKE_PIN){
    minPL = CHOKE_MIN_PL;
    maxPL = CHOKE_MAX_PL;
    minPercent = CHOKE_MIN;
    maxPercent = CHOKE_MAX;
    
  }
  else if (servoNum == THROTTLE_PIN){
    minPL = THROTTLE_MIN_PL;
    maxPL = THROTTLE_MAX_PL;
    minPercent = THROTTLE_MIN;
    maxPercent = THROTTLE_MAX;
    
  }
  else if (servoNum == ESC_PIN){
    minPL = ESC_MIN_PL;
    maxPL = ESC_MAX_PL;    
    minPercent = ESC_MIN;
    maxPercent = ESC_MAX;
    
  }

  double pulseLength = mapVal(maxPL,minPercent,maxPercent,minPL*pow(10,3),maxPL*pow(10,3));
//  myServo.writeMicroseconds(pulseLength);
//yao: replaced with adafruit lib func

  pwm.setPWM(servoNum,0,pulseLength);
  
}


// initializes ESC PWM

void initializeMotor(){
  Serial.println("Initializing...");
  Serial.println("Starting Phase 1 - sending 1.0 ms pulse");
//  escServo.writeMicroseconds(1000);
  sendPulse(ESC_PIN,0);
  delay(5000);
  Serial.println("Starting Phase 2 - sending 1.65 ms pulse");
//  escServo.writeMicroseconds(1650);
  sendPulse(ESC_PIN,65);
  delay(5000);
  Serial.println("Starting Phase 3 - sending 1.8 ms pulse");
//  escServo.writeMicroseconds(1800);
  sendPulse(ESC_PIN,80);
  delay(10000);
  Serial.println("Starting Final Phase 4 - sending 2.0 ms pulse");
//  escServo.writeMicroseconds(2000);
  sendPulse(ESC_PIN,100);
  delay(5000);
  // stop after initialization
  Serial.println("Motor Stopped, ready to go!");
//  escServo.writeMicroseconds(1500);
  sendPulse(ESC_PIN,50);
}
