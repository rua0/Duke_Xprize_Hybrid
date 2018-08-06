#include "myfunc.h"
#include "myGloab.h"
#include "myController.h"
EngineTypeDef::EngineTypeDef(){

}
void EngineTypeDef::init(){

    // initiate PWM signal
    pwm.begin();
    pwm.setPWMFreq(FREQ);
    Serial.println("pwm init");

    thr=0;
    chk=25;
    ESC=0;
    tmp=0;
    STAGE=0;
    prev_STAGE=-1;
    Serial.println("var init");

    // CLOSED THROTTLE, CLOSED CHOKE
    setServoLow(THROTTLE_PIN);
    setServoLow(CHOKE_PIN);
    setServoLow(ESC_PIN);
    setPulse(CHOKE_PIN,chk);

    Serial.println("choke, throttle, ESC low");

    Serial.println("Type: $(STAGE/MODE/Sequence#) $(Parameter, if any)");
    
    Serial.println("Type 1 to fire");
    
}
void EngineTypeDef::acq(){
    //read sensor data
}

#define PRINT_VALUE
void EngineTypeDef::update(){
    //update servo values
    setPulse(ESC_PIN, ESC);
    setPulse(THROTTLE_PIN, thr);
    setPulse(CHOKE_PIN, chk);
}

void EngineTypeDef::exe(){
    if(Serial.available()){
        //take Serial input as stage change if the previous stage isn't 5
        if(prev_STAGE!=5){
            STAGE = Serial.parseInt();
        }else{
        
            tmp=Serial.parseFloat();
            if (tmp!=1000)//not escaping, update throttle
            {
                thr=tmp;
                #ifdef PRINT_VALUE
                    Serial.print("Updating throttle to ");
                    Serial.println(thr);
                #endif
            }else{
                STAGE = Serial.parseInt();
            }
        }
    }

    //stage changed, updates later
    if(STAGE!=prev_STAGE){
        Serial.print("STAGE: ");
        Serial.println(STAGE);  
    }
    
    switch (STAGE) {

        //start stage
        case 0:
            if(STAGE!=prev_STAGE){
                Serial.println("Waiting for command: 1 - Start Motor, 4/5 - Idle Motor, 100 - Stop Motor (Choke)");
            }
            break;

        case 1:
            Serial.println("Initiating Ignition Sequence: Warm-up");
            // High THROTTLE, CLOSED CHOKE
         //motor running slowly
            ESC=38;
            thr=70;
            chk=50;
            
            update();
            //wait a bit to let choke servo move past a point
            delay(300);
            //let choke servo go to target backwards
            chk=30;
            #ifdef PRINT_VALUE
                Serial.println("THR: 70, CHO: 30, Motor: 38");
            #endif
            update();
            delay(4200);
         
        break;
      
        case 2:
            Serial.println("Start Sequence Phase 2...");
         

            thr=50;
            chk=50;
            #ifdef PRINT_VALUE
                Serial.println("THR: 50, CHO: 50, Motor: 38");
            #endif
            update();

            delay(800);
        break;
         
        case 3:
        //open choke and increase throttle
            Serial.println("Start Sequence Phase 3...");
            ESC=30;
            thr=65;
            chk=50;
            #ifdef PRINT_VALUE
                Serial.println("THR: 65, CHO: 50, Motor: 30");
            #endif
            update();
            delay(800);
        break;
         
        case 4:
        //disengaging ESC
            //first entering stage 4, waiting for change to stage 5
            if (prev_STAGE!=4)
            {
                Serial.println("Idle Sequence 1...");
                ESC=25;
            thr=65;
            chk=70;
            #ifdef PRINT_VALUE
                Serial.println("THR: 65, CHO: 70, Motor: 25");
            #endif
            update();
            delay(800);
            }
            
        break;
        
        //haven't worked on these yet
        case 5:
            //first entering stage 5, interactive
            if (prev_STAGE!=5)
            {
                Serial.println("Idle Sequence 2.., interactive");
                thr=40;
                ESC=10;
                #ifdef PRINT_VALUE
                    Serial.println("THR: 40, CHO: 70, Motor: 10");
                #endif
            }
            
            update();
        break;
         
        case 1000:
            if (prev_STAGE!=100)
            {
                Serial.println("Initiating Stop Sequence and Disabling Ignition");
                //commented out for now
                //setServoLow(SPARK_PIN);
                
                ESC=0;
            thr=0;
            chk=100;
            #ifdef PRINT_VALUE
                Serial.println("THR: 0, CHO: 100, Motor: 0");
            #endif
            update();
            }
         
        
         break;
      
         
        default:
         break;
       
    }


    //update prev_stage to current anyhow
    prev_STAGE=STAGE;
    if(STAGE<=3&&STAGE>0){
      STAGE+=1;
    }
    
}




















     
     

