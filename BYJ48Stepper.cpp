/*
	BYJ48Stepper.h

	Library for controlling BYJ48 stepper motor or any 8 phase stepper that
  follows the sequence:
  Step# - Pin1 Pin2 Pin3 Pin4
  0 - 0 0 0 1
  1 - 0 0 1 1
  2 - 0 0 1 0
  3 - 0 1 1 0
  4 - 0 1 0 0
  5 - 1 1 0 0
  6 - 1 0 0 0
  7 - 1 0 0 1 
  
  You can use any 4 digital pins using the 4 parameter constructor. Connect
  input1 to the first parameter, input2 to the second parameter etc.
  
  Also contains some optimizations for ATMEGA2560. If you construct Stepper
  with PinGroup::MEGA2560_6_9 or PinGroup::MEGA2560_10_13 and connect using
  pins 6-9 or 10-13, it will write directly to the pins without using
  digitalWrite().
  
  The step(int steps) method blocks any other code in your loop while it steps
  "steps" times. The step() method without parameters steps once according to
  the direction given to the start(int direction) method. A positive
  "direction" steps once clockwise for any negative number and once counter-
  clockwise for any positive value. A "direction" of 0 or calling the stop()
  method prevents step() from doing anything.
  
  Both methods respect the current speed which can be set using
  setSpeed(double RPM).
  
  See Examples for usage tips.
    
	Created 07.11.16
	By Christian Scott
	Modified 08.11.16
	By Christian Scott
*/
 
#include "Arduino.h"
#include "BYJ48Stepper.h"

Stepper::Stepper(
  int motorPin1, int motorPin2,
  int motorPin3, int motorPin4,
  PinGroup pinGroup) :
    m_motorPin1(motorPin1), m_motorPin2(motorPin2),
    m_motorPin3(motorPin3), m_motorPin4(motorPin4),
    m_pinGroup(pinGroup),
    m_lastStepTime(micros()), m_stepIndex(0) {
	  setSpeed(1);
}

Stepper::Stepper(
  int motorPin1, int motorPin2,
  int motorPin3, int motorPin4) :
    Stepper(motorPin1, motorPin2, motorPin3, motorPin4, PinGroup::NONE) {
  pinMode(m_motorPin1, OUTPUT);
  pinMode(m_motorPin2, OUTPUT);
  pinMode(m_motorPin3, OUTPUT);
  pinMode(m_motorPin4, OUTPUT);
}

Stepper::Stepper(PinGroup pinGroup) :
    Stepper(0, 0, 0, 0, pinGroup) {
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

void Stepper::moveNextStep(int direction) {
  m_stepIndex += direction;
  if (m_stepIndex == 8) m_stepIndex = 0;
	if (m_stepIndex == -1) m_stepIndex = 7;
  moveMotor();
}

void Stepper::step(int steps) {
  int stepsLeft = abs(steps);
  int direction = (steps < 0) ? -1 : 1;
  while (stepsLeft > 0) {
    unsigned long now = micros();
    if (now - m_lastStepTime >= m_stepDelay) {
      m_lastStepTime = now;
      moveNextStep(direction);
      stepsLeft--;
    }
  }
}

void Stepper::step() {
  if (m_runningStep == 0) return;
	unsigned long now = micros();
	if (now - m_lastStepTime >= m_stepDelay) {
		m_lastStepTime = now;
    moveNextStep(m_runningStep);
	}
}

void Stepper::start(int direction) {
  m_runningStep = (direction < 0) ? -1 : 1;
}

void Stepper::stop() {
  m_runningStep = 0;
}

void Stepper::moveMotor() {
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
