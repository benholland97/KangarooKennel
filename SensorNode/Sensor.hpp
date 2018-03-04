#include "Configuration.hpp"

class Sensor {
public:
  Sensor();

  ~Sensor();

  float getTempFH();

  float getTempC();

  float getHumidity();

  void getData();

private:
  Weather sensor;
  int state;
  float humidity;
  float tempf;
};

