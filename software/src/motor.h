#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>


// direction of the motors
#define MOTOR_DIRECTION_REVERSE 0u    // reverse means, move motor in home direction
#define MOTOR_DIRECTION_FORWARD 1u    // forward means, move motor against home direction
#define AXIS_ENDSTOP_REACHED    0u    // return value of digitalRead() function when end stop is reached

#define ACCELERATION_DISABLED   0u
#define ACCELERATION_ENABLED    1u

// Stepping resolution between motor steps, measured in mm
#define STEP_RESOLUTION ( ((float)AXIS_TRAVEL_DISTANCE_360 / (float)NUMBER_OF_STEPS_360) )

#define CONVERT_INTO_STEPS(number) ( (int32_t)(number/STEP_RESOLUTION) )


class Motor
{
  private:
    // private variables
    const uint8_t DIR_PIN;      // direction pin of the motor
    const uint8_t STEP_PIN;     // step pin of the motor
    const uint8_t ENDSTOP_PIN;  // endstop signal pin
    int32_t AXIS_MAX_STEPS;     // maximum possible steps in forward direction relative to home point

    // used to compute logical axis position measured in mm, by default is 0 when machine starts
    // counts numbers of steps moved REVERSE OR FORWARD, this will be multiplied by STEP_RESOLUTION to obtain current position
    int32_t steps;              // motor position stored in steps relative to 0 position
    uint16_t speed;             // motor moving speed (mm/min)
    uint16_t delayBetweenSteps; // delay out a motor step transition

    // private functions
    Motor(); // constructor with no param not allowed

  public:
    // public functions
    Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin, uint16_t axisMaxPos);

    bool step(uint8_t direction);

    void waitBetweenSteps(bool accelerationEnabled);

    uint8_t setSpeed(uint16_t newSpeed);

    float getPosition();

    int32_t getPositionInSteps();

    uint8_t setPosition(float newPosition);

    void moveToHome(void);
};




#endif
