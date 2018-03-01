#include "Configuration.hpp"
#include "MyXbee.hpp"

MyXbee *xbee;
char receivedData[ZB_PACKET_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up");
  delay(5000);
  xbee = new MyXbee();
  xbee->setPayload("This is");
}

void loop() {
  Serial.flush();
//  xbee->sendMessage();
  delay(2000);

 
  if (xbee->receiveMessage()) {
    xbee->getMessage(receivedData);
    Serial.println(receivedData);
  }

}

