import os, traceback, time, json
from sys import exit
################################################################################
from machine import Machine
from utilities import MachineException, InvalidCommandException
################################################################################

# main menu
def executeMainMenu():
    while(1):
        print("__________ VIRTUAL CNC MACHINE MAIN MENU __________")
        print("1. Start interactive cmd")
        print("2. Open and execute a gcode file")
        print("3. Exit")
        userInput = int(input("Please select an option: "))
        if(userInput == 3): exit("Exit...");
        if(userInput == 1 or userInput == 2): return userInput



################################################################################
##################################### MAIN #####################################
virtualMachine = Machine()

userInput = executeMainMenu()
if(userInput == 1):
    print("Interactive cmd started... Type quit() to terminate program.")
    while(1):
        inputBuffer = input(">>>")
        if(inputBuffer == "quit()"): exit("Exit...");

        try:
            virtualMachine.parseLine(inputBuffer)
            virtualMachine.executeMovementCommand()
        except MachineException as e:
            print("MachineException: " + str(e))
        except InvalidCommandException as e:
            print("InvalidCommandException: " + str(e))
        except Exception as e:
            print("\nUNEXPECTED ERROR: ")
            print(traceback.format_exc())
