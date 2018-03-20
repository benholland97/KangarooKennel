#include "MyXbee.hpp"

MyXbee::MyXbee() {

  xbee = XBee();
  mySerial = new SoftwareSerial(PIN_RX, PIN_TX);

//                  Tx variables
//  uint8_t payload[] = { 'B', 'O', 'O', 'T'};
//  payloadVal = "BOOT";
  //Destination XBee address
  destAddr = XBeeAddress64(ADDR_LOW, ADDR_HIGH);
  //Transmission request
//  txReq = ZBTxRequest(destAddr, payload, sizeof(payload));
  //Transmission response
  txStatus = ZBTxStatusResponse();

//                  Rx variables

  //Variable to store receieved response
  rxResp = XBeeResponse();
  //To store received data
  rxData = ZBRxResponse();
  //Modem status response
  msr = ModemStatusResponse();

  memset(rxMessage,0,sizeof(rxMessage));

  mySerial->begin(9600);
  xbee.setSerial(*mySerial);
}

MyXbee::~MyXbee() {
}

void MyXbee::setPayload(float temp, float humidity, float pres) {

  Serial.println("Setting Payload");
//  Serial.println(temp);
//  Serial.println(humidity);
//  Serial.println(pres);

  char tString[6], hString[6], pString[6], msgString[40];

  memset(msgString,0,sizeof(msgString));
  memset(tString,0,sizeof(tString));
  memset(hString,0,sizeof(pString));
  memset(pString,0,sizeof(hString));
  
  dtostrf(temp,5,2,tString);
  dtostrf(humidity,5,2,hString);
  dtostrf(pres,5,2,pString);

  strcpy(msgString, tString);
  strcat(msgString,",");
  strcat(msgString, hString);
  strcat(msgString,",");
  strcat(msgString, pString);

  Serial.println(tString);
  Serial.println(hString);
  Serial.println(pString);

  Serial.println(msgString);

  txReq = ZBTxRequest(destAddr, msgString, sizeof(msgString));

//  Serial.println(tString);
//  Serial.println(hString);
//  Serial.println(pString);
//
//  for(int i=0; i<5; ++i) {
//    msgString[i] = tString[i];
//    msgString[i+5] = hString[i];
//    msgString[i+10] = pString[i];
//  }
//
//  msgString[4] = ',';
//  msgString[9] = ',';
  
//  strcat(msgString,tString);
//  strcat(msgString,hString);
//  strcat(msgString,pString);

 
//  byte *t = (byte *)&temp;
//  byte *h = (byte *)&humidity;
//  byte *p = (byte *)&pres;
//
//  uint8_t mmsg[ZB_PACKET_SIZE];
//
//  for (int i=0; i<4; ++i) {
//    mmsg[i] = t[i];
//    mmsg[i+4] = h[i];
//    mmsg[i+8] = p[i];
//  }

//  payload[4] = ',';
//  payload[9] = ',';

//  uint8_t tArr[4];
//
//  tArr = (uint8_t*)(&temp);
//
//  for (int i=0; i<4; ++i) {
//    Serial.write(tArr[i]);
//  }
//
//  memset(payload,0,sizeof(payload));
//  strcpy(payload,tArr);

  
//  union {
//    float val;
//    byte b[4];
//  } t;
//
//  t.val = temp;
////  h.val = humidity;
////  p.val = pres;
//  byte test_byte = 9;
//
//  payload[0] = test_byte;
//  payload[1] = t.b[1];
//  payload[2] = t.b[2];
//  payload[3] = t.b[3];


//  memset(payload,0,sizeof(char)*ZB_PACKET_SIZE);
//  
//  memcpy(payload, t.b, sizeof(t.b));
//  memcpy(payload+sizeof(t.b), h.b, sizeof(h.b));
//  memcpy(payload+sizeof(h.b), p.b, sizeof(p.b));

//  Serial.print(t.b[0]);
//
//  for (int i=0; i<4; ++i) {
//    payload[i] = t.b[i];
//    payload[i+5] = h.b[i];
//    payload[i+10] = p.b[i];
//  }
//
//  payload[4] = ',';
//  payload[9] = ',';

//  memset(payload,0,sizeof(payload));
//  strcpy(payload,msg);
//  txReq = ZBTxRequest(destAddr, payload, ZB_PACKET_SIZE);
  Serial.println("tx reg set");
}

bool MyXbee::sendMessage() {
  
  Serial.println("Sending message");
  xbee.send(txReq);

  if(xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      Serial.println("Received response correct");
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        //Successful message sent + received 
        Serial.println("Successful message delivery: ");
        return true;
      } else {
        Serial.print("Error in message delivery: ");
        Serial.println(txStatus.getDeliveryStatus());
      }
    }
  } else if (xbee.getResponse().isError()) {
    Serial.print("Error reading response packet: ");
    Serial.println(xbee.getResponse().getErrorCode());
  } else {
    Serial.println("No response packet received");
  }
  return false;
}

bool MyXbee::receiveMessage() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    //Data received
    Serial.println("Data received");
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rxData);
      Serial.println("ZB rx packet received"); 
      
      if ((rxData.getOption() == ZB_PACKET_ACKNOWLEDGED) 
          || (rxData.getOption() == ZB_BROADCAST_PACKET)) {
         //Data received + responsed correctly
        Serial.println("ZB packet acknowledged");
      } else {
        Serial.println("ZB packet not acknowledged");
      }

      checksum = rxData.getChecksum();
      Serial.print("Checksum :");
      Serial.println(checksum, HEX);

      packetLength = rxData.getPacketLength();
      Serial.print("Packet Length: ");
      Serial.println(packetLength, DEC);


      for (int i = 0; i < rxData.getDataLength(); i++) {
        rxMessageTemp[i] = rxData.getData()[i];
      }
      if (strcmp(rxMessageTemp,rxMessage)!=0) {
        memcpy ( &rxMessage, &rxMessageTemp, sizeof(rxMessageTemp));
      }
      memset(rxMessageTemp,0,sizeof(rxMessageTemp));

      return true;
      
//      for (int i = 0; i < xbee.getResponse().getFrameDataLength(); i++) {
//      nss.print("frame data [");
//      nss.print(i, DEC);
//      nss.print("] is ");
//      nss.println(xbee.getResponse().getFrameData()[i], HEX);
//      }
    }
  } else if (xbee.getResponse().isError()) {
    Serial.print("Data unavailable, error:");
    Serial.println(xbee.getResponse().getErrorCode());
  }
  else {
//    Serial.println("No data received");
  }
  return false;
}

void MyXbee::getMessage(char* data) {
  Serial.println("Attempting to get message");
  for(int i=0; i < sizeof(rxMessage); ++i) {
    data[i] = rxMessage[i];
//    Serial.println(data[i]);
  }
}

