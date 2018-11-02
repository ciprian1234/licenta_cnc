#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

//direction
#define MOTOR_MOVE_REVERSE 0u
#define MOTOR_MOVE_FORWARD 1u

#define MOTOR_END 0

class Motor
{
  private:
    // Private variables
    const uint8_t DIR_PIN;
    const uint8_t STEP_PIN;
    const uint8_t ENDSTOP_PIN;

    //uint8_t direction;      // only 2 directions are possible
    uint8_t stepState;      // motor is stepping at a transition from false to true or otherwise
    uint16_t speed;         // motor moving speed (mm/min)
    bool endstop;           // if endstop is reached no further movemend in that direction is possible




    // Private functions
    Motor(); // constructor with no param not allowed
  public:
    // Private variables
    float position;         // axis position measured in mm, by default is 0 when machine starts

    // public functions
    Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin);
    uint8_t step(uint8_t dir);
    uint8_t setSpeed(uint16_t sp);
};

#endif
