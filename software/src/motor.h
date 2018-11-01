#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

//cagegories of directions
#define MOTOR_MOVE_DOWN     0u
#define MOTOR_MOVE_UP       1u

#define MOTOR_MOVE_LEFT     0u
#define MOTOR_MOVE_RIGHT    1u

#define MOTOR_MOVE_FORWARD  0u
#define MOTOR_MOVE BACKWARD 1u

class Motor
{
  private:
    // Private variables
    const uint8_t DIR_PIN;
    const uint8_t STEP_PIN;
    const uint8_t ENDSTOP_PIN;

    uint8_t direction;      // only 2 directions are possible
    uint8_t stepState;      // motor is stepping at a transition from false to true or otherwise
    float position;         // axis position measured in mm, by default is 0 when machine starts
    bool endstop;           // if endstop is reached no further movemend in that direction is possible



    // Private Functions
    Motor(); // constructor with no param not allowed
  public:
    Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin);
};

#endif
