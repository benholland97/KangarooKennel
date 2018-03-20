#include "Configuration.hpp"

class MyXbee {
public:

  bool sendMessage();

  bool receiveMessage();

  void setPayload(float temp, float humidity, float pres);

  void getMessage(char* data);

  MyXbee();

  ~MyXbee();

private:
  SoftwareSerial *mySerial;
  XBee xbee;
  char payload[ZB_PACKET_SIZE];
  XBeeAddress64 destAddr;
  ZBTxRequest txReq;
  ZBTxStatusResponse txStatus;

  XBeeResponse rxResp;
  ZBRxResponse rxData;
  ModemStatusResponse msr;
  byte checksum;
  int packetLength;
  char rxMessageTemp[ZB_PACKET_SIZE], rxMessage[ZB_PACKET_SIZE];
};
