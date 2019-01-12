# Max X axis (mm)
AXIS_MAX_POSITION_X = 200

# Max Y axis (mm)
AXIS_MAX_POSITION_Y = 200

# Max Z axis (mm)
AXIS_MAX_POSITION_Z = 50

# Number of steps per a complete 360 degree rotation of stepper motor
NUMBER_OF_STEPS_360 = 800 # microsteping: 1/4

# Number of milimeters a axis travels when an 360 rotation is complete
AXIS_TRAVEL_DISTANCE_360 = 8

# Stepping resolution between motor steps, measured in mm
STEP_RESOLUTION = (AXIS_TRAVEL_DISTANCE_360 / NUMBER_OF_STEPS_360)
