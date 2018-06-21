#include <Wire.h>
#include <Servo.h>
#include <string.h>

//SET PINS FOR SERVOS AND ESC
#define CHOKE_PIN  6
#define THROTTLE_PIN  5
#define ESC_PIN  3 
Servo sparkServo;
Servo chokeServo;
Servo throttleServo;
Servo escServo;


// DEFINE MIN/MAX PWM LENGTHS IN MILLISECONDS (ms)
const double CHOKE_MIN_PL = 0.650; // closed choke, min air
const double CHOKE_MAX_PL = 1.200; // open choke, max air
 
const double THROTTLE_MIN_PL = 1.250; // closed (originally open/full throttle, max gas)
const double THROTTLE_MAX_PL = 1.925; // open   (closed/min throttle, min gas)

const double ESC_MIN_PL = 1;
const double ESC_MAX_PL = 2;


// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 100 = 100% = full speed forwards
    // -100 = -100% = full speed backwards -- SPARK, THROTTLE, AND CHOKE SERVOS DON'T MOVE IN REVERSE DIRECTION!!!

const double CHOKE_MIN = 0;
const double CHOKE_MAX = 100;

const double THROTTLE_MIN = 0;
const double THROTTLE_MAX = 100;

const double ESC_MIN = -100;
const double ESC_MAX = 100;


// DEFINE SERIAL INPUT
int percent = 0;
double pulseLength = 1500;
char serialString[100];
int MODE = 0;


// maps input value to output value for given input range and output range
double mapVal(double val, double fromMin, double fromMax, double toMin, double toMax){
  double out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}


// sends pwm pulse based on decimal percentage of throttle/amount of openness
// if ESC, percent = how fast motor is spinning
// if CHOKE SERVO, percent  = how open it is
// if THROTTLE SERVO, percent = how closed it is
void sendPulse(int servoNum, double percent){
  double minPL,maxPL, minPercent, maxPercent;  
  Servo myServo;
  if (servoNum == CHOKE_PIN){
    minPL = CHOKE_MIN_PL;
    maxPL = CHOKE_MAX_PL;
    minPercent = CHOKE_MIN;
    maxPercent = CHOKE_MAX;
    myServo = chokeServo;
  }
  else if (servoNum == THROTTLE_PIN){
    minPL = THROTTLE_MIN_PL;
    maxPL = THROTTLE_MAX_PL;
    minPercent = THROTTLE_MIN;
    maxPercent = THROTTLE_MAX;
    myServo = throttleServo;
  }
  else if (servoNum == ESC_PIN){
    minPL = ESC_MIN_PL;
    maxPL = ESC_MAX_PL;    
    minPercent = ESC_MIN;
    maxPercent = ESC_MAX;
    myServo = escServo;
  }

  double pulseLength = mapVal(percent,minPercent,maxPercent,minPL*pow(10,3),maxPL*pow(10,3));
  myServo.writeMicroseconds(pulseLength);

}

// send servo to min position
// if ESC, motor off
// if CHOKE SERVO, closes
// if THROTTLE SERVO, opens
void setServoLow(int servoNum){
 double minPL;
 Servo myServo;
 if (servoNum == CHOKE_PIN){
    minPL = CHOKE_MIN_PL;
    myServo = chokeServo;
  }
  else if (servoNum == THROTTLE_PIN){
    minPL = THROTTLE_MIN_PL;
    myServo = throttleServo;
  }
  else if (servoNum == ESC_PIN){
    minPL = ESC_MIN_PL;
    myServo = escServo;
  }

  myServo.writeMicroseconds(minPL*pow(10,3));
  
}


