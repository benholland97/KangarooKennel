#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <Wire.h>
#include <SparkFun_Si7021_Breakout_Library.h>

//-----------------------PINS------------------

//-----T&H SENSOR-----
#define PIN_PWR   A3
#define PIN_GND   A2
#define PIN_PWM1  6
#define PIN_PWM2  7

//-----PRESENCE SENSOR------
#define PIN_LED 4
#define PIN_DR 10
#define PIN_PIR 11


//-----XBEE-----
#define PIN_RX  2
#define PIN_TX  3


//-------------------SENSOR CONSANTS--------------
#define TX_DELAY_ABS   5000
#define TX_DELAY_PRES  3000



//--------------------XBEE CONSTANTS-------------
#define ZB_PACKET_SIZE 0x50

//-------------API------------
#define TX_64_REQUEST 0x0
#define TX_16_REQUEST 0x1
#define AT_COMMAND_REQUEST 0x08
#define AT_COMMAND_QUEUE_REQUEST 0x09
#define REMOTE_AT_REQUEST 0x17
#define ZB_TX_REQUEST 0x10
#define ZB_EXPLICIT_TX_REQUEST 0x11
#define RX_64_RESPONSE 0x80
#define RX_16_RESPONSE 0x81
#define RX_64_IO_RESPONSE 0x82
#define RX_16_IO_RESPONSE 0x83
#define AT_RESPONSE 0x88
#define TX_STATUS_RESPONSE 0x89
#define MODEM_STATUS_RESPONSE 0x8a
#define ZB_RX_RESPONSE 0x90
#define ZB_EXPLICIT_RX_RESPONSE 0x91
#define ZB_TX_STATUS_RESPONSE 0x8b
#define ZB_IO_SAMPLE_RESPONSE 0x92
#define ZB_IO_NODE_IDENTIFIER_RESPONSE 0x95
#define AT_COMMAND_RESPONSE 0x88
#define REMOTE_AT_COMMAND_RESPONSE 0x97

//---------TX------------
#define SUCCESS 0x0
#define CCA_FAILURE 0x2
#define INVALID_DESTINATION_ENDPOINT_SUCCESS 0x15
#define NETWORK_ACK_FAILURE 0x21
#define NOT_JOINED_TO_NETWORK 0x22
#define SELF_ADDRESSED 0x23
#define ADDRESS_NOT_FOUND 0x24
#define ROUTE_NOT_FOUND 0x25
#define PAYLOAD_TOO_LARGE 0x74

//-----------RX------------
#define ZB_PACKET_ACKNOWLEDGED 0x01
#define ZB_BROADCAST_PACKET 0x02

//---------ADDRESS-----------
#define ADDR_LOW   0x00000000
#define ADDR_HIGH  0x00000000







