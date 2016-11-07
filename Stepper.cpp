#include "Arduino.h"
#include "Stepper.h"

Stepper::Stepper(int t_numberOfSteps,
  int t_motorPin1, int t_motorPin2,
  int t_motorPin3, int t_motorPin4) :
    m_numberOfSteps(t_numberOfSteps),
    m_motorPin1(t_motorPin1), m_motorPin2(t_motorPin2),
    m_motorPin3(t_motorPin3), m_motorPin4(t_motorPin4),
    m_stepIndex(0),
    m_lastStepTime(0) {
  pinMode(m_motorPin1, OUTPUT);
  pinMode(m_motorPin2, OUTPUT);
  pinMode(m_motorPin3, OUTPUT);
  pinMode(m_motorPin4, OUTPUT);
}

Stepper::Stepper(int numberOfSteps,
  PinGroup pinGroup) :
    m_numberOfSteps(numberOfSteps),
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
  m_stepDelay = 60000000L / (m_numberOfSteps * s);
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
