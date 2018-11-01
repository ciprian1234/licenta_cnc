#include "motor.h"


Motor::Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin):
  DIR_PIN(dirPin), STEP_PIN(stepPin), ENDSTOP_PIN(endstopPin)
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);


  direction = 0u;
  stepState = 0u;
  position = 0;
  endstop = false;

  digitalWrite(DIR_PIN, direction);
  digitalWrite(STEP_PIN, stepState);
  endstop = digitalRead(ENDSTOP_PIN);
}



//motor home procedure: move motor while endstop is true, then move in reverse direction while endstop is false
