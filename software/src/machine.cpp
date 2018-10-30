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
        // Handle G type commands
        switch(integerPart)
        {
          case COMMAND_MOVEMENT_G00: break;
          case COMMAND_MOVEMENT_G01: break;
          case COMMAND_MOVEMENT_G02: break;
          case COMMAND_MOVEMENT_G03: break;
          case COMMAND_G28: break;
          case COMMAND_UNIT_G20: break;
          case COMMAND_UNIT_G21: break;
          case COMMAND_POSITIONING_G90: break;
          case COMMAND_POSITIONING_G91: break;
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
          case 'X': break;
          case 'Y': break;
          case 'Z': break;
          case 'F': break;
          case 'I': break;
          case 'J': break;
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
  return RETURN_SUCCES;
}
