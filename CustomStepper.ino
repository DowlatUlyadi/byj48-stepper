#include "Stepper.h"

const unsigned int stepCount = 64 * 64;
const unsigned int speed = 10;

Stepper sOptimal(stepCount, Stepper::PinGroup::MEGA2560_6_9);
Stepper sSlow(stepCount, 6, 7, 8, 9);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sOptimal.setSpeed(speed);
  sSlow.setSpeed(speed);
}

void loop() {
  sSlow.step(stepCount);
  delay(500);
  sSlow.step(-stepCount);
  delay(500);
  sOptimal.step(stepCount);
  delay(500);
  sOptimal.step(-stepCount);
  delay(500);
}
