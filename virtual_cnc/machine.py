import re
################################################################################
import config
from utilities import equals, parseNumber, MachineException, InvalidCommandException



#################################### MACROS ####################################
X_AXIS = "X_AXIS"
Y_AXIS = "Y_AXIS"
Z_AXIS = "Z_AXIS"
MOTOR_DIRECTION_FORWARD = "FORWARD"
MOTOR_DIRECTION_REVERSE = "REVERSE"
################################################################################



# all possible types of machine commands
COMMAND_TYPE_G          = 0
COMMAND_TYPE_M          = 1
COMMAND_TYPE_SPECIAL    = 2

# movement commands
COMMAND_MOVEMENT_G00 = 0 # secvential linear move [DEFAULT]
COMMAND_MOVEMENT_G01 = 1 # linear move
COMMAND_MOVEMENT_G02 = 2 # arc move clockwise
COMMAND_MOVEMENT_G03 = 3 # arc move counter clockwise

# unit commands: in mm or inch
COMMAND_UNIT_G20 = 20 # set measurement unit in inches
COMMAND_UNIT_G21 = 21 # set measurement unit in mm  [DEFAULT]

# positioning absolute or relative
COMMAND_POSITIONING_G90 = 90 # abosolute positioning [DEFAULT]
COMMAND_POSITIONING_G91 = 91 # relative, incremental positioning

COMMAND_OFFSET_G92 = 92 # set current physical position as home


class Point_3d_t:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


class MachineMode_t:
    def __init__(self):
        self.unit = COMMAND_UNIT_G21                # set measurement unit in mm  [DEFAULT]
        self.movement = COMMAND_MOVEMENT_G00        # secvential linear move [DEFAULT]
        self.positioning = COMMAND_POSITIONING_G90  # abosolute positioning [DEFAULT]


class State_t:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.f = 1000


class Command_t:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.i = 0.0
        self.j = 0.0
        self.r = 0.0
        self.f = 0.0

        self.flag_x = False
        self.flag_y = False
        self.flag_z = False
        self.flag_i = False
        self.flag_j = False
        self.flag_r = False
        self.flag_f = False

    def print(self):
        print("[%3f, %3f, %3f, %3f, %3f, %3f, %3f]"%(self.x, self.y, self.z, self.i, self.j, self.r, self.f))
        print("[%d, %d, %d, %d, %d, %d, %d]"%(self.flag_x, self.flag_y, self.flag_z, self.flag_i, self.flag_j, self.flag_r, self.flag_f))




