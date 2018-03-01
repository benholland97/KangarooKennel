#include "MyXbee.hpp"

MyXbee::MyXbee() {

  xbee = XBee();
  mySerial = new SoftwareSerial(PIN_RX, PIN_TX);

//                  Tx variables
  uint8_t payload[] = { 'T', 'R', 'E', 'V'};
  payloadVal = "TREV";
  //Destination XBee address
  destAddr = XBeeAddress64(ADDR_LOW, ADDR_HIGH);
  //Transmission request
  txReq = ZBTxRequest(destAddr, payload, sizeof(payload));
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

void MyXbee::setPayload(String msg) {
  Serial.println("Setting Payload");
  uint8_t payload[msg.length()];
  for (int i=0; i<msg.length();++i) {
    payload[i] = msg.charAt(i);
  }
  txReq = ZBTxRequest(destAddr, payload, sizeof(payload));
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

