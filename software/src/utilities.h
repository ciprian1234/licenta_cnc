#ifndef UTILITIES_H
#define UTILITIES_H
#include <Arduino.h>
#include "config.h"

typedef struct
{
  uint8_t data[RX_BUFF_SIZE];
  uint8_t start;  //start position of current command
  uint8_t pos;    // current position in rx buffer
  uint8_t end;    // end position, pointing to NULL character
}Rx_buffer_t;

#endif
