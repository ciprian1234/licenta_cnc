#include "motor.h"


Motor::Motor(uint8_t dirPin, uint8_t stepPin, uint8_t endstopPin):
  DIR_PIN(dirPin), STEP_PIN(stepPin), ENDSTOP_PIN(endstopPin)
{
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  // TODO: pinMode(ENDSTOP_PIN, INPUT_PULLUP);

  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
}
