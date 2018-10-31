#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include "utilities.h"

// If debug is enabled, debug messages will be sent to serial
#define DEBUG_ENABLED (1u)

//#define DEBUG_PRINT(str)  if(DEBUG_ENABLED) { Serial.println(str); }

// extern variables
extern Rx_buffer_t rx_buffer;

// Return status of the function is
#define RETURN_SUCCES                   (0u)
#define ERROR_RX_BUFFER_OVERFLOW        (1u)
#define ERROR_INVALID_CHAR_RECEIVED     (2u)
#define ERROR_COMMAND_NOT_SUPPORTED     (3u)
#define ERROR_SYMBOL_NOT_SUPPORTED      (4u)
#define ERROR_INVALID_NUMBER_FORMAT     (5u)
#define ERROR_UNEXPECTED              (255u)


void performSoftwareReset(void);
void handleRuntimeError(uint8_t errorCode);


#endif


// If ERROR_RX_BUFFER_OVERFLOW, ignore current line, stop the printer at current machine
// state and send negative feedback to cnc

// If ERROR_INVALID_INPUT_CHAR, ignore current line, stop the printer at current machine
// state and send negative feedback

// If ERROR_COMMAND_NOT_SUPPORTED, ignore current command, send feedback, go to next command
