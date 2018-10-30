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

  this->machineMode.unit = COMMAND_UNIT_G21;  // set default unit to mm
  this->machineMode.positioning = COMMAND_POSITIONING_G90; // set default absolute mode positioning
}




uint8_t Machine::parseLine(Rx_buffer_t& buffer)
{
  uint8_t returnedStatus = RETURN_SUCCES;
  uint8_t commandSymbol = 0u;
  float commandNumber = 0.0;
  int16_t integerPart = 0;


  while(buffer.data[buffer.currentParsePos] != 0)
  {
    // get command symbol and increment current parsing position
    commandSymbol = buffer.data[buffer.currentParsePos++];

    // ignore spaces
    if(commandSymbol == ' ') { continue; }

    returnedStatus = parseNumber(buffer, commandNumber);
    if( RETURN_SUCCES != returnedStatus) { return returnedStatus; }
    integerPart = (int16_t)commandNumber;

    // DEBUG
    Serial.print("3.PARSING:\n");
    Serial.print("CommandSymbol: ["); Serial.print(commandSymbol);  Serial.print("]\n");
    Serial.print("CommandNumber: ["); Serial.print(commandNumber);  Serial.print("]\n");
    Serial.print("IntegerPart: [");   Serial.print(integerPart);    Serial.print("]\n\n");

    switch(commandSymbol)
    {
      case 'G':
        currentCommand.type = COMMAND_TYPE_G;
        switch(integerPart)
        {
          case COMMAND_MOVEMENT_G00: machineMode.movement = COMMAND_MOVEMENT_G00; break;
          case COMMAND_MOVEMENT_G01: machineMode.movement = COMMAND_MOVEMENT_G01; break;
          case COMMAND_MOVEMENT_G02: machineMode.movement = COMMAND_MOVEMENT_G02; break;
          case COMMAND_MOVEMENT_G03: machineMode.movement = COMMAND_MOVEMENT_G03; break;
          case COMMAND_G28: /* TODO: implement go to home */ break;
          case COMMAND_UNIT_G20: this->machineMode.unit = COMMAND_UNIT_G20; break;
          case COMMAND_UNIT_G21: this->machineMode.unit = COMMAND_UNIT_G21; break;
          case COMMAND_POSITIONING_G90: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          case COMMAND_POSITIONING_G91: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          default:
            return ERROR_COMMAND_NOT_SUPPORTED;
        }
        break;

      case 'M':
        // TODO: implement M commands
        break;

      case '#':
        // handle special commands
        break;

      default:
        // hanndle command parameters
        switch(commandSymbol)
        {
          case 'X':
            // set to active X movement flag
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->currentCommand.new_x = commandNumber; } // absolute
            else { this->currentCommand.new_x = this->machineState.x + commandNumber; } // relative positioning
            break;

          case 'Y':
            // set to active Y movement flag
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->currentCommand.new_y = commandNumber; } // absolute
            else { this->currentCommand.new_y = this->machineState.y + commandNumber; } // relative positioning
            break;

          case 'Z': break;
            // set to active Y movement flag
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->currentCommand.new_y = commandNumber; } // absolute
            else { this->currentCommand.new_y = this->machineState.y + commandNumber; } // relative positioning
            break;

          case 'I': break;
          case 'J': break;
          case 'F': break;
          default:
            return ERROR_SYMBOL_NOT_SUPPORTED;
        } // end switch 2
    } // end switch 1
  } //end while

  // Line parsing complete
  buffer.ready = false;
  return RETURN_SUCCES;
}


uint8_t Machine::executeCommand()
{
  // execute movement command
  switch()
  return RETURN_SUCCES;
}
