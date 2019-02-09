#ifndef MACHINE_H
#define MACHINE_H

#include "system.h"
#include "motor.h"

#define COMMAND_TYPE_G            0u
#define COMMAND_TYPE_M            1u
#define COMMAND_TYPE_SPECIAL      2U

#define COMMAND_MOVEMENT_G00      0u // secvential linear move [DEFAULT]
#define COMMAND_MOVEMENT_G01      1u // linear move
#define COMMAND_MOVEMENT_G02      2u // arc move clockwise
#define COMMAND_MOVEMENT_G03      3U // arc move counter clockwise

#define COMMAND_DWELL_G04         4U // wait some ammount of time doing nothing

#define COMMAND_PLANE_XY_G17      17u // select xy plane [DEFAULT]
#define COMMAND_PLANE_XZ_G18      18u // select xz plane
#define COMMAND_PLANE_YZ_G19      19u // select yz plane

#define COMMAND_UNIT_G20          20u // set measurement unit in inches
#define COMMAND_UNIT_G21          21u // set measurement unit in mm

#define COMMAND_POSITIONING_G90   90u // abosolute positioning [DEFAULT]
#define COMMAND_POSITIONING_G91   91u // incremental position

#define COMMAND_HOME_G28          28u   // move to home
#define COMMAND_OFFSET_G92        92u   // set home offset


#define COMMAND_TEST_G250         250u   // used for testing
#define COMMAND_TEST_G251         251u   // used for testing
#define COMMAND_TEST_G252         252u   // used for testing
#define COMMAND_TEST_G253         253u   // used for testing
#define COMMAND_TEST_G254         254u   // used for testing
#define COMMAND_TEST_G255         255u   // used for testing



typedef struct
{
  int32_t x, y;
}Point2d_int32_t;

typedef struct
{
  int32_t x, y, z;
}Point3d_int32_t;


typedef struct
{
  float x, y;
}Point2d_float_t;

typedef struct
{
  float x, y, z;
}Point3d_float_t;





typedef struct
{
  uint8_t movement;     // movement mode chould be G00 G01 G02 G03
  uint8_t plane;        // selected plane for G02 and G03 cutting: XY Xz Yz
  uint8_t unit;         // set unit to mm (G21) or inches G20
  uint8_t positioning;  // G90 for absolute, G91 for incremental
  uint8_t waitFlag;     // wait notification flag used for G04 command
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
  uint16_t lineIndex;
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
  void clearCurrentCmd() {  memset(&this->newCmd, 0, sizeof(MachineCommand_t) );  }
  uint8_t setMotorsSpeed(uint16_t newSpeed);
  uint8_t performAxisLinearMovement_G00(Motor& inputMotor, int32_t newAxisPosition);
  uint8_t performLinearInterpolation_G01(Point3d_float_t& p1);
  uint8_t performLinearInterpolation_G01_Optimized(Point3d_int32_t& p1);
  uint8_t performCircularArcInterpolation(Motor& axis_1, Motor& axis_2, uint8_t DIR);
  uint8_t performCircularArcInterpolation_Optimized(Motor& axis_1, Motor& axis_2, uint8_t DIR);
};


#endif
