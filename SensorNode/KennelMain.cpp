#include "Configuration.hpp"
#include "MyXbee.hpp"
#include "Sensor.hpp"

MyXbee *xbee;
Sensor *sensor;
char receivedData[ZB_PACKET_SIZE];
int txDelay;
bool delaySetFlag;


int isMotionDetected() {
  return digitalRead(11);
}

int isDoorOpen() {
  return digitalRead(10);
}

void setup() {
  Serial.begin(9600);
  delay(5000);

  //Door + PIR sensor setup
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_PIR, LOW);

  delaySetFlag = false;
  
  xbee = new MyXbee();
  char msg[] = {"24.4,30.5,1,"};
  xbee->setPayload(msg);

  sensor = new Sensor();

  
}

void loop() {
  
  while(!isDoorOpen()) {
    
    Serial.println("Door closed");
    //Door closed

    if (xbee->receiveMessage()) {
      xbee->getMessage(receivedData);
      Serial.println(receivedData);
    }
    
    if (delaySetFlag == false) {
      Serial.println("Setting delay");
      //if motion detected set delay to present if not set delay to absent
      txDelay = isMotionDetected()? TX_DELAY_PRES : TX_DELAY_ABS;
      delaySetFlag = true;
    }
    
    //Send Data 
    sensor->getData();
    
    Serial.println("Sending data");
    digitalWrite(PIN_LED, HIGH);
    delay(txDelay);      
    digitalWrite(PIN_LED, LOW);
    delay(txDelay);   
  }
  
  delaySetFlag = false;
  Serial.println("Door open");
  //Door open
}


