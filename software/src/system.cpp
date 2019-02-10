#include "system.h"
#include <avr/wdt.h>


// Disable or enable DEBUG messages
uint8_t DEBUG = 0;
float EPSILON = 0.003;

// perform requested delay in microseconds
void delay_sync(uint32_t us)
{
  do {
    if(us < 2)    { return; }  // delayMicroseconds - cant handle delays lower then 2us
    if(us < 1000) { delayMicroseconds((uint16_t)us);  return; }
    delayMicroseconds(1000);
    us -= 1000;
  }while(true);
}




// perform microcontroller reset
void performSoftwareReset()
{
  wdt_enable( WDTO_15MS);
  while(1) {} // wait for watchdog to trigger an reset
}




// handle runtime errors
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

    case ERROR_AXIS_ENDING_EXCEEDED:
      Serial.print( F("CNC>>>error: Axis ending exceeded!\n") );
      break;

    case ERROR_UNEXPECTED:
      Serial.print( F("CNC>>>error: An unexpected error occured!\n") );
      break;

    default:
      Serial.print( F("CNC>>>error: Unhandled error!\n") );
  }
}
