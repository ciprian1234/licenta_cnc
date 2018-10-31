/* By default, an integer constant is treated as an int with the attendant limitations in values.
 * To specify an integer constant with another data type, follow it with: */

/* Add 'u' or 'U' to force a constant into an unsigned data format: Example 33u */
/* Add 'l' or 'L' to force the constant into a long data format. Example: 100000L */
/* Add 'ul' or 'UL' to force the constant into an unsigned long constant. Example: 32767ul */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#define PIN_ENABLE_MOTORS 8u

#define PIN_MOTOR_X_STEP  2u
#define PIN_MOTOR_Y_STEP  3u
#define PIN_MOTOR_Z_STEP  4u

#define PIN_MOTOR_X_DIR   5u
#define PIN_MOTOR_Y_DIR   6u
#define PIN_MOTOR_Z_DIR   7u

#define PIN_ENDSTOP_X     9u
#define PIN_ENDSTOP_Y    10u
#define PIN_ENDSTOP_Z    11u

// Reserved pins (not used yet)
#define PIN_RESERVED_12   12u
#define PIN_RESERVED_13   13u
#define PIN_RESERVED_A0   A0
#define PIN_RESERVED_A1   A1
#define PIN_RESERVED_A2   A2
#define PIN_RESERVED_A3   A3
#define PIN_RESERVED_A4   A4
#define PIN_RESERVED_A5   A5

#endif
