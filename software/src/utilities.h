#ifndef UTILITIES_H
#define UTILITIES_H
#include <Arduino.h>
#include "config.h"


// Data types
typedef struct
{
  char data[RX_BUFF_SIZE];
  uint8_t currentWritePos;  // current reading position in data buffer
  uint8_t currentParsePos;  // current parsing position in data buffer
  boolean ready;            // is set to true if buffer is ready for parsing
}Rx_buffer_t;


// Functions
uint8_t readLine(int16_t inputChar, Rx_buffer_t& buffer, uint8_t size);
uint8_t parseNumber(Rx_buffer_t& buffer, float& outputNumber);
bool equals(float f1, float f2);


#endif