// send servo to max position
// if ESC, motor at max rpm
// if CHOKE SERVO, opens
// if THROTTLE SERVO, closes
void setServoHigh(int servoNum){
  double maxPL;
  Servo myServo;
 if (servoNum == CHOKE_PIN){
    maxPL = CHOKE_MAX_PL;
    myServo = chokeServo;
  }
  else if (servoNum == THROTTLE_PIN){
    maxPL = THROTTLE_MAX_PL;
    myServo = throttleServo;
  }
  else if (servoNum == ESC_PIN){
    maxPL = ESC_MAX_PL;
    myServo = escServo;
  }

  myServo.writeMicroseconds(maxPL*pow(10,3));
  
}


// initializes ESC PWM
void initializeESC(){
  Serial.println("Initializing...");
  Serial.println("Starting Phase 1 - sending 1.0 ms pulse");
  escServo.writeMicroseconds(1000);
  delay(5000);
  Serial.println("Starting Phase 2 - sending 1.65 ms pulse");
  escServo.writeMicroseconds(1650);
  delay(5000);
  Serial.println("Starting Phase 3 - sending 1.8 ms pulse");
  escServo.writeMicroseconds(1700);
  delay(10000);
  Serial.println("Starting Final Phase 4 - sending 2.0 ms pulse for 10 seconds");
  escServo.writeMicroseconds(2000);
  delay(10000);
  // stop after initialization
  Serial.println("Done Initializing!");
  escServo.writeMicroseconds(1500);
  
}


void setup() {
  
  // setup servos
  chokeServo.attach(CHOKE_PIN);
  throttleServo.attach(THROTTLE_PIN);
  escServo.attach(ESC_PIN);
  
  Serial.begin(9600);

  Serial.println("Initiating Ignition Sequence");
  // CLOSED THROTTLE, CLOSED CHOKE
  setServoLow(THROTTLE_PIN);
  setServoLow(CHOKE_PIN);

  initializeESC();

  setServoLow(ESC_PIN);
  Serial.println("Motor Stopped");
  
  yield();
  //??? do not understand this
}



void loop() {
  /* HERE WE HAVE MODIFIED SOME TENTATIVE CODE FROM A PREVIOUS GROUP THAT RUNS THE ENGINE USING DIFFERENT MODES WHICH CORRESPOND TO DIFFERENT SPEEDS. 
  HOWEVER WE NEVER GOT THE OPPORTUNITY TO USE IT SO ITS VIABILITY IS UNCERTAIN.
  */
  
     
     if(MODE == 0) {
        Serial.println("Waiting for command: 1/2/3 - Start Motor, 4/5 - Idle Motor, 6 - Stop Motor (Choke)");
     }
 
     MODE = Serial.parseInt();
     switch (MODE) {
       case 1:
         Serial.println("Initiating Ignition Sequence");
         // CLOSED THROTTLE, CLOSED CHOKE
         //setServoHigh(SPARK_PIN); //shouldn't commented out
        // setServoLow(THROTTLE_PIN, 0.4);//shouldn't commented out
         setServoLow(CHOKE_PIN);
         
         setServoHigh(ESC_PIN);
         
         delay(1500);
         
         sendPulse(ESC_PIN, 0.4);
         break;
      
       case 2:
         Serial.println("Moving to Start Sequence Phase 2...");
         sendPulse(ESC_PIN, 0.4);
         
         sendPulse(THROTTLE_PIN, 0.4);
         sendPulse(CHOKE_PIN, 0.5);
         
         break;
         
       case 3:
         Serial.println("Moving to Start Sequence Phase 3...");
         sendPulse(ESC_PIN, 0.4);
         sendPulse(THROTTLE_PIN, 0.65);
         sendPulse(CHOKE_PIN, 0.5);
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
         sendPulse(THROTTLE_PIN, 0.65);
         sendPulse(CHOKE_PIN, 0.5);
         break;
         
       case 6:
         Serial.println("Initiating Stop Sequence and Disabling Ignition");
         
         //setServoLow(SPARK_PIN);
 
         sendPulse(ESC_PIN, 0.0);
         sendPulse(THROTTLE_PIN, 0.0);
         sendPulse(CHOKE_PIN, 1.0);
         break;
      
         
       default:
         break;
       
     }
           

 }
