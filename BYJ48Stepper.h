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

#ifndef BYJ48_STEPPER_H
#define BYJ48_STEPPER_H

class Stepper {
public:
  enum PinGroup { NONE, MEGA2560_6_9, MEGA2560_10_13 };
  Stepper(
    int motorPin1, int motorPin2,
    int motorPin3, int motorPin4);
  Stepper(PinGroup pinGroup);
  void setSpeed(double);
  void step(int);
  const int STEPS_PER_ROTATION = 4096;
private:
  void doSteps();
  unsigned long m_lastStepTime;
  unsigned long m_stepDelay;
  int m_motorPin1, m_motorPin2, m_motorPin3, m_motorPin4;
  int m_stepIndex, m_numberOfSteps;
  PinGroup m_pinGroup;
  unsigned char* DDR_B = (unsigned char*)0x24;
  unsigned char* PORT_B = (unsigned char*)0x25;
  const unsigned char PORT_B_MASK = 0x0F;
  const unsigned char PORT_B_STEPS[8] {
    0x80, 0xC0, 0x40, 0x60,
    0x20, 0x30, 0x10, 0x90
  };
  unsigned char* DDR_H = (unsigned char*)0x101;
  unsigned char* PORT_H = (unsigned char*)0x102;
  const unsigned char PORT_H_MASK = 0x87;
  const unsigned char PORT_H_STEPS[8] {
    0x40, 0x60, 0x20, 0x30,
    0x10, 0x18, 0x08, 0x48 };
};

#endif
