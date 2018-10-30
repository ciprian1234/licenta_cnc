#ifndef MACHINE_H
#define MACHINE_H

#include "utilities.h"

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
  float x, y, z;  // Current position of the machine
  float f;        // Current speed of the machine (mm/minute)
  float acceleration; // Machine acceleration
}MachineState_t;


typedef struct
{
  bool x_end;   // x endpoint switch
  bool y_end;   // y endpoint switch
  bool z_end;   // z endpoint switch
}MachineEndSwitches_t;


typedef struct
{
  uint8_t movement;     // movement mode chould be G00 G01 G02 G03
  uint8_t unit;         // set unit to mm (G21) or inches G20
  uint8_t positioning;  // G90 for absolute, G91 for incremental

}MachineMode_t;


typedef struct
{
  uint8_t type;   // could either be COMMAND_TYPE_G, COMMAND_TYPE_M, or COMMAND_TYPE_SPECIAL
  uint8_t code;   // command code number
  float new_X, new_Y, new_Z; // new x y z position for 3d printer
  float new_f;    // new feed rate speed
  float i, j;  // for arc movement radius distance.
}MachineCommand_t;



class Machine
{
private:

  // Private variables
  Point_3d_t homePoint;
  MachineState_t machineState;
  MachineMode_t machineMode;
  MachineCommand_t currentCommand;
  MachineEndSwitches_t endSwitches;

  // Private Functions


public:
  Machine();
  void init();
  uint8_t parseLine(Rx_buffer_t& buffer);
  uint8_t executeCommand();

};

#endif
