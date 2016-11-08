#include "BYJ48Stepper.h"

const int LED_PIN = 13;
const double RPM = 5;
const unsigned long TOGGLE_MOTOR_DELAY = 1000;
const unsigned long TOGGLE_LED_DELAY = 250;

int direction = 0; // 0 = clockwise, 1 = stop, 2 = counter-clockwise, 3 = stop
int ledValue = LOW;
unsigned long lastMotorToggle;
unsigned long lastLedToggle;

Stepper s(Stepper::PinGroup::MEGA2560_6_9);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  s.setSpeed(RPM);
  lastMotorToggle = millis();
  lastLedToggle = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastMotorToggle >= TOGGLE_MOTOR_DELAY) {
    lastMotorToggle = now;
    direction = (direction + 1) % 4;
    if (direction == 0)
      s.start(-1);
    else if (direction == 2)
      s.start(1);
    else
      s.stop();
  }
  if (now - lastLedToggle >= TOGGLE_LED_DELAY) {
    lastLedToggle = now;
    ledValue = !ledValue;
    digitalWrite(LED_PIN, ledValue);
  }
  s.step();
}