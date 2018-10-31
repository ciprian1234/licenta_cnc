#include "system.h"
#include <avr/wdt.h>


// perform microcontroller reset
void performSoftwareReset()
{
  wdt_enable( WDTO_15MS);
  while(1) {} // wait for watchdog to trigger an reset
}


void handleRuntimeError(uint8_t errorCode)
{
  // reset rx buffer
  memset(&rx_buffer, 0, sizeof(Rx_buffer_t));

  switch(errorCode)
  {
    case ERROR_RX_BUFFER_OVERFLOW:
      Serial.print( F("CNC>>>error: Rx buffer overflow!\n") );
      while( '\n' != Serial.read() ) { /* Ignore all characters until newline is reached */ }
      break;

    case ERROR_INVALID_CHAR_RECEIVED:
      Serial.print( F("CNC>>>error: Invalid character received!\n") );
      while( '\n' != Serial.read() ) { /* Ignore all characters until newline is reached */ }
      break;

    case ERROR_COMMAND_NOT_SUPPORTED:
      Serial.print( F("CNC>>>error: Command not supported!\n") );
      break;

    case ERROR_SYMBOL_NOT_SUPPORTED:
      Serial.print( F("CNC>>>error: Symbol not supported!\n") );
      break;

    case ERROR_INVALID_NUMBER_FORMAT:
      Serial.print( F("CNC>>>error: Invalid number format or number size exceded!\n") );
      break;

    case ERROR_UNEXPECTED:
      Serial.print( F("CNC>>>error: An unexpected error occured!\n") );
      break;

    default:
      Serial.print( F("CNC>>>error: Unhandled error!\n") );
  }
}
