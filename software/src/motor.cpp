#include "motor.h"
#include "system.h"
#include "config.h"

#define ONE_SENOND_IN_MICROSECONDS  (1000000u)

#define AXIS_MIN_SPEED  (MIN_MOTOR_RPM * AXIS_TRAVEL_DISTANCE_360)
#define AXIS_MIN_SPEED  (MAX_MOTOR_RPM * AXIS_TRAVEL_DISTANCE_360)

// Stepping resolution between motor steps, measured in mm
#define STEP_RESOLUTION (((float)AXIS_TRAVEL_DISTANCE_360 / (float)NUMBER_OF_STEPS_360))


// Utility function used to set a delay in microseconds
static void delay_sync(uint32_t us);





Motor::Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin, uint16_t axisMaxPos):
  DIR_PIN(dirPin), STEP_PIN(stepPin), ENDSTOP_PIN(endstopPin)
{
  // setup motors pins
  pinMode(this->DIR_PIN, OUTPUT);
  pinMode(this->STEP_PIN, OUTPUT);
  pinMode(this->ENDSTOP_PIN, INPUT_PULLUP);

  this->AXIS_MAX_POSITION = axisMaxPos;
  this->stepState = 0u;
  this->position = 0;

  digitalWrite(DIR_PIN, 0u);
  digitalWrite(STEP_PIN, stepState);
}




void Motor::step(uint8_t direction)
{
  bool endstop = digitalRead(this->ENDSTOP_PIN);
  if((MOTOR_DIRECTION_REVERSE == direction) && (AXIS_ENDSTOP_REACHED == endstop) )
  {
    Serial.println("cnc>>>warning: axis reached reverse ending!"); return;
  }
  if((MOTOR_DIRECTION_FORWARD == direction) && (this->position >= AXIS_MAX_POSITION)
  {
    Serial.println("cnc>>>warning: axis reached forward ending!"); return;
  }

  // Continue if limits were not reached

  // change direction
  digitalWrite(this->DIR_PIN, dir);

  // perform step
  this->stepState = !this->stepState;
  digitalWrite(this->STEP_PIN, this->stepState);

  // update position
  if ( direction == MOTOR_DIRECTION_REVERSE)    { this->position -= STEP_RESOLUTION; }
  else if(direction == MOTOR_DIRECTION_FORWARD) { this->position += STEP_RESOLUTION; }
  else { /* this is reached only if direction is invalid */ }
}




// wait some ammount of time between steps to simulate axis moving speed
// input: speed (mm/min)        NUMBER_OF_STEPS_360       AXIS_TRAVEL_DISTANCE_360 = 8mm
// output: wait computed delay between steps
void Motor::waitBetweenSteps(bool accelerationEnabled)
{
  // acceleration: increment motor RPM every N steps
  // acceleration factor - how much RPM is incremented
  // acceleration_interval - how frequent RPM is incremented


  // SPEED = RPM * AXIS_TRAVEL_DISTANCE_360
  // RPM = SPEED / AXIS_TRAVEL_DISTANCE_360     RPS = RPM / 60
  float RPS = (this->speed / AXIS_TRAVEL_DISTANCE_360) / 60;  // rotations per second

  // How many steps are required to travel at specified RPM?
  // STEPS_PER_MINUTE = RPM * NUMBER_OF_STEPS_360
  uin16_t stepsPerSecond = RPS * NUMBER_OF_STEPS_360

  // What delay between steps is required to travel at specified
  uint16_t speedDelay = (uint16_t)(ONE_SENOND_IN_MICROSECONDS / stepsPerSecond)

  // STEP_RESOLUTION = AXIS_TRAVEL_DISTANCE_360 / NUMBER_OF_STEPS_360
  // STEPS_PER_MINUTE = SPEED / STEP_RESOLUTION
  // STEPS_PER_SECOND =  (SPEED / STEP_RESOLUTION) / 60
  // delayMicroseconds = 1.000.000 / STEPS_PER_SECOND


  // 300 000us // at 8mm/min,  200 microsteping (0.04mm)
  // 150 000us // at 8mm/min,  200 microsteping (0.02mm)
  //  75 000us // at 8mm/min,  200 microsteping (0.01mm)
  //  37 500us // at 8mm/min,  200 microsteping (0.005mm)
  //  18 750us // at 8mm/min,  200 microsteping (0.0025mm)

  // 1176us // at 2040mm/min,  200 microsteping (0.04mm)
  //  588us // at 2040mm/min,  400 microsteping (0.02mm)
  //  294us // at 2040mm/min,  800 microsteping (0.01mm)
  //  147us // at 2040mm/min, 1600 microsteping (0.005mm)
  //   73us // at 2040mm/min, 3200 microsteping (0.0025mm)
  delay_sync(speedDelay);
}




uint8_t Motor::setSpeed(uint16_t newSpeed)
{
  if( (newSpeed < AXIS_MIN_SPEED) || newSpeed > AXIS_MAX_SPEED )
  {
    return ERROR_INVALID_SPEED;
  }

}





float Motor::getPosition()
{
  return this->position;
}




// set Logical position to axis
uint8_t Motor::setPosition(float newPosition)
{
  if(newPosition > this->AXIS_MAX_POSITION) { return ERROR_AXIS_MAX_POSITION_EXCEDED; }
  this->position = newPosition;
}



// move axis to home position
void Motor::moveToHome(void)
{
  // 1. move motor in reverse while endstop is not reached,
  while( AXIS_ENDSTOP_REACHED != digitalRead(this->ENDSTOP_PIN) )
  {
    this->step(MOTOR_DIRECTION_REVERSE);
    this->waitBetweenSteps(ACCELERATION_ENABLED);
  }

  // 2. then move motor forward while endstop is pressed
  while( AXIS_ENDSTOP_REACHED == digitalRead(this->ENDSTOP_PIN) )
  {
    this->step(MOTOR_DIRECTION_FORWARD);
    this->waitBetweenSteps(ACCELERATION_ENABLED);
  }

  this->setPosition(0.0);
}




void delay_sync(int32_t us)
{
  do {
    if(us < 2) { return; }  // delayMicroseconds - cant handle delays lower then 2us
    if(us < 1000) { delayMicroseconds((uint16_t)us); return; }
    delayMicroseconds(1000);
    us -= 1000;
  }while(true);
}
