#define PIN_DR 10
#define PIN_PIR 11
#define PIN_LED 7

const int txDelayAbs = 5;    
const int txDelayPres = 1; 
int txDelay; 

int isMotionDetected() {
  return digitalRead(11);
}

int isDoorOpen() {
  return digitalRead(10);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_PIR, LOW);
}

void loop() {

  while(!isDoorOpen()) {

    //Door closed

    //if motion detected set delay to present if not set delay to absent
    txDelay = isMotionDetected()? txDelayPres : txDelayAbs;

    //Send data 
    digitalWrite(PIN_LED, HIGH);
    delay(txDelay * 500);      
    digitalWrite(PIN_LED, LOW);
    delay(txDelay * 500);     
    
  }

  //Door open
}



