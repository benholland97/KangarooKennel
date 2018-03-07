#include "PresenceSensor.hpp"

PresSensor::PresSensor() {
  pinMode(PIN_RLD, OUTPUT);
  pinMode(PIN_YLD, OUTPUT);
  pinMode(PIN_GLD, OUTPUT);
  pinMode(PIN_DR, INPUT);
  pinMode(PIN_MTN, INPUT);
  sensorInterval_absent = 3;
  sensorInterval_present = 1;
  sensorInterval = sensorInterval_absent;
  pirTimeLimit = 3;
  timeStart = millis();
}

void PresSensor::start() {
    // Waiting for door opening AND subsequent closure
  
  inputCheck = digitalRead(PIN_DR); 
  inputLoop = 1;
  
  timeStart = millis();
  
  while(inputLoop) {  // these loops wait for a CHANGE in input, AND a LOW/HIGH reading (more robust?)
      
      if( (digitalRead(PIN_DR) != inputCheck) && (digitalRead(PIN_DR)==LOW) ) {  
           inputCheck = digitalRead(PIN_DR);
      
            while(inputLoop) {
            
                if( (digitalRead(PIN_DR) != inputCheck) && (digitalRead(PIN_DR)==HIGH) ) {
                    inputLoop = 0;
                
            }   
         }
      }
  
      if( getTimeDiff(timeStart) >= sensorInterval ) {
              
        timeStart = millis();   // resets counter every interval
  
        // BELOW REPRESENTS SENSOR READING (just a flashing LED) -- put your sensor functions here!
        digitalWrite(PIN_RLD, HIGH); delay(25); digitalWrite(PIN_RLD, LOW);  
        
      }
      
  }
  
  // PIR sensor polling -- calling getPresence()
  
  digitalWrite(PIN_YLD, HIGH);
  
  if( getPresence()==1 )                              //
      sensorInterval = sensorInterval_present;        //  Sets the sensor polling time interval based on whether
                                                      //  getPresence returned 0 (absent) or 1 (present)    
  else                                                //
      sensorInterval = sensorInterval_absent;         //
  
  digitalWrite(PIN_YLD, LOW);
}
  



int PresSensor::getPresence() {
    unsigned long timeRef = millis(); // takes reference point for getTimeDiff()
    
    while(1) {
        
        if( digitalRead(PIN_MTN)==HIGH )   // was motion detected?
            return (1);

        if( getTimeDiff(timeRef) > pirTimeLimit )   // was time limit breached?
            return (0);
            
    }  
}


// HELPER FUNCTIONS

//  Timer 0 (TMR0) has an interrupt set to trigger every millisecond - the function millis() retrieves
//  how many milliseconds have passed since the program started - the function getTimeDiff() compares
//  current value of millis() to one taken earlier (_timeStart)

unsigned long PresSensor::getTimeDiff(unsigned long _timeStart) {

  return( (millis() - _timeStart)/1000.0 );
}


