#include "utilities.h"

//SYSTEM
#define ERROR_RX_BUFFER_OVERFLOW 0x01
void handleRuntimeError(uint8_t error);
void performSoftwareReset(void); //init all global variables

/* Var declaration */
Rx_buffer_t rx_buffer;



/* Function declarations */
bool readLine(int16_t inputChar, Rx_buffer_t& buffer, uint8_t size);



void setup() {
    Serial.begin(BAUD_RATE);
}

void loop() {
  // read from serial
  if( readLine(Serial.read(), rx_buffer, RX_BUFF_SIZE) )
  {
    //machine



    // perform syntactic validation
    // perform semantic validation
    // extract and parse
    // execute commands
    // return feedback
    // Serial.print( F("cnc>") );
  }




}


/* Read from inputChar, into the buffer until newline is reached
*  returns: TRUE if newline is reached, FALSE otherwise */
bool readLine(int16_t inputChar, Rx_buffer_t& buffer, uint8_t size)
{
  if( inputChar == -1 ) { return 0; } // no char available

  switch(inputChar)
  {
    case '\r': break; // ignore CR
    case '\n': // endline reached, return TRUE
      buffer.data[ buffer.pos ] = 0;
      buffer.end = buffer.pos;
      buffer.pos = 0;
      buffer.start = 0;
      return 1;
    default:
      if( buffer.pos < size - 1 )
      {
        buffer.data[ buffer.pos++ ] = inputChar;
      }
      else
      {
        handleRuntimeError(ERROR_RX_BUFFER_OVERFLOW);
      }
  }
  return 0;
}



/* */
void handleRuntimeError(uint8_t error)
{

}

void parseLine()
{
  uint8_t pos = 0;
  while(rx_buffer.data[pos] != 0)
  {

  }

}
