
#include "Configuration.hpp"

class MyXbee {
public:

  bool sendMessage();

  bool receiveMessage();

  void setPayload(String msg);

  void getMessage(char* data);

  MyXbee();

  ~MyXbee();

private:
  SoftwareSerial *mySerial;
  XBee xbee;
  String payloadVal;
  XBeeAddress64 destAddr;
  ZBTxRequest txReq;
  ZBTxStatusResponse txStatus;

  XBeeResponse rxResp;
  ZBRxResponse rxData;
  ModemStatusResponse msr;
  byte checksum;
  int packetLength;
  char rxMessageTemp[ZB_PACKET_SIZE];
  char rxMessage[ZB_PACKET_SIZE];
};
