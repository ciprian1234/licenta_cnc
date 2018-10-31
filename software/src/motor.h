#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

class Motor
{
  private:
    // Private variables
    const uint8_t DIR_PIN;
    const uint8_t STEP_PIN;
    const uint8_t ENDSTOP_PIN;

    // Private Functions
    Motor(); // constructor with no param not allowed
  public:
    Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin);
};

#endif
