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

  this->machineMode.plane = COMMAND_PLANE_XY_G17;           // selected plane for G02 and G03 cutting, XY plane by default
  this->machineMode.unit = COMMAND_UNIT_G21;                // set default unit to mm
  this->machineMode.positioning = COMMAND_POSITIONING_G90;  // set default absolute mode positioning

  // set axis logical position, maybe load from EEPROM position before previous reset
  this->motor_x.setPosition(0);
  this->motor_y.setPosition(0);
  this->motor_z.setPosition(0);
  pinMode(PIN_ENABLE_MOTORS, OUTPUT);
  digitalWrite(PIN_ENABLE_MOTORS, LOW);

  // set line index to 0
  this->lineIndex = 0;
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
    // Serial.print("[PARSER: ");
    // Serial.print("Symbol: "); Serial.print(commandSymbol);  Serial.print("  ");
    // Serial.print("Number: "); Serial.print(commandNumber);  Serial.print("]\n");
    // Serial.print("CommandNumber: ["); Serial.print(commandNumber);  Serial.print("]\n");

    switch(commandSymbol)
    {
      // treat case with N symbol received
      case 'N':
      {
        this->lineIndex = integerPart;
        break;
      }

      // treat case with G symbol received
      case 'G':
      {
        switch(integerPart)
        {
          case COMMAND_MOVEMENT_G00:    this->machineMode.movement = COMMAND_MOVEMENT_G00; break;
          case COMMAND_MOVEMENT_G01:    this->machineMode.movement = COMMAND_MOVEMENT_G01; break;
          case COMMAND_MOVEMENT_G02:    this->machineMode.movement = COMMAND_MOVEMENT_G02; break;
          case COMMAND_MOVEMENT_G03:    this->machineMode.movement = COMMAND_MOVEMENT_G03; break;
          case COMMAND_DWELL_G04:       this->machineMode.waitFlag = 1; break;
          case COMMAND_PLANE_XY_G17:    this->machineMode.plane = COMMAND_PLANE_XY_G17; break;
          case COMMAND_PLANE_XZ_G18:    this->machineMode.plane = COMMAND_PLANE_XZ_G18; break;
          case COMMAND_PLANE_YZ_G19:    this->machineMode.plane = COMMAND_PLANE_YZ_G19; break;
          case COMMAND_UNIT_G20:        this->machineMode.unit = COMMAND_UNIT_G20; break;
          case COMMAND_UNIT_G21:        this->machineMode.unit = COMMAND_UNIT_G21; break;
          case COMMAND_HOME_G28:        motor_x.moveToHome(); motor_y.moveToHome(); motor_z.moveToHome(); break;
          case COMMAND_POSITIONING_G90: this->machineMode.positioning = COMMAND_POSITIONING_G90; break;
          case COMMAND_POSITIONING_G91: this->machineMode.positioning = COMMAND_POSITIONING_G91; break;
          case COMMAND_OFFSET_G92:      motor_x.setPosition(0.0); motor_y.setPosition(0.0); motor_z.setPosition(0.0); break;
          case COMMAND_TEST_G250:       this->machineMode.movement = COMMAND_TEST_G250; break;
          case COMMAND_TEST_G251:       this->machineMode.movement = COMMAND_TEST_G251; break;
          case COMMAND_TEST_G252:       this->machineMode.movement = COMMAND_TEST_G252; break;
          case COMMAND_TEST_G253:       this->machineMode.movement = COMMAND_TEST_G253; break;
          case COMMAND_TEST_G254:       this->machineMode.movement = COMMAND_TEST_G254; break;
          case COMMAND_TEST_G255:       this->machineMode.movement = COMMAND_TEST_G255; break;
          default: return ERROR_COMMAND_NOT_SUPPORTED;
        }
        break;
      }

      // treat case with M symbol received
      case 'M':
      {
        switch(integerPart)
        {
          case 2: Serial.println("CNC>>>File succcesfully executed!"); break;
          case 17: digitalWrite(PIN_ENABLE_MOTORS, LOW);  break;  // enable all motors
          case 18: digitalWrite(PIN_ENABLE_MOTORS, HIGH); break;  // disable all motors
          default: return ERROR_COMMAND_NOT_SUPPORTED;
        }
        break;
      }


      // treat case with # symbol received  (special commands)
      case '#':
      {
        switch(integerPart)
        {
          case 1:
            Serial.print("{X: "); Serial.print(motor_x.getPosition(), 10); Serial.print(",");
            Serial.print(" Y: "); Serial.print(motor_y.getPosition(), 10); Serial.print(",");
            Serial.print(" Z: "); Serial.print(motor_z.getPosition(), 10); Serial.print("}\n");
            break;
          case 2: break;
        }
        break;
      }


      // hanndle command parameters
      default:
      {
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
          default:  return ERROR_SYMBOL_NOT_SUPPORTED;
        } // end switch 2
      } // end default case
    } // end switch 1
  } //end while

  // Line parsing complete
  buffer.ready = false;
  return RETURN_SUCCES;
}




