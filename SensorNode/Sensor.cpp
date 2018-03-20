#include "Sensor.hpp"

Sensor::Sensor() {
  float humidity = 0;
  float tempf = 0;
  state = LOW;
    
  pinMode(PIN_PWR, OUTPUT);
  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_PWM1, OUTPUT);
  pinMode(PIN_PWM2, OUTPUT);

  digitalWrite(PIN_PWR, HIGH);
  digitalWrite(PIN_GND, LOW);
  digitalWrite(PIN_PWM1, HIGH);
  digitalWrite(PIN_PWM2, LOW);

  sensor.begin();
}

void Sensor::getData() {
  humidity = sensor.getRH();
  tempf = sensor.getTempF();
  
  if (state == LOW) {
    digitalWrite(PIN_PWM1, LOW);
    digitalWrite(PIN_PWM2, HIGH);
    state = HIGH;
  } else {
    digitalWrite(PIN_PWM1, HIGH);
    digitalWrite(PIN_PWM2, LOW);
    state = LOW;
  }
}

float Sensor::getTempFH() {
  return tempf;
}

float Sensor::getHumidity() {
  return humidity;
}

float Sensor::getTempC() {
  return ((tempf-32)*.5569);
}

