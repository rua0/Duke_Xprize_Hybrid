int thr,cho,pt;
char str[100];

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



//interactive tests
void inter_test(){
  thr=Serial.parseInt();
//  cho=Serial.parseInt();
  sprintf(str,"throttle value is %d, choke value is %d",thr,cho);
  Serial.println(str);
//  sendPulse(THROTTLE_PIN, thr);
//  sendPulse(CHOKE_PIN,cho);
  sendPulse(ESC_PIN,thr);
}

//interactive tests
void inter_pt_test(){
  pt=Serial.parseInt();
  sprintf(str,"pulse tick is %d",pt);
  Serial.println(str);
  pwm.setPWM(CHOKE_PIN,0,pt);
}

//test esc
void test_ESC(){
	sendPulse(ESC_PIN,20);
    delay(1000);
    sendPulse(ESC_PIN,30);
    delay(1000);
    sendPulse(ESC_PIN,50);
    delay(500);
    setServoLow(ESC_PIN);
}

//TEST infinite loop
//    while(1){
//      
//      if(Serial.available()){
//        //debug pwm
//
//        //debug together
////        inter_test();
////        inter_pt_test();
//        //delay(1000);
//      }
//      //inc_test();
////      setServoLow(CHOKE_PIN);
////      setServoLow(THROTTLE_PIN);
////      setServoHigh(CHOKE_PIN);
////      setServoHigh(THROTTLE_PIN);
////      
//    }