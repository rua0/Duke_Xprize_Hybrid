/*
Arduino Hall Effect Sensor Project from Arduino Website
*/
// Modified for nonlatching type hall effect sensor


 volatile byte revolutions;
 unsigned int rpm;
 unsigned long timeold;
 void setup()
 {
   Serial.begin(9600);
   attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
   revolutions = 0;
   rpm = 0;
   timeold = 0;
 }
 void loop()//Measure RPM
 {
   if (revolutions >= 20) { 
     rpm = 60*1000/(millis() - timeold)*revolutions;
     timeold = millis();
     revolutions = 0;
     Serial.println(rpm,DEC);
     Serial.println("Your speed is "+rpm);
   }
 }
 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   revolutions++;
   //Serial.println(revolutions);
 }