class Machine:
    def __init__(self):
         self.machineMode = MachineMode_t() # init machine with default modes
         self.state = State_t()
         self.newCmd = None
         print("Virtual cnc machine initialized...")
         self.printState()




    def printState(self):
        print("{x: %.3f, y:%.3f, z:%.3f, f:%d}"%(self.state.x, self.state.y, self.state.z, self.state.f))




    def parseLine(self, inputBuffer):

        self.newCmd = Command_t() # Assign new command
        commandSymbol = None
        commandNumber = None

        while(len(inputBuffer) > 0):
            commandSymbol = inputBuffer[0]
            if(commandSymbol in " \t"): inputBuffer = inputBuffer[1:]; continue;
            if(commandSymbol not in "#GMXYZIJRFST"): raise InvalidCommandException("Invalid commmand symbol: [%s]"%commandSymbol)

            (commandNumber, inputBuffer) = parseNumber(inputBuffer[1:])
            self.handleCommand(commandSymbol, commandNumber) # send command to GCODE intepreter




    def handleCommand(self, symbol, number):
        integerPart = int(number)
        if(symbol == 'G'):
            if(  integerPart == 0):     self.machineMode.movement = COMMAND_MOVEMENT_G00;
            elif(integerPart == 1):     self.machineMode.movement = COMMAND_MOVEMENT_G01;
            elif(integerPart == 2):     self.machineMode.movement = COMMAND_MOVEMENT_G02;
            elif(integerPart == 3):     self.machineMode.movement = COMMAND_MOVEMENT_G03;
            elif(integerPart == 20):    self.machineMode.unit = COMMAND_UNIT_G20; # set unit in inches
            elif(integerPart == 21):    self.machineMode.unit = COMMAND_UNIT_G21; # set unit in mm
            elif(integerPart == 90):    self.machineMode.positioning = COMMAND_POSITIONING_G90; # abs positioning
            elif(integerPart == 91):    self.machineMode.positioning = COMMAND_POSITIONING_G91; # relative positioning
            elif(integerPart == 28):    self.goToHome();
            elif(integerPart == 92):    pass; # TODO set current position as home
            else: raise InvalidCommandException("Command: [%s%d] not supported!"%(symbol, integerPart));
        elif(symbol == 'M'):
            if(integerPart == 17):      pass; # TODO enable all motors
            elif(integerPart == 18):    pass; # TODO disable all motors
            else: raise InvalidCommandException("Command: [%s%d] not supported!"%(symbol, integerPart));
        elif(symbol == '#'):
            if(  integerPart == 0):     pass; # TODO: print machine mode
            elif(integerPart == 1):     pass; # TODO: print machine state
            elif(integerPart == 2):     pass; # TODO implement something
            else: raise InvalidCommandException("Command: [%s%d] not supported!"%(symbol, integerPart));
        elif(symbol == 'X'):
            self.newCmd.flag_x = True;
            if( self.machineMode.positioning == COMMAND_POSITIONING_G90):   self.newCmd.x = number;
            else: self.newCmd.x = self.state.x + number;
        elif(symbol == 'Y'):
            self.newCmd.flag_y = True;
            if( self.machineMode.positioning == COMMAND_POSITIONING_G90):   self.newCmd.y = number;
            else: self.newCmd.y = self.state.y + number;
        elif(symbol == 'Z'):
            self.newCmd.flag_z = True;
            if( self.machineMode.positioning == COMMAND_POSITIONING_G90):   self.newCmd.z = number;
            else: self.newCmd.z = self.state.z + number;
        elif(symbol == 'I'): self.newCmd.flag_i = True;     self.newCmd.i = number;
        elif(symbol == 'J'): self.newCmd.flag_j = True;     self.newCmd.j = number;
        elif(symbol == 'R'): self.newCmd.flag_r = True;     self.newCmd.r = number;
        elif(symbol == 'F'): self.newCmd.flag_f = True;     self.newCmd.f = number;
        elif(symbol == 'S'): pass; # not supported
        elif(symbol == 'T'): pass; # not supported
        else:  raise InvalidCommandException("Command symbol: [%s] not supported!"%symbol);




    def executeMovementCommand(self):
        # update speed of axis movement if flag is set
        if( self.newCmd.flag_f == True):
            print("TODO: Update feed rate of motor movement with value: %f"%self.newCmd.f)

        self.newCmd.print()
        p0 = Point_3d_t(self.state.x, self.state.y, self.state.z)
        p1 = Point_3d_t(self.state.x, self.state.y, self.state.z)
        if(self.newCmd.flag_x): p1.x = self.newCmd.x;
        if(self.newCmd.flag_y): p1.y = self.newCmd.y;
        if(self.newCmd.flag_z): p1.z = self.newCmd.z;

        if(self.machineMode.movement == COMMAND_MOVEMENT_G00):
            self.performAxisLinearMovement_G00(p0, p1);
        elif(self.machineMode.movement == COMMAND_MOVEMENT_G01):
            self.performLinearInterpolation_G01(p0, p1);
        elif(self.machineMode.movement == COMMAND_MOVEMENT_G02): pass
        elif(self.machineMode.movement == COMMAND_MOVEMENT_G03): pass




    def goToHome(self):
        print("TODO: Implement go to home!")




    def performAxisLinearMovement_G00(self, p0, p1):
        # Compute directions
        directionVector = Point_3d_t(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z)
        direction_x = MOTOR_DIRECTION_FORWARD
        direction_y = MOTOR_DIRECTION_FORWARD
        direction_z = MOTOR_DIRECTION_FORWARD
        if( directionVector.x < 0 ): direction_x = MOTOR_DIRECTION_REVERSE
        if( directionVector.y < 0 ): direction_y = MOTOR_DIRECTION_REVERSE
        if( directionVector.z < 0 ): direction_z = MOTOR_DIRECTION_REVERSE

        self.printState()
        while( not equals(self.state.x, p1.x) ):
            self.transmit_step(X_AXIS, direction_x)
        while( not equals(self.state.y, p1.y) ):
            self.transmit_step(Y_AXIS, direction_y)
        while( not equals(self.state.z, p1.z) ):
            self.transmit_step(Z_AXIS, direction_z)
        self.printState()




    def performLinearInterpolation_G01(self, p0, p1):
        print("TODO: Implement G01")




    def transmit_step(self, axis, direction):
        if(axis == X_AXIS):
            if(direction == MOTOR_DIRECTION_FORWARD): self.state.x += config.STEP_RESOLUTION
            else: self.state.x -= config.STEP_RESOLUTION
        if(axis == Y_AXIS):
            if(direction == MOTOR_DIRECTION_FORWARD): self.state.y += config.STEP_RESOLUTION
            else: self.state.y -= config.STEP_RESOLUTION
        if(axis == Z_AXIS):
            if(direction == MOTOR_DIRECTION_FORWARD): self.state.z += config.STEP_RESOLUTION
            else: self.state.z -= config.STEP_RESOLUTION
        #self.printState()
