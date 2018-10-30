#include "utilities.h"
#include <avr/wdt.h>
#include <stdlib.h>


// Private functions declaration
static bool isValidCharacter(uint8_t ch);



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
          buffer.data[buffer.currentWritePos++] = (uint8_t)inputChar;
        }
      }
  }
  return RETURN_SUCCES;
}




uint8_t parseNumber(Rx_buffer_t& buffer, float outputNumber)
{
  // Ex of max size possible number: -12345.12345678
  const uint8_t MAX_NUMBER_SIZE = 16;
  uint8_t numberStr[MAX_NUMBER_SIZE];

  uint8_t ch = 0;
  uint8_t currentDigit = 0;
  int8_t negativeFactor = 1;


  // extract string formatted number
  uint8_t pos = 0;
  while( buffer.data[ buffer.currentParsePos] != ' ')
  {
    if(pos >= size -1) { return ERROR_NUMBER_PARSING; }
    if(buffer.currentParsePos >= RX_BUFF_SIZE-1 ) { return ERROR_PARSING_NUMBER; }

    ch = buffer.data[ buffer.currentParsePos++ ];
    if( false != isNumericSymbol(ch) ) { return ERROR_PARSING_NUMBER; }
    numberStr[pos++] = ch;
  }
  numberStr[pos] = 0; // append null to the end

  outputNumber = atof(numberStr);
  if( (outputNumber == 0.0) && (strncmp(numberStr, "0.0", 3) != 0) ) { return ERROR_PARSING_NUMBER; }

  return RETURN_SUCCES;
}



uint8_t stringToInteger(uint8_t* inputString, uint8_t digits, int16_t& outputNumber)
{
  uint8_t curentDigitValue;

  if()

  for(uint8_t pos = digits-1; digits>=0; digits-- )
  {
    if( inputString[pos] >= '0' && inputString[pos] <= '9')
    {
      curentDigitValue = inputString[pos] - (uint8_t)'0';
      outputNumber += pow(10, pos) * curentDigitValue;
    }
    else
    {
      return RETURN_ERROR;
    }
  }
  // 4321 = 4 * 1000

}




/* Check if a received character is a valid gcode character */
static bool isValidCharacter(uint8_t ch)
{
  if( (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'z') ||
      (ch >= 'A' && ch <= 'Z') ||
      (ch == '#') ||
      (ch == '-') ||
      (ch == '.') )
  {
    return true;
  }
  else
  {
    return false;
  }
}

static bool isNumericSymbol(uint8_t ch)
{
  if( (ch == '-') || (ch >= '0' && ch <= '9') || (ch == '.') )
    return true;
  return false;
}
