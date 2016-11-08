#include "BYJ48Stepper.h"

// 4096 steps per rotation on the BYJ
const unsigned int stepCount = 4096;
const double speed = 5;

Stepper sOptimal1(Stepper::PinGroup::MEGA2560_10_13);
Stepper sOptimal2(Stepper::PinGroup::MEGA2560_6_9);
Stepper sSlow(10, 11, 12, 13);

void setup() {
  sOptimal1.setSpeed(speed);
  sSlow.setSpeed(speed);
}

void loop() {
  sOptimal1.step(stepCount);
  delay(500);
  sOptimal1.step(-stepCount);
  delay(500);
  sSlow.step(stepCount);
  delay(500);
  sSlow.step(-stepCount);
  delay(500);
}
