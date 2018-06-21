#include <Wire.h>
#include <Servo.h>
#include <string.h>

// SET PIN
#define SERVO_PIN 1
Servo myServo;

// DEFINE MIN/MAX PULSE LENGTH IN MILLISECONDS (ms)
const double minPL = 1;
const double maxPL = 2;

// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 100 = 100% = full speed forwards
    // -100 = -100% = full speed backwards
const int minThrottle = -100;
const int maxThrottle = 100;


// DEFINE SERIAL INPUT
int percentThrottle = 0;
double pulseLength = 1500;
char serialString[100];


void setup() {
  // setup esc servo
  myServo.attach(SERVO_PIN);

  Serial.println("Initializing...");
  // sweep over pwm lengths from 1 ms to 2 ms and read the angle of the servo
  for (int pulseLength = 1000; pulseLength <= 2000; pulseLength++) { 
    myServo.writeMicroseconds(pulseLength);     
    delay(1500);                                 
  }
  
  Serial.begin(9600);

  
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
  int pulseLength = mapVal(percentThrottle,minThrottle,maxThrottle,minPL*pow(10,3),maxPL*pow(10,3));
  return pulseLength;
}

void sendPulse(int percentThrottle){
  double pulseLength = convertToPL(percentThrottle);
  myServo.writeMicroseconds(pulseLength);
}

void loop() {
  if (Serial.available()>0){
      percentThrottle = Serial.parseInt();
      pulseLength = convertToPL(percentThrottle);
  }

  sprintf(serialString,"percent throttle: %d \t pulse length: %d", percentThrottle, (int)pulseLength);
  Serial.println(serialString);

  // convert throttle decimal percentage into pulse length duration and send pulse
  sendPulse(percentThrottle);

  delay(500);

}
