#include "motor.h"
#include "system.h"

Motor::Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin):
  DIR_PIN(dirPin), STEP_PIN(stepPin), ENDSTOP_PIN(endstopPin)
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);


  stepState = 0u;
  position = 0;
  endstop = false;

  digitalWrite(DIR_PIN, 0u);
  digitalWrite(STEP_PIN, stepState);
  endstop = digitalRead(ENDSTOP_PIN);
}


uint8_t Motor::step(uint8_t dir)
{
  if( MOTOR_END == digitalRead(this->ENDSTOP_PIN) )
  {
    Serial.println("cnc>>>warning: reached to the end!");
    return RETURN_SUCCES;
  }

  // change direction
  digitalWrite(this->DIR_PIN, dir);

  // perform step
  this->stepState = !this->stepState;
  digitalWrite(this->STEP_PIN, this->stepState);

  // update position
  if(dir == MOTOR_MOVE_REVERSE) {this->position -= STEP_RESOLUTION;}
  else {this->position += STEP_RESOLUTION;}
  return RETURN_SUCCES;
}
//motor home procedure: move motor while endstop is true, then move in reverse direction while endstop is false


uint8_t Motor::setSpeed(uint16_t sp)
{
  // INPUT:
  // AXIS_TRAVEL_DISTANCE_360 = 8mm
  // SPEED (mm/min)
  // SPEED_s  = SPEED / 60 (mm/s)

  // SPEED = RPM * AXIS_TRAVEL_DISTANCE_360
  // RPM = SPEED / AXIS_TRAVEL_DISTANCE_360
  // RPS = RPM / 60


  // How many steps are required to travel at specified RPM?
  // STEPS_PER_MINUTE = RPM * NUMBER_OF_STEPS_360

  // How many steps are required to travel at specified RPM?
  // STEPS_PER_SECOND = STEPS_PER_MINUTE / 60

  // What delay between steps is required to travel at specified
  // delay = 1000 ms / STEPS_PER_SECOND

  // NUMBER_OF_STEPS_360 = 200
  // STEP_RESOLUTION = AXIS_TRAVEL_DISTANCE_360 / NUMBER_OF_STEPS_360
  // STEPS_PER_MINUTE = SPEED / STEP_RESOLUTION
  // STEPS_PER_SECOND =  (SPEED / STEP_RESOLUTION) / 60
  // delayMicroseconds = 1.000.000 / STEPS_PER_SECOND
}