uint8_t Machine::executeMovementCommand()
{
  // uint8_t returnedStatus = RETURN_SUCCES;

  // no movement requested
  if(!this->newCmd.flags.all) { return RETURN_SUCCES; }

  // wait command requested (G04)
  if(this->machineMode.waitFlag && this->newCmd.flags.x)
  {
    this->machineMode.waitFlag = 0; // reset flag
    delay(this->newCmd.x);
    return RETURN_SUCCES;
  }

  // set motors speed
  if(this->newCmd.flags.f) { this->setMotorsSpeed(this->newCmd.f);  }

  switch(this->machineMode.movement)
  {
    case COMMAND_MOVEMENT_G00:
    {
      if(this->newCmd.flags.x) { performAxisLinearMovement_G00(motor_x, this->newCmd.x); }
      if(this->newCmd.flags.y) { performAxisLinearMovement_G00(motor_y, this->newCmd.y); }
      if(this->newCmd.flags.z) { performAxisLinearMovement_G00(motor_z, this->newCmd.z); }
      break;
    }
    case COMMAND_MOVEMENT_G01:
    {
      Point3d_float_t p1{motor_x.getPosition(), motor_y.getPosition(), motor_z.getPosition()}; // new point
      if(this->newCmd.flags.x) { p1.x = this->newCmd.x; }
      if(this->newCmd.flags.y) { p1.y = this->newCmd.y; }
      if(this->newCmd.flags.z) { p1.z = this->newCmd.z; }
      performLinearInterpolation_G01(p1);
      break;
    }
    case COMMAND_MOVEMENT_G02:
    case COMMAND_MOVEMENT_G03:
    {
      // Check arc direction
      uint8_t arcDirection = (this->machineMode.movement == COMMAND_MOVEMENT_G02)? 0:1;
      // TODO: check if new point: p1 is on circle
      if(this->machineMode.plane == COMMAND_PLANE_XY_G17)        { performCircularArcInterpolation(motor_x, motor_y, arcDirection); }
      else if( this->machineMode.plane == COMMAND_PLANE_XZ_G18 ) { performCircularArcInterpolation(motor_x, motor_z, arcDirection); }
      else if( this->machineMode.plane == COMMAND_PLANE_YZ_G19 ) { performCircularArcInterpolation(motor_y, motor_z, arcDirection); }
      else { /* This is only reached if invalid plane is selected */ }
      break;
    }
    case COMMAND_TEST_G250: { performCircularArcInterpolation_Optimized(motor_x, motor_y, 0); break; }
    case COMMAND_TEST_G251: { performCircularArcInterpolation_Optimized(motor_x, motor_y, 1); break; }
    case COMMAND_TEST_G252: { break; }
    case COMMAND_TEST_G253: { break; }
    case COMMAND_TEST_G254: { break; }
    case COMMAND_TEST_G255:
    {
      // Test movement: optimized version, but not working with very large numbers
      Point3d_int32_t p1{motor_x.getPositionInSteps(), motor_y.getPositionInSteps(), motor_z.getPositionInSteps()}; // new point
      if(this->newCmd.flags.x) { p1.x = CONVERT_INTO_STEPS(this->newCmd.x); }
      if(this->newCmd.flags.y) { p1.y = CONVERT_INTO_STEPS(this->newCmd.y); }
      if(this->newCmd.flags.z) { p1.z = CONVERT_INTO_STEPS(this->newCmd.z); }
      performLinearInterpolation_G01_Optimized(p1);
      break;
    }
    default:
      return ERROR_UNEXPECTED;
  }

  return RETURN_SUCCES;
}




