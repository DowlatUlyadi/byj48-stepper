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

#ifndef BYJ48_STEPPER_H
#define BYJ48_STEPPER_H

class Stepper {
public:
  enum PinGroup { NONE, MEGA2560_6_9, MEGA2560_10_13 };
  Stepper(
    int motorPin1, int motorPin2,
    int motorPin3, int motorPin4,
    PinGroup pinGroup);
  Stepper(
    int motorPin1, int motorPin2,
    int motorPin3, int motorPin4);
  Stepper(PinGroup pinGroup);
  void setSpeed(double);
  void step(int);
  void step();
  void start(int direction);
  void stop();
  const int STEPS_PER_ROTATION = 4096;
private:
  void moveMotor();
  void moveNextStep(int direction);
  unsigned long m_lastStepTime;
  unsigned long m_stepDelay;
  int m_motorPin1, m_motorPin2, m_motorPin3, m_motorPin4;
  int m_stepIndex;
  int m_runningStep;
  PinGroup m_pinGroup;
  unsigned char* DDR_B = (unsigned char*)0x24;
  unsigned char* PORT_B = (unsigned char*)0x25;
  unsigned char* DDR_H = (unsigned char*)0x101;
  unsigned char* PORT_H = (unsigned char*)0x102;
  const unsigned char PORT_B_MASK = 0x0F;
  const unsigned char PORT_B_STEPS[8] {
    0x80, 0xC0, 0x40, 0x60,
    0x20, 0x30, 0x10, 0x90
  };
  const unsigned char PORT_H_MASK = 0x87;
  const unsigned char PORT_H_STEPS[8] {
    0x40, 0x60, 0x20, 0x30,
    0x10, 0x18, 0x08, 0x48 };
};

#endif
