#include "BYJ48Stepper.h"

// 4096 steps per rotation on the BYJ
const unsigned int stepCount = 4096;
const double speed = 5;

// Use pins 6-9 or 10-13 on the MEGA2560 for a slightly optimized version
Stepper sOptimal1(Stepper::PinGroup::MEGA2560_10_13);
Stepper sOptimal2(Stepper::PinGroup::MEGA2560_6_9);
// Otherwise use this generic version which isn't exactly a slouch either
Stepper sSlow(10, 11, 12, 13);

void setup() {
  // Speed is in RPM
  sOptimal1.setSpeed(speed);
  sSlow.setSpeed(speed);
}

void loop() {
  // Positive step counts go counter-clockwise
  sOptimal1.step(stepCount);
  // Negative step counts go clockwise
  delay(500);
  sOptimal1.step(-stepCount);
  delay(500);
  sSlow.step(stepCount);
  delay(500);
  sSlow.step(-stepCount);
  delay(500);
}
