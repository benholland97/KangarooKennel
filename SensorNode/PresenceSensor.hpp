#include "Configuration.hpp"

class PresSensor {
public:

  void start();

  int getPresence();

  unsigned long getTimeDiff(unsigned long _timeStart);

  PresSensor();

  ~PresSensor();

private:
  // TIMING VARIABLES
  int sensorInterval_absent;      // time intervals between sensor readings
  int sensorInterval_present;     // for presence and absence of dog (seconds)
  int pirTimeLimit;   // time limit for PIR sensor (seconds)
  
  int sensorInterval;  // this will be set to one of the above based on getPresence()'s return value

  // MISC
  int inputCheck, inputLoop;
  unsigned long timeStart;

};
