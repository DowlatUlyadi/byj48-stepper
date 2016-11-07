#include "Stepper.h"

const unsigned int stepCount = 64 * 64;
const double speed = 0.2;

Stepper sOptimal(stepCount, Stepper::PinGroup::MEGA2560_10_13);
Stepper sSlow(stepCount, 10, 11, 12, 13);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sOptimal.setSpeed(speed);
  sSlow.setSpeed(speed);
}

void loop() {
  sSlow.step(stepCount >> 2);
  delay(500);
  sSlow.step(-(stepCount >> 2));
  delay(500);
  sOptimal.step(stepCount >> 2);
  delay(500);
  sOptimal.step(-(stepCount >> 2));
  delay(500);
}
