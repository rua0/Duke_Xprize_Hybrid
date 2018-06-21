#include <Wire.h>
#include <Servo.h>
#include <string.h>

// SET PIN
#define ESC_PIN 3
Servo escServo;

// DEFINE MIN/MAX PULSE LENGTH IN MILLISECONDS (ms)
const double ESC_MIN_PL = 1;
const double ESC_MAX_PL = 2;

// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 100 = 100% = full speed forwards
    // -100 = -100% = full speed backwards
const int ESC_MIN = -100;
const int ESC_MAX = 100;


// DEFINE SERIAL INPUT
int percentThrottle = 0;
double pulseLength = 1500;
char serialString[100];


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
  escServo.writeMicroseconds(1800);
  delay(10000);
  Serial.println("Starting Final Phase 4 - sending 2.0 ms pulse");
  escServo.writeMicroseconds(2000);
  delay(5000);
  // stop after initialization
  Serial.println("Motor Stopped, ready to go!");
  escServo.writeMicroseconds(1500);

}


void setup() {
  // setup esc servo
  escServo.attach(ESC_PIN);
  
  Serial.begin(9600);


  initializeESC();
  
  
  yield();
}


// maps input value to output value for given input range and output range
int mapVal(int val, int fromMin, int fromMax, int toMin, int toMax){
  int out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}


// maps throttle percentage to PWM pulse length in μs (converts ms ->μs!!!)
int convertToPL(int percentThrottle){
  int pulseLength = mapVal(percentThrottle,ESC_MIN,ESC_MAX,ESC_MIN_PL*pow(10,3),ESC_MAX_PL*pow(10,3));
  return pulseLength;
}

void sendMotorPulse(int percentThrottle){
  double pulseLength = convertToPL(percentThrottle);
  escServo.writeMicroseconds(pulseLength);
}

void loop() {
  if (Serial.available()>0){
      percentThrottle = Serial.parseInt();
      pulseLength = convertToPL(percentThrottle);
  }

  sprintf(serialString,"percent throttle: %d \t pulse length: %d", percentThrottle, (int)pulseLength);
  Serial.println(serialString);

  // convert throttle decimal percentage into pulse length duration and send pulse
  sendMotorPulse(percentThrottle);

  delay(500);

}
