/*
 * Stepper.h - Library for controlling BYJ48 stepper motor.
 * Contains some optimizations for MEGA2560.
 * Created by Christian Scott, November 11, 2016
 * Released into the public domain
 * 
 * Created because BYJ48 uses a different sequence than
 * some other 4-pin stepper motors:
 * Step# Pin1 Pin2 Pin3 Pin4
 * 0 - 0 0 0 1
 * 1 - 0 0 1 1
 * 2 - 0 0 1 0
 * 3 - 0 1 1 0
 * 4 - 0 1 0 0
 * 5 - 1 1 0 0
 * 6 - 1 0 0 0
 * 7 - 1 0 0 1 
 */
 
 #include "Arduino.h"
#include "Stepper.h"

Stepper::Stepper(
  int t_motorPin1, int t_motorPin2,
  int t_motorPin3, int t_motorPin4) :
    m_motorPin1(t_motorPin1), m_motorPin2(t_motorPin2),
    m_motorPin3(t_motorPin3), m_motorPin4(t_motorPin4),
    m_stepIndex(0),
    m_lastStepTime(0) {
  pinMode(m_motorPin1, OUTPUT);
  pinMode(m_motorPin2, OUTPUT);
  pinMode(m_motorPin3, OUTPUT);
  pinMode(m_motorPin4, OUTPUT);
}

Stepper::Stepper(PinGroup pinGroup) :
    m_pinGroup(pinGroup),
    m_stepIndex(0),
    m_lastStepTime(0) {
  switch (m_pinGroup) {
    case PinGroup::MEGA2560_6_9:
      *DDR_H &= 0x87;
      *DDR_H |= 0x78;
      break;
    case PinGroup::MEGA2560_10_13:
      *DDR_B &= 0x0F;
      *DDR_B |= 0xF0;
      break;
  }
}

void Stepper::setSpeed(double s) {
  m_stepDelay = 60000000L / (STEPS_PER_ROTATION * s);
}

void Stepper::step(int steps) {
  int stepsLeft = abs(steps);
  int stepDelta = 1;
  if (steps < 0) stepDelta = -1;
  while (stepsLeft > 0) {
    unsigned long now = micros();
    if (now - m_lastStepTime >= m_stepDelay) {
      m_lastStepTime = now;
      m_stepIndex = m_stepIndex + stepDelta;
      if (m_stepIndex == 8) m_stepIndex = 0;
      if (m_stepIndex == -1) m_stepIndex = 7;
      doSteps();
      stepsLeft--;
    }
  }
}

void Stepper::doSteps() {
  switch (m_pinGroup) {
    case NONE:
      switch (m_stepIndex) {
        case 0:
          digitalWrite(m_motorPin1, LOW);
          digitalWrite(m_motorPin3, LOW);
          break;
        case 1:
          digitalWrite(m_motorPin3, HIGH);
          digitalWrite(m_motorPin4, HIGH);
          break;
        case 2:
          digitalWrite(m_motorPin2, LOW);
          digitalWrite(m_motorPin4, LOW);
          break;
        case 3:
          digitalWrite(m_motorPin2, HIGH);
          digitalWrite(m_motorPin3, HIGH);
          break;
        case 4:
          digitalWrite(m_motorPin1, LOW);
          digitalWrite(m_motorPin3, LOW);
          break;
        case 5:
          digitalWrite(m_motorPin1, HIGH);
          digitalWrite(m_motorPin2, HIGH);
          break;
        case 6:
          digitalWrite(m_motorPin2, LOW);
          digitalWrite(m_motorPin4, LOW);
          break;
        case 7:
          digitalWrite(m_motorPin1, HIGH);
          digitalWrite(m_motorPin4, HIGH);
          break;
      }
      break;
    case PinGroup::MEGA2560_6_9:
      *PORT_H = (*PORT_H & PORT_H_MASK) | PORT_H_STEPS[m_stepIndex];
      break;
    case PinGroup::MEGA2560_10_13:
      *PORT_B = (*PORT_B & PORT_B_MASK) | PORT_B_STEPS[m_stepIndex];
      break;
  }
}
