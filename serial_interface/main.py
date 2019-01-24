import time, serial
import sys, threading

# thread is reading in infilite loop, and sends a flag when
# data has come and is available for receiveing new commands
cnc_available = False
connected = False


# thread function for reading cnc response
def readService(ser):
    global cnc_available
    while True:
        response = ser.readline().decode('utf-8')
        if( ("connected" in response) or ("done" in response) or ("error" in response) ):
            cnc_available = True

        print(response, end='')
        sys.stdout.flush()



def send_gcode_file(ser, gcodeFile):
    global cnc_available
    with open(gcodeFile, "r") as file:
        # go through each line and send it
        for line in file:
            # wait until cnc is available
            while not cnc_available:
                pass

            ser.write( line.encode('utf-8') )
            cnc_available = False









################################################################################

#print menu
print("Available options:")
print("1. Send normal gcode commands from console.")
print("2. Type file to send a gcode file.")
print("3. Type quit to exit program.")

with serial.Serial('COM5', 115200, timeout = 1) as ser: # TODO: Set timeout = None (in blocking mode)
    print("Connected to serial port: %s"%ser.name)
    readThread = threading.Thread(target = readService, args = [ser])
    readThread.start()


    while(True):
        input_cmd = input()

        if( input_cmd == "quit"):
            exit("Exiting...")
        if( input_cmd == "file"):
            send_gcode_file(ser, "infile.gcode")
        else:
            # Send gcode commands from console
            if cnc_available:
                input_cmd += '\n'
                ser.write( input_cmd.encode('utf-8') )
                cnc_available = False
            else:
                print("INFO: CNC is not available, wait until previous command is finished!")
                sys.stdout.flush()
