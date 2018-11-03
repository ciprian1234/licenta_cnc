#include "machine.h"
#include "platform_config.h"


Machine::Machine():
  motor_x(PIN_MOTOR_X_DIR, PIN_MOTOR_X_STEP, PIN_ENDSTOP_X, AXIS_MAX_POSITION_X),
  motor_y(PIN_MOTOR_Y_DIR, PIN_MOTOR_Y_STEP, PIN_ENDSTOP_Y, AXIS_MAX_POSITION_Y),
  motor_z(PIN_MOTOR_Z_DIR, PIN_MOTOR_Z_STEP, PIN_ENDSTOP_Z, AXIS_MAX_POSITION_Z)
{
  this->init();
}




void Machine::init()
{
  memset(&this->machineMode, 0, sizeof(MachineMode_t) );
  memset(&this->newCmd, 0, sizeof(MachineCommand_t) );

  this->machineMode.unit = COMMAND_UNIT_G21;  // set default unit to mm
  this->machineMode.positioning = COMMAND_POSITIONING_G90; // set default absolute mode positioning

  // set axis logical position, maybe load from EEPROM position before previous reset
  this->motor_x.setPosition(0);
  this->motor_y.setPosition(0);
  this->motor_z.setPosition(0);
  pinMode(PIN_ENABLE_MOTORS, OUTPUT);
  digitalWrite(PIN_ENABLE_MOTORS, LOW);
  // TODO: set drivers in sleep mode - set sleep pin to LOW
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
    Serial.print("[PARSER: ");
    Serial.print("Symbol: "); Serial.print(commandSymbol);  Serial.print("  ");
    Serial.print("Number: "); Serial.print(commandNumber);  Serial.print("]\n");
    //Serial.print("CommandNumber: ["); Serial.print(commandNumber);  Serial.print("]\n");

    switch(commandSymbol)
    {
      case 'G':
        // handle G type commands
        switch(integerPart)
        {
          case COMMAND_MOVEMENT_G00:    this->machineMode.movement = COMMAND_MOVEMENT_G00; break;
          case COMMAND_MOVEMENT_G01:    this->machineMode.movement = COMMAND_MOVEMENT_G01; break;
          case COMMAND_MOVEMENT_G02:    this->machineMode.movement = COMMAND_MOVEMENT_G02; break;
          case COMMAND_MOVEMENT_G03:    this->machineMode.movement = COMMAND_MOVEMENT_G03; break;
          case COMMAND_HOME_G28:        motor_z.moveToHome(); motor_x.moveToHome(); motor_y.moveToHome(); break;
          case COMMAND_UNIT_G20:        this->machineMode.unit = COMMAND_UNIT_G20; break;
          case COMMAND_UNIT_G21:        this->machineMode.unit = COMMAND_UNIT_G21; break;
          case COMMAND_POSITIONING_G90: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          case COMMAND_POSITIONING_G91: this->machineMode.positioning = COMMAND_POSITIONING_G91; break;
          case COMMAND_OFFSET_G92:      motor_x.setPosition(0.0); motor_y.setPosition(0.0); motor_z.setPosition(0.0); break;
          default: return ERROR_COMMAND_NOT_SUPPORTED;
        }
        break;

      case 'M':
      switch(integerPart)
      {
        case 17: digitalWrite(PIN_ENABLE_MOTORS, LOW);  break;
        case 18: digitalWrite(PIN_ENABLE_MOTORS, HIGH); break;
        default: return ERROR_COMMAND_NOT_SUPPORTED;
      }
        break;

      case '#':
        // handle special commands
        break;

      default:
        // hanndle command parameters
        switch(commandSymbol)
        {
          case 'X':
            this->newCmd.flags.x = true;
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->newCmd.x = commandNumber; } // absolute positioning
            else { this->newCmd.x = commandNumber + motor_x.getPosition(); } // relative positioning
            break;

          case 'Y':
            this->newCmd.flags.y = true;
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->newCmd.y = commandNumber; } // absolute positioning
            else { this->newCmd.y = commandNumber + motor_y.getPosition(); } // relative positioning
            break;

          case 'Z':
            this->newCmd.flags.z = true;
            if(this->machineMode.positioning == COMMAND_POSITIONING_G90) { this->newCmd.z = commandNumber; } // absolute positioning
            else { this->newCmd.z = commandNumber + motor_z.getPosition(); } // relative positioning
            break;

          case 'I': this->newCmd.flags.i = true;  this->newCmd.i = commandNumber; break;
          case 'J': this->newCmd.flags.j = true;  this->newCmd.j = commandNumber; break;
          case 'R': this->newCmd.flags.r = true;  this->newCmd.r = commandNumber; break;
          case 'F': this->newCmd.flags.f = true;  this->newCmd.f = commandNumber; break;
          case 'S': break;
          case 'T': break;
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

  if(!this->newCmd.flags.all) { return RETURN_SUCCES; } // no movement requested

  // set motors speed
  if(this->newCmd.flags.f) { this->setMotorsSpeed(this->newCmd.f);  }

  switch(this->machineMode.movement)
  {
    case COMMAND_MOVEMENT_G00:
      if(this->newCmd.flags.x) { performAxisLinearMovement_G00(motor_x, this->newCmd.x); }
      if(this->newCmd.flags.y) { performAxisLinearMovement_G00(motor_y, this->newCmd.y); }
      if(this->newCmd.flags.z) { performAxisLinearMovement_G00(motor_z, this->newCmd.z); }
      Serial.print("{X: "); Serial.print(motor_x.getPosition()); Serial.print(", ");
      Serial.print("Y: "); Serial.print(motor_y.getPosition()); Serial.print(", ");
      Serial.print("Z: "); Serial.print(motor_z.getPosition()); Serial.print("}\n");
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
  memset(&this->newCmd, 0, sizeof(MachineCommand_t) );
  return RETURN_SUCCES;
}




uint8_t Machine::setMotorsSpeed( uint16_t newSpeed)
{
  motor_x.setSpeed(newSpeed);
  motor_y.setSpeed(newSpeed);
  motor_z.setSpeed(newSpeed);
  return RETURN_SUCCES;
}




uint8_t Machine::performAxisLinearMovement_G00(Motor& inputMotor, float newAxisPosition)
{
  // TODO: implement relative positioning and ends checking
  uint16_t steps = 0;
  if(inputMotor.getPosition() < newAxisPosition)  // MOVE FORWARD
  {
    while(inputMotor.getPosition() < newAxisPosition) { if(!inputMotor.step(MOTOR_DIRECTION_FORWARD)) {break;} steps++;}
  }
  else if(inputMotor.getPosition() > newAxisPosition)  // MOVE REVERSE
  {
    while(inputMotor.getPosition() > newAxisPosition) { if(!inputMotor.step(MOTOR_DIRECTION_REVERSE)) {break;} steps++;}
  }
  Serial.print("[steps: "); Serial.print(steps); Serial.print("]\n");
  return RETURN_SUCCES;
}
