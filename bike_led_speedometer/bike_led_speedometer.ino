
#include "ArduinoClock.h"
#include "ArduinoDigitalPin.h"
#include "ArduinoAnalogWritablePin.h"
#include "PulseCounter.h"
#include "PulseToSpeedStatus.h"
#include "PinEffect.h"
#include "BikeLedSpeedometerUi.h"

#define ATTINY

#ifdef ATTINY
  #define PULSE_PIN 4
  #define PULSE_FEEDBACK_PIN 0
  #define DECELERATING_PIN 1
  #define WATCHDOG_PIN 3
#else //ARDUINO
  #define PULSE_PIN 2
  #define PULSE_FEEDBACK_PIN 3
  #define DECELERATING_PIN 5
#endif

//hardware
DigitalPin *pulsePin;
AnalogWritablePin *pulseFeedbackPin;
DigitalWritablePin *deceleratingPin, *watchDogPin;
Clock *clock;

//logic
PulseCounter *pulseCounter;
PulseToSpeedStatus *pulseToSpeedStatus;

//ui
BikeLedSpeedometerUi *bikeLedSpeedometerUi;

void setup() {
  //Serial.begin(9600);
  clock = new ArduinoClock();
  pulsePin = new ArduinoDigitalPin( PULSE_PIN, INPUT );
  pulseFeedbackPin = new ArduinoAnalogWritablePin( PULSE_FEEDBACK_PIN );
  deceleratingPin = new ArduinoDigitalPin( DECELERATING_PIN, OUTPUT );
  watchDogPin = new ArduinoDigitalPin( WATCHDOG_PIN, OUTPUT );
  
  pulseCounter = new PulseCounter( pulsePin );
  pulseToSpeedStatus = new PulseToSpeedStatus();
  bikeLedSpeedometerUi = new BikeLedSpeedometerUi( clock );
  
  watchDogPin->set( true );
}

void watchdog() {
  static uint32_t toggle = millis() + 1000;
  if( millis() >= toggle ) {
    watchDogPin->toggle();
    toggle = millis() + 1000;
  }
}

void loop() {
  clock->update();
  
  pulseCounter->tick( clock );
  pulseToSpeedStatus->tick( clock );
  bikeLedSpeedometerUi->tick( clock );
}

