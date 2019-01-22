#ifndef CONFIG_H
#define CONFIG_H

// Baud rate
#define BAUD_RATE 115200u


// Size of the buffer for recieved commands
#define RX_BUFF_SIZE 128u


// Max X axis (mm)
#define AXIS_MAX_POSITION_X 200u


// Max Y axis (mm)
#define AXIS_MAX_POSITION_Y 200u


// Max Z axis (mm)
#define AXIS_MAX_POSITION_Z  50u


// Number of steps per a complete 360 rotation of stepper motor
#define NUMBER_OF_STEPS_360 800u


// Number of milimeters a axis travels when an 360 rotation is complete
#define AXIS_TRAVEL_DISTANCE_360 8u


// SPEED = RPM * AXIS_TRAVEL_DISTANCE_360
#define MIN_MOTOR_RPM     (1u)    // minimum motor rotations per minute
#define MAX_MOTOR_RPM   (255u)    // maximum motor rotations per minute

#endif
