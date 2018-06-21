#include <Servo.h>

/*
 * Utility for programming the Rotorstar 120A-HV ESC.
 * Allows you to mimic the movements of an RC radio throttle in order to 
 *    navigate the menus of the ESC programming mode and test motor operation.
 * Important note: It is recommended you use pin 3. This pin uses low frequency 
 *    pwm and the numbers here are only verified to work in this setup. 
 */

int PinToESC = 3;
int data = 0;

#define THRTL_MAX 250
#define THRTL_MIN 150

uint16_t valueTransform(float pos) {
  uint16_t mMin = THRTL_MIN;
  uint16_t mMax = THRTL_MAX;
  return (pos*(mMax-mMin))+mMin;
}

void setup() {
  pinMode(PinToESC, OUTPUT); //connect this PWM pin with the data cable of the ESC
  analogWrite(PinToESC, data);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  if (Serial.available()) {
    data = Serial.parseInt(); // Parse an Integer from Serial
    if(data == 150) {
      analogWrite(PinToESC, data);
    }
    if(data == 250) {
      analogWrite(PinToESC, data);
    }
  }
  Serial.println(data);
}
