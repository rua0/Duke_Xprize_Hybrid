
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// SET PIN
#define ESC_PIN 9

// DEFINE PWM FREQ IN Hz
#define FREQ 50

// DEFINE NUMBER OF TICKS PER PULSE
#define NUM_TICKS  4096

// CALCULATE TICK LENGTH IN MILLISECONDS (ms)
//added 1.0 to make right hand side castable to double
const double TICK_LENGTH = pow(10,3)/(1.0*FREQ * NUM_TICKS); // gives ms/tick

// DEFINE MIN/MAX PWM LENGTHS IN MILLISECONDS (ms)
const double ESC_MIN_PL = 1;
const double ESC_MAX_PL = 2;


// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 1.0 = 100% = full speed forwards
    // -1.0 = -100% = full speed backwards
const double ESC_MIN = 0;
const double ESC_MAX = 100;

int minTick = round(ESC_MIN_PL/TICK_LENGTH);
int maxTick = round(ESC_MAX_PL/TICK_LENGTH);


// user input from serial monitor
int percentThrottle;
char serialString[100];


// maps input value to output value for given input range and output range
double mapVal(double val, double fromMin, double fromMax, double toMin, double toMax){
  double out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}

// maps throttle percentage to PWM pulse length in ticks
int convertToTicks(double percentThrottle){
  double pulseLength = mapVal(percentThrottle,ESC_MIN,ESC_MAX,ESC_MIN_PL,ESC_MAX_PL); // gives desired pulse length in ms
  int pulseTicks = round(pulseLength/TICK_LENGTH);
  if (pulseTicks = NUM_TICKS){
    pulseTicks = pulseTicks - 1;
  }
  
  return pulseTicks;
}



// sends pwm pulse based on decimal percentage of throttle 
void sendMotorPulse(double percentThrottle){
  
  
 
  if (maxTick >= NUM_TICKS){          // tick index can't exceed 4095
    maxTick = NUM_TICKS-1;
  }

  // map pulse tick to value in between min/max ticks
  //map percent throttle 0-100 to tick 1024-2048 (duty cycle 25%-50%)
  int pulseTick = mapVal(percentThrottle,ESC_MIN,ESC_MAX,minTick,maxTick);
  if (pulseTick >= NUM_TICKS){          // tick index can't exceed 4095
    pulseTick = NUM_TICKS-1;
  }
  Serial.print("pulseTick is");
  Serial.println(pulseTick);
  pwm.setPWM(ESC_PIN,0,pulseTick);

}

void initializeMotor(){
  Serial.println("Initializing...");
  Serial.println("Starting Phase 1 - sending 1.0 ms pulse");
//  escServo.writeMicroseconds(1000);
  sendMotorPulse(0);
  delay(5000);
  Serial.println("Starting Phase 2 - sending 1.65 ms pulse");
//  escServo.writeMicroseconds(1650);
  sendMotorPulse(65);
  delay(5000);
  Serial.println("Starting Phase 3 - sending 1.8 ms pulse");
//  escServo.writeMicroseconds(1800);
  sendMotorPulse(80);
  delay(10000);
  Serial.println("Starting Final Phase 4 - sending 2.0 ms pulse");
//  escServo.writeMicroseconds(2000);
  sendMotorPulse(100);
  delay(5000);
  // stop after initialization
  Serial.println("Motor Stopped, ready to go!");
//  escServo.writeMicroseconds(1500);
  sendMotorPulse(50);
}
// send 0 PWM signal
void setDigitalLow(uint8_t servoNum) {
  pwm.setPWM(servoNum,0,0);
}

// send MAX PWM signal
void setDigitalHigh(uint8_t servoNum) {
  pwm.setPWM(servoNum,0,NUM_TICKS-1);
}


// send motor min throttle
void setMotorLow(){
  
  // calculate min tick
  int minTick = round(ESC_MIN_PL/TICK_LENGTH);
 
  pwm.setPWM(ESC_PIN,0,minTick);
  
}

// send motor max throttle
void setMotorHigh(){
  
  // calculate max tick
  int maxTick = round(ESC_MAX_PL/TICK_LENGTH);
  if (maxTick >= NUM_TICKS){          // tick index can't exceed 4095
    maxTick = NUM_TICKS-1;
  }
  
  pwm.setPWM(ESC_PIN,0,maxTick);
}


 void setup() {
  // initiate PWM signal
  pwm.begin();
  pwm.setPWMFreq(FREQ);
  
  
  Serial.begin(9600);
  
  yield();
 
  // setup generator
  setDigitalLow(ESC_PIN);
 
  while (!Serial) {
    ;
  }
  
  Serial.println("All systems go!");
  //print a bunch of useful parameters
  Serial.print("minTick is");
  Serial.println(minTick);
  Serial.print("maxTick is");
  Serial.println(maxTick);
  // calculate min/max ticks
  Serial.print("TICK_LENGTH IS ");
  Serial.println(TICK_LENGTH*1000);
  initializeMotor();
 }




void loop() {

  //test infitie loop
//  int percentThrottle=0;
//  while(1){
//    sendMotorPulse(percentThrottle);
////    int pulseTick = mapVal(percentThrottle,ESC_MIN,ESC_MAX,minTick,maxTick);
////    Serial.print("mapped pulseTick is");
////    Serial.println(pulseTick);
//    percentThrottle+=10;
//    if(percentThrottle=100) percentThrottle=0;
//    delay(1000);
//    
//  }
//----------------------

  if(Serial.available()){
  percentThrottle = Serial.parseInt();
  }
  
  // convert value from 0% to 100% throttle into pulse length duration; negative percent mean backwards rotation
  double pulseLength = map(percentThrottle,ESC_MIN,ESC_MAX,ESC_MIN_PL,ESC_MAX_PL);
  
  int pulseTicks = convertToTicks(percentThrottle);
  
  sprintf(serialString,"percent throttle: %d \t pulse Ticks:0 - %d \t pulse length: %f", percentThrottle, pulseTicks, pulseLength);
  
   
  Serial.print(serialString);
  Serial.println(pulseLength);
  

  // send pulse
  sendMotorPulse(percentThrottle);
  
    
  delay(1500);

  
}
