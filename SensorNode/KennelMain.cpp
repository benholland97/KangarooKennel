#include "Configuration.hpp"
#include "MyXbee.hpp"
#include "Sensor.hpp"

MyXbee *xbee;
Sensor *sensor;
char receivedData[ZB_PACKET_SIZE];
int txDelay;
bool delaySetFlag;


int isMotionDetected() {

  unsigned long timeRef = millis(); // takes reference point for getTimeDiff()
  Serial.println("Searching for presence");

  while((millis() - timeRef)/1000.0 < PIR_TIMEOUT ) {
    if(digitalRead(11)) {
      Serial.println("Dog darn doggy is here");
      return 1;
    }
  }

  return 0;
}

int isDoorOpen() {
  return digitalRead(10);
}

void setup() {
  Serial.begin(9600);
  
  //Door + PIR sensor setup
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_PIR, LOW);

  delaySetFlag = false;
  
  xbee = new MyXbee();

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

    //True on immediate loop following door close
    if (delaySetFlag == false) {
      Serial.println("Setting delay");
      Serial.println(txDelay);
      //if motion detected set delay to present if not set delay to absent
      txDelay = isMotionDetected()? TX_DELAY_PRES : TX_DELAY_ABS;
      delaySetFlag = true;
    }
    
    //Send Data 
    sensor->getData();
    float temp = sensor->getTempC();
    float humidity = sensor->getHumidity();
    float pres;
    
    if (txDelay == TX_DELAY_PRES) {
      pres = 1.0;
      Serial.println("Dog detected");
    } else {
      pres = 0.0;
      Serial.println("No Dog detected");
    }
   
    xbee->setPayload(temp,humidity,pres);
    xbee->sendMessage();
    
    Serial.println("Sending data");
    digitalWrite(PIN_LED, HIGH);
    delay(txDelay);      
    digitalWrite(PIN_LED, LOW);
    delay(txDelay);   
  }
  
  delaySetFlag = false;
  Serial.println("Door open");
  //Door open

  Serial.println("");
}


