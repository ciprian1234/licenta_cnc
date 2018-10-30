#include "machine.h"

Machine::Machine()
{
  this->init();
}


void Machine::init()
{
  memset(&this->homePoint, 0 , sizeof(Point_3d_t) );
  memset(&this->machineState, 0, sizeof(MachineState_t) );
  memset(&this->machineMode, 0, sizeof(MachineMode_t) );
  memset(&this->currentCommand, 0, sizeof(MachineCommand_t) );
  memset(&this->endSwitches, 0, sizeof(MachineEndSwitches_t) );

  this->machineMode.unit = COMMAND_UNIT_G21;
  this->machineMode.positioning = COMMAND_POSITIONING_G90; // set absolute mode positioning
}


void Machine::parseLine(Rx_buffer_t& buffer, uint8_t size)
{
  uint8_t commandType = 0;
  uint16_t integerPart = 0;
  uint16_t fractionalPart = 0;

  uint8_t pos = 0;
  while(buffer.data[pos] != 0)
  {

    //read real number;

    switch(buffer.data[pos])
    {
      case 'G':

        break;
      case 'M':
        // TODO
        break;
      case '#':
        // handle special commands
        break;
      default:
        switch(buffer.data[pos])
        {
          case ' ': break; // ignore spaces
          case 'X':
            break;
          case 'Y':
            break;
          case 'Z':
            break;
          case 'F':
            break;
          case 'I':
            break;
          case 'J':
            break;
          default:
            ;
        } // end switch 2
    } // end switch 1

  } //end while

}


void Machine::executeCommand()
{

}
