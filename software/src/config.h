#ifndef CONFIG_H
#define CONFIG_H


// Baud rate
#define BAUD_RATE     9600u

// Size of the buffer for recieved commands
#define RX_BUFF_SIZE  128u

// Max X axis (mm)
#define MAX_X	200u

// Max Y axis (mm)
#define MAX_Y	200u

// Max Z axis (mm)
#define MAX_Z 500u

// Maximum travel speed (mm/min)
// default speed = RPM * AXIS_TRAVEL_DISTANCE_360
#define MAX_TRAVEL_SPEED  1000u

// Maximum drawing/cutting speed (mm/min)
#define MAX_DRAWING_SPEED 400u

// Number of steps per a complete 360 rotation of stepper motor
#define NUMBER_OF_STEPS_360 200u

// Number of milimeters a axis travels when an 360 rotation is complete
#define AXIS_TRAVEL_DISTANCE_360 8u


#endif
