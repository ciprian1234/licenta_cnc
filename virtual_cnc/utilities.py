class MachineException(Exception):
    pass

class InvalidCommandException(Exception):
    pass


def isNumericSymbol(chr):
    if( chr in ".-0123456789"):
        return True
    return False


# extract number from buffer and return it
def parseNumber(inputBuffer):
    tempBuff = ""
    while( (len(inputBuffer) > 0) and isNumericSymbol(inputBuffer[0]) ):
        tempBuff += inputBuffer[0]
        inputBuffer = inputBuffer[1:]

    # Convert extracted str to number
    number = None
    try:
        number = float(tempBuff)
    except(ValueError, TypeError):
        raise InvalidCommandException("Failed to convert token: <%s> to number"%tempBuff)
    return (float(number), inputBuffer)




# check if two float numbers are equal
EPSILON = 0.001
def equals(f1, f2):
    if( (f2 >= (f1 - EPSILON)) and (f2 <= (f1 + EPSILON)) ):
        return True
    return False
