#ifndef MACHINE_H
#define MACHINE_H

#include "system.h"
#include "motor.h"

#define COMMAND_TYPE_G        0U
#define COMMAND_TYPE_M        1U
#define COMMAND_TYPE_SPECIAL  2U

#define COMMAND_MOVEMENT_G00    0U // secvential linear move [DEFAULT]
#define COMMAND_MOVEMENT_G01    1U // linear move
#define COMMAND_MOVEMENT_G02    2U // arc move clockwise
#define COMMAND_MOVEMENT_G03    3U // arc move counter clockwise

#define COMMAND_UNIT_G20        20U // set measurement unit in inches
#define COMMAND_UNIT_G21        21U // set measurement unit in mm

#define COMMAND_POSITIONING_G90   90U // abosolute positioning [DEFAULT]
#define COMMAND_POSITIONING_G91   91U // incremental position

#define COMMAND_G28   28U   // move to home




typedef struct
{
  float x, y, z;
}Point_3d_t;




typedef struct
{
  uint8_t movement;     // movement mode chould be G00 G01 G02 G03
  uint8_t unit;         // set unit to mm (G21) or inches G20
  uint8_t positioning;  // G90 for absolute, G91 for incremental
}MachineMode_t;




typedef struct
{
  //uint8_t type;   // could either be COMMAND_TYPE_G, COMMAND_TYPE_M, or COMMAND_TYPE_SPECIAL
  //uint8_t code;   // command code number
  float x, y, z; // new x y z position for 3d printer
  float i, j, r;  // for arc movement radius distance.
  uint16_t f;    // new feed rate speed (mm/min)

  union {
    uint8_t all;
    struct { uint8_t x:1, y:1, z:1, i:1, j:1, r:1, f:1; };
  }flags; // movement flags

}MachineCommand_t;




class Machine
{
private:
  // private variables
  Point_3d_t homePoint;
  MachineMode_t machineMode;
  MachineCommand_t newCmd;
  Motor motor_x;
  Motor motor_y;
  Motor motor_z;

  // private Functions
public:
  // public functions
  Machine();
  void init();
  uint8_t parseLine(Rx_buffer_t& buffer);
  uint8_t executeMovementCommand();
  uint8_t setMotorsSpeed( uint16_t newSpeed);
  uint8_t performAxisLinearMovement_G00(Motor& inputMotor, float newAxisPosition);
};


#endif
