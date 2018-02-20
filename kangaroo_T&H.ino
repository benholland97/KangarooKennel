

/******************************************************************************
  SparkFun Si7021 Breakout Example 
  Joel Bartlett @ SparkFun Electronics
  Original Creation Date: May 18, 2015
  Updated May 4, 2016
  This sketch prints the temperature and humidity the Serial port.

  The library used in this example can be found here:
  https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries

  Hardware Connections:
      HTU21D ------------- Photon
      (-) ------------------- GND
      (+) ------------------- 3.3V (VCC)
       CL ------------------- D1/SCL
       DA ------------------- D0/SDA

  Development environment specifics:
  	IDE: Particle Dev
  	Hardware Platform: SparkFun RedBoard
                       Arduino IDE 1.6.5

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*******************************************************************************/
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

float humidity = 0;
float tempf = 0;


int power = A3;
int GND = A2;
int pwm1 = 6;
int pwm2 = 7;
int state = LOW;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;

//---------------------------------------------------------------
void setup()
{
    Serial.begin(9600);   // open serial over USB at 9600 baud

    pinMode(power, OUTPUT);
    pinMode(GND, OUTPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(pwm2, OUTPUT);

    digitalWrite(power, HIGH);
    digitalWrite(GND, LOW);
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, LOW);
    
          sensor.begin();

          delay(1000);

    //Initialize the I2C sensors and ping them

}
//---------------------------------------------------------------
void loop()
{
    //Get readings from all sensors
    getWeather();
    printInfo();

      if (state == LOW)
  {
    digitalWrite(pwm1, LOW);
    digitalWrite(pwm2, HIGH);
    state = HIGH;
    Serial.println("temp 1");
  }
  else
  {
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, LOW);
    state = LOW;
    Serial.println("temp 2");
  }
delay(1000);


}
//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
//---------------------------------------------------------------
void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("F, ");
  Serial.print((tempf-32)*.5569);   //conversion to celsius
  Serial.print("C, ");
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
}
