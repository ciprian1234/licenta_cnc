#include "utilities.h"
#include "system.h"
#include <stdlib.h>
// Private functions declaration
static bool isValidCharacter(char ch);
static bool isNumericSymbol(char ch);



/* Read from inputChar, into the buffer until newline is reached
*  returns: RETURN_SUCCES if no error occurs or error code number otherwise */
uint8_t readLine(int16_t inputChar, Rx_buffer_t& buffer, uint8_t size)
{
  if( inputChar == -1 ) { return RETURN_SUCCES; } // no char available

  switch(inputChar)
  {
    case '\r': break; // ignore CR
    case '\n': // endline reached, buffer is ready for parsing
      buffer.data[ buffer.currentWritePos ] = 0;
      buffer.currentWritePos = 0;
      buffer.currentParsePos = 0;
      buffer.ready = true;
      return RETURN_SUCCES;
    default:

      if( buffer.currentWritePos >= size - 1 )
      {
        return ERROR_RX_BUFFER_OVERFLOW;
      }
      else
      {
        // perform character validation
        if( false == isValidCharacter(inputChar) )
        {
          return ERROR_INVALID_CHAR_RECEIVED;
        }
        else
        {
          buffer.data[buffer.currentWritePos++] = (char)inputChar;
        }
      }
  }
  return RETURN_SUCCES;
}




uint8_t parseNumber(Rx_buffer_t& buffer, float& outputNumber)
{
  // Ex of max size possible number: -12345.12345678
  const uint8_t MAX_NUMBER_SIZE = 16;
  char numberStr[MAX_NUMBER_SIZE];

  // extract number in string format
  uint8_t pos = 0;
  while( isNumericSymbol(buffer.data[buffer.currentParsePos]) )
  {
    if(pos >= MAX_NUMBER_SIZE-1) { return ERROR_INVALID_NUMBER_FORMAT; }
    if(buffer.currentParsePos >= RX_BUFF_SIZE-1 ) { return ERROR_INVALID_NUMBER_FORMAT; }
    numberStr[pos++] = buffer.data[ buffer.currentParsePos++ ];
  }
  numberStr[pos] = 0; // append null to the end
  if(pos == 0 ) { return ERROR_INVALID_NUMBER_FORMAT; }

  // DEBUG
  Serial.print("2.EXTRACTION: {{"); Serial.print(numberStr); Serial.print("}}\n");

  outputNumber = atof(numberStr);
  return RETURN_SUCCES;
}




/* Check if a received character is a valid gcode character */
static bool isValidCharacter(char ch)
{
  if( (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'z') ||
      (ch >= 'A' && ch <= 'Z') ||
      (ch == '#') ||
      (ch == '-') ||
      (ch == '.') ||
      (ch == ' ') )
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* Check if a character is a numeric symbol */
static bool isNumericSymbol(char ch)
{
  if( (ch == '-') || (ch >= '0' && ch <= '9') || (ch == '.') )
    return true;
  return false;
}
