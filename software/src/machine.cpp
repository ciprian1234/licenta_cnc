#include "machine.h"
#include "platform_config.h"

Machine::Machine():
  motor_x(PIN_MOTOR_X_DIR, PIN_MOTOR_X_STEP, PIN_ENDSTOP_X),
  motor_y(PIN_MOTOR_Y_DIR, PIN_MOTOR_Y_STEP, PIN_ENDSTOP_Y),
  motor_z(PIN_MOTOR_Z_DIR, PIN_MOTOR_Z_STEP, PIN_ENDSTOP_Z)
{
  this->init();
}


void Machine::init()
{
  memset(&this->homePoint, 0 , sizeof(Point_3d_t) );
  memset(&this->machineState, 0, sizeof(MachineState_t) );
  memset(&this->machineMode, 0, sizeof(MachineMode_t) );
  memset(&this->currentCommand, 0, sizeof(MachineCommand_t) );

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

    // get command number
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
        // handle G type commands
        switch(integerPart)
        {
          case COMMAND_MOVEMENT_G00:    machineMode.movement = COMMAND_MOVEMENT_G00; break;
          case COMMAND_MOVEMENT_G01:    machineMode.movement = COMMAND_MOVEMENT_G01; break;
          case COMMAND_MOVEMENT_G02:    machineMode.movement = COMMAND_MOVEMENT_G02; break;
          case COMMAND_MOVEMENT_G03:    machineMode.movement = COMMAND_MOVEMENT_G03; break;
          case COMMAND_G28:             /* TODO: implement go to home */ break;
          case COMMAND_UNIT_G20:        this->machineMode.unit = COMMAND_UNIT_G20; break;
          case COMMAND_UNIT_G21:        this->machineMode.unit = COMMAND_UNIT_G21; break;
          case COMMAND_POSITIONING_G90: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          case COMMAND_POSITIONING_G91: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          default: return ERROR_COMMAND_NOT_SUPPORTED;
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
          case 'X': this->currentCommand.moveFlags.x = true;  this->currentCommand.new_x = commandNumber; break;
          case 'Y': this->currentCommand.moveFlags.y = true;  this->currentCommand.new_y = commandNumber; break;
          case 'Z': this->currentCommand.moveFlags.z = true;  this->currentCommand.new_z = commandNumber; break;
          case 'I': this->currentCommand.moveFlags.i = true;  this->currentCommand.new_i = commandNumber; break;;
          case 'J': this->currentCommand.moveFlags.j = true;  this->currentCommand.new_j = commandNumber; break;;
          case 'F': this->currentCommand.moveFlags.f = true;  this->currentCommand.new_f = commandNumber; break;;
          default: return ERROR_SYMBOL_NOT_SUPPORTED;
        } // end switch 2
    } // end switch 1
  } //end while

  // Line parsing complete
  buffer.ready = false;
  return RETURN_SUCCES;
}


uint8_t Machine::executeMovementCommand()
{

  if(!this->currentCommand.moveFlags.all) { return RETURN_SUCCES; } // no movement requested

  if(true == this->currentCommand.moveFlags.x) {}

  switch(this->machineMode.movement)
  {
    case COMMAND_MOVEMENT_G00:
      //move first z then x then z
      if( true == this->currentCommand.moveFlags.x)
      {
        while(motor_x.position < currentCommand.new_x)
        {
          motor_x.step(MOTOR_MOVE_FORWARD);
          // motor.setSpeed();
          // motor.getStepDelay();
          // delayMicroseconds(unsigned int us)
        }

      }
      break;
    case COMMAND_MOVEMENT_G01:
      break;
    case COMMAND_MOVEMENT_G02:
      break;
    case COMMAND_MOVEMENT_G03:
      break;
    default:
      return ERROR_UNEXPECTED;
  }

  // reset current command data
  memset(&this->currentCommand, 0, sizeof(MachineCommand_t) );
  return RETURN_SUCCES;
}
