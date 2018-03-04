#include "Configuration.hpp"
#include "MyXbee.hpp"
#include "Sensor.hpp"

MyXbee *xbee;
Sensor *sensor;
char receivedData[ZB_PACKET_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up");
  delay(5000);
  xbee = new MyXbee();
  xbee->setPayload("This is");

  sensor = new Sensor();
}

void loop() {
  Serial.flush();
//  xbee->sendMessage();
  delay(2000);

  sensor->getData();

  Serial.print("Temp : ");
  Serial.println(sensor->getTempC());
 
//  if (xbee->receiveMessage()) {
//    xbee->getMessage(receivedData);
//    Serial.println(receivedData);
//  }

}