uint8_t Machine::setMotorsSpeed( uint16_t newSpeed)
{
  motor_x.setSpeed(newSpeed);
  motor_y.setSpeed(newSpeed);
  motor_z.setSpeed(newSpeed);
  return RETURN_SUCCES;
}




uint8_t Machine::performAxisLinearMovement_G00(Motor& inputMotor, int32_t newAxisPosition)
{
  if(inputMotor.getPosition() < newAxisPosition)  // MOVE FORWARD
  {
    while(inputMotor.getPosition() < newAxisPosition) { if(!inputMotor.step(MOTOR_DIRECTION_FORWARD)) {return ERROR_AXIS_ENDING_EXCEEDED;} }
  }
  else if(inputMotor.getPosition() > newAxisPosition)  // MOVE REVERSE
  {
    while(inputMotor.getPosition() > newAxisPosition) { if(!inputMotor.step(MOTOR_DIRECTION_REVERSE)) {return ERROR_AXIS_ENDING_EXCEEDED;} }
  }
  return RETURN_SUCCES;
}





uint8_t Machine::performLinearInterpolation_G01(Point3d_float_t& p1)
{
  Point3d_float_t p0{motor_x.getPosition(), motor_y.getPosition(), motor_z.getPosition()}; // get coordonates of current position
  Point3d_float_t steps{0.0, 0.0, 0.0};  // reaminder between 2 intermediar steps

  // calculate direction vector of the line between p0 and p1
  Point3d_float_t directionVector{p1.x - p0.x,   p1.y - p0.y,  p1.z - p0.z};

  // calculate euclidian distance between p0 and p1
  float EuclidianDistance = sqrt( pow(directionVector.x, 2) + pow(directionVector.y, 2) + pow(directionVector.z, 2) );
  float interpolationResolution_x = (STEP_RESOLUTION / EuclidianDistance) * abs(directionVector.x);
  float interpolationResolution_y = (STEP_RESOLUTION / EuclidianDistance) * abs(directionVector.y);
  float interpolationResolution_z = (STEP_RESOLUTION / EuclidianDistance) * abs(directionVector.z);

  // compute motor directions
  uint8_t direction_x = ( directionVector.x < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;
  uint8_t direction_y = ( directionVector.y < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;
  uint8_t direction_z = ( directionVector.z < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;

  // Pn(xn, yn, zn) = p0 + (STEP_RESOLUTION / EuclidianDistance) * directionVector
  while( !equals(motor_x.getPosition(), p1.x) || !equals(motor_y.getPosition(), p1.y) || !equals(motor_z.getPosition(), p1.z) )
  {
    steps.x += interpolationResolution_x;
    steps.y += interpolationResolution_y;
    steps.z += interpolationResolution_z;

    if( steps.x >= STEP_RESOLUTION )
    {
      if(!motor_x.step(direction_x)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.x -= STEP_RESOLUTION;
    }

    if( steps.y >= STEP_RESOLUTION )
    {
      if(!motor_y.step(direction_y)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.y -= STEP_RESOLUTION;
    }

    if( steps.z >= STEP_RESOLUTION )
    {
      if(!motor_z.step(direction_z)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.z -= STEP_RESOLUTION;
    }

  }
  return RETURN_SUCCES;
}




uint8_t Machine::performLinearInterpolation_G01_Optimized(Point3d_int32_t& p1)
{
  // get coordonates of current position
  Point3d_int32_t p0{motor_x.getPositionInSteps(), motor_y.getPositionInSteps(), motor_z.getPositionInSteps()};
  Point3d_float_t steps{0.0, 0.0, 0.0};  // reaminder between 2 intermediar steps

  // calculate direction vector of the line between p0 and p1
  Point3d_int32_t directionVector{p1.x - p0.x,   p1.y - p0.y,  p1.z - p0.z};
  // compute motor directions
  uint8_t direction_x = ( directionVector.x < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;
  uint8_t direction_y = ( directionVector.y < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;
  uint8_t direction_z = ( directionVector.z < 0.0 ) ? MOTOR_DIRECTION_REVERSE : MOTOR_DIRECTION_FORWARD;

  // calculate euclidian distance between p0 and p1
  float EuclidianDistance = sqrt( pow(directionVector.x, 2) + pow(directionVector.y, 2) + pow(directionVector.z, 2) );
  float interpolationResolution_x = (1.0 / EuclidianDistance) * abs(directionVector.x);
  float interpolationResolution_y = (1.0 / EuclidianDistance) * abs(directionVector.y);
  float interpolationResolution_z = (1.0 / EuclidianDistance) * abs(directionVector.z);

  // Pn(xn, yn, zn) = p0 + (STEP_RESOLUTION / EuclidianDistance) * directionVector
  while( (motor_x.getPositionInSteps() != p1.x) || (motor_y.getPositionInSteps() != p1.y) || (motor_z.getPositionInSteps() != p1.z) )
  {
    steps.x += interpolationResolution_x;
    steps.y += interpolationResolution_y;
    steps.z += interpolationResolution_z;

    if( steps.x >= 1 )
    {
      if(!motor_x.step(direction_x)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.x -= 1;
    }

    if( steps.y >= 1 )
    {
      if(!motor_y.step(direction_y)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.y -= 1;
    }

    if( steps.z >= 1 )
    {
      if(!motor_z.step(direction_z)) { return ERROR_AXIS_ENDING_EXCEEDED; }
      steps.z -= 1;
    }

  }
  return RETURN_SUCCES;
}




// Circle ecuation: (x − center.x)^2 + (y − center.y)^2 = radius^2
// Possible moving directions:
// ClockWise:
// Case 1: (p.x < center.x) AND (p.y > center.y) => X++, Y++  (We are in upper left)
// Case 2: (p.x > center.x) AND (p.y > center.y) => X++, Y--  (We are in upper right)
// Case 3: (p.x > center.x) AND (p.y < center.y) => X--, Y--  (We are in lower right)
// Case 4: (p.x < center.x) AND (p.y < center.y) => X--, Y++  (We are in lower left)
uint8_t Machine::performCircularArcInterpolation(Motor& first_axis, Motor& second_axis, uint8_t DIR)
{
  Point2d_float_t p0{first_axis.getPosition(), second_axis.getPosition()};  // starting point
  Point2d_float_t p1{this->newCmd.x, this->newCmd.y};                       // ending point
  Point2d_float_t p = p0;                                                   // moving point

  // Compute center of the cerc
  Point2d_float_t center{0,0};
  if(this->newCmd.flags.r)
  {
    center.x = p0.x + this->newCmd.r;
    center.y = p0.y;
  }
  else
  {
    center.x = first_axis.getPosition() + this->newCmd.i;
    center.y = second_axis.getPosition() + this->newCmd.j;
  }

  // Radius squared of the circle
  float radius_squared = pow(center.x - p0.x, 2) + pow(center.y - p0.y, 2);

  // DEBUGING
  // Serial.print("[ p0("); Serial.print(p0.x, DEC); Serial.print(","); Serial.print(p0.y, DEC); Serial.print(")");
  // Serial.print("  p1("); Serial.print(p1.x, DEC); Serial.print(","); Serial.print(p1.y, DEC); Serial.print(")");
  // Serial.print("  center("); Serial.print(center.x, DEC); Serial.print(","); Serial.print(center.y, DEC); Serial.print(") ]\n");
  // Serial.print("radius_squared: "); Serial.print(radius_squared); Serial.print("\n");
  // Serial.print("pow(center.x - p0.x, 2): "); Serial.print(pow(center.x - p0.x, 2)); Serial.print("\n");
  // Serial.print("pow(center.y - p0.y, 2): "); Serial.print(pow(center.y - p0.y, 2)); Serial.print("\n");

  // tests
  uint8_t f = 0, a = 0, b = 0;
  float fxy = 0.0;
  uint8_t decision_binary = 0u;
  int8_t step_x = 0, step_y = 0;
  do
  {
    fxy = pow(p.x-center.x, 2) + pow(p.y-center.y, 2) - radius_squared;
    f = (fxy < 0)?        0:1;
    a = (p.x < center.x)? 0:1;
    b = (p.y < center.y)? 0:1;

    // compute if we step on x direction or y direction
    step_x = 0; step_y = 0;
    decision_binary = ((DIR<<3) | (f<<2) | (a<<1)) + b;
    switch( decision_binary )
    {
      // clockwise directions
      case  0: { step_x = -1; break; }
      case  1: { step_y =  1; break; }
      case  2: { step_y = -1; break; }
      case  3: { step_x =  1; break; }
      case  4: { step_y =  1; break; }
      case  5: { step_x =  1; break; }
      case  6: { step_x = -1; break; }
      case  7: { step_y = -1; break; }
      // counter clockwise directions
      case  8: { step_y = -1; break; }
      case  9: { step_x = -1; break; }
      case 10: { step_x =  1; break; }
      case 11: { step_y =  1; break; }
      case 12: { step_x =  1; break; }
      case 13: { step_y = -1; break; }
      case 14: { step_y =  1; break; }
      case 15: { step_x = -1; break; }
    }

    // execute step
    if(step_x < 0)      { if(!first_axis.step(MOTOR_DIRECTION_REVERSE)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else if(step_x > 0) { if(!first_axis.step(MOTOR_DIRECTION_FORWARD)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else { /*Do not move yet on x axis yet*/ }
    if(step_y < 0)      { if(!second_axis.step(MOTOR_DIRECTION_REVERSE)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else if(step_y > 0) { if(!second_axis.step(MOTOR_DIRECTION_FORWARD)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else { /*Do not move yet on y axis yet*/ }

    // uptade point coordonates
    p.x = first_axis.getPosition();
    p.y = second_axis.getPosition();


    // exit when the end point of the arc is reached
    if( (this->newCmd.flags.x && this->newCmd.flags.y) && (equals(p.x, p1.x) && equals(p.y, p1.y)) )
    { Serial.print("ARC COMPLETE: P1 REACHED!"); break;  }
  }
  while( !equals(p.x, p0.x) || !equals(p.y, p0.y) ); // exit when cerc is completed

  Serial.print("p("); Serial.print(p.x, DEC); Serial.print(","); Serial.print(p.y, DEC); Serial.print(")\n");
  if(this->newCmd.flags.x && this->newCmd.flags.y) // corect floating point position problems
  {
    first_axis.setPosition(this->newCmd.x);
    second_axis.setPosition(this->newCmd.y);
  }

  return RETURN_SUCCES;
}



// optimized version, not working with very large numbers
uint8_t Machine::performCircularArcInterpolation_Optimized(Motor& axis_1, Motor& axis_2, uint8_t DIR)
{
  Point2d_int32_t p0{motor_x.getPositionInSteps(), motor_x.getPositionInSteps()};             // starting point
  Point2d_int32_t p1{CONVERT_INTO_STEPS(this->newCmd.x), CONVERT_INTO_STEPS(this->newCmd.y)}; // ending point
  Point2d_int32_t p = p0;                                                                     // moving point

  // Compute center of the cerc
  Point2d_int32_t center{0,0};
  if(this->newCmd.flags.r)
  {
    center.x = p0.x + CONVERT_INTO_STEPS(this->newCmd.r);
    center.y = p0.y;
  }
  else
  {
    center.x = motor_x.getPositionInSteps() + CONVERT_INTO_STEPS(this->newCmd.i);
    center.y = motor_y.getPositionInSteps() + CONVERT_INTO_STEPS(this->newCmd.j);
  }

  // Radius squared of the circle
  float radius_squared = pow(center.x - p0.x, 2) + pow(center.y - p0.y, 2);

  // DEBUGING
  // Serial.print("[ p0("); Serial.print(p0.x, DEC); Serial.print(","); Serial.print(p0.y, DEC); Serial.print(")");
  // Serial.print("  p1("); Serial.print(p1.x, DEC); Serial.print(","); Serial.print(p1.y, DEC); Serial.print(")");
  // Serial.print("  center("); Serial.print(center.x, DEC); Serial.print(","); Serial.print(center.y, DEC); Serial.print(") ]\n");
  // Serial.print("radius_squared: "); Serial.print(radius_squared); Serial.print("\n");
  // Serial.print("pow(center.x - p0.x, 2): "); Serial.print(pow(center.x - p0.x, 2)); Serial.print("\n");
  // Serial.print("pow(center.y - p0.y, 2): "); Serial.print(pow(center.y - p0.y, 2)); Serial.print("\n");

  // tests
  uint8_t f = 0, a = 0, b = 0;
  float fxy = 0.0;
  uint8_t decision_binary = 0u;
  int8_t step_x = 0, step_y = 0;
  do
  {
    fxy = pow(p.x-center.x, 2) + pow(p.y-center.y, 2) - radius_squared;
    f = (fxy < 0)?   0:1;
    a = (p.x < center.x)? 0:1;
    b = (p.y < center.y)? 0:1;

    // compute if we step on x direction or y direction
    step_x = 0; step_y = 0;
    decision_binary = ((DIR<<3) | (f<<2) | (a<<1)) + b;
    switch( decision_binary )
    {
      // clockwise directions
      case  0: { step_x = -1; break; }
      case  1: { step_y =  1; break; }
      case  2: { step_y = -1; break; }
      case  3: { step_x =  1; break; }
      case  4: { step_y =  1; break; }
      case  5: { step_x =  1; break; }
      case  6: { step_x = -1; break; }
      case  7: { step_y = -1; break; }
      // counter clockwise directions
      case  8: { step_y = -1; break; }
      case  9: { step_x = -1; break; }
      case 10: { step_x =  1; break; }
      case 11: { step_y =  1; break; }
      case 12: { step_x =  1; break; }
      case 13: { step_y = -1; break; }
      case 14: { step_y =  1; break; }
      case 15: { step_x = -1; break; }
    }

    // execute step
    if(step_x < 0)      { if(!motor_x.step(MOTOR_DIRECTION_REVERSE)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else if(step_x > 0) { if(!motor_x.step(MOTOR_DIRECTION_FORWARD)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else { /*Do not move yet on x axis yet*/ }
    if(step_y < 0)      { if(!motor_y.step(MOTOR_DIRECTION_REVERSE)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else if(step_y > 0) { if(!motor_y.step(MOTOR_DIRECTION_FORWARD)) { return ERROR_AXIS_ENDING_EXCEEDED; } }
    else { /*Do not move yet on y axis yet*/ }

    // uptade point coordonates
    p.x = motor_x.getPositionInSteps();
    p.y = motor_y.getPositionInSteps();


    // exit when the end point of the arc is reached
    if( !this->newCmd.flags.r && (p.x == p1.x && p.y == p1.y) ) { Serial.print("ARC COMPLETE: P1 REACHED!"); break;  }
  }
  while( (p.x != p0.x) || (p.y != p0.y) ); // exit when cerc is completed

  Serial.print("p("); Serial.print(p.x, DEC); Serial.print(","); Serial.print(p.y, DEC); Serial.print(")\n");
  // Serial.print("fxy: "); Serial.print(fxy); Serial.print("  ");
  // Serial.print("STEP_X: "); Serial.print(step_x); Serial.print("  ");
  // Serial.print("STEP_Y: "); Serial.print(step_y); Serial.print("  ");
  // Serial.print("BIN("); Serial.print(decision_binary, BIN); Serial.print(") >\n");

  return RETURN_SUCCES;
}
