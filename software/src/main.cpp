#include "utilities.h"
#include "machine.h"


/* Var declaration */
Rx_buffer_t rx_buffer;
Machine machine;




void setup() {
    Serial.begin(BAUD_RATE);
    machine.init();
    memset(&rx_buffer, 0u, sizeof(Rx_buffer_t));

    // wait for serial communication
    while(!Serial) {;}
}



void loop() {
  uint8_t returnedStatus = 0u;

  // read from serial until a line is complete
  returnedStatus = readLine(Serial.read(), rx_buffer, RX_BUFF_SIZE);
  if( RETURN_SUCCES != returnedStatus ) { handleRuntimeError(returnedStatus); return; }

  // if buffer is ready (an entire line is recieved)
  if( true == rx_buffer.ready)
  {
    // parse line
    returnedStatus = machine.parseLine(rx_buffer);
    if(RETURN_SUCCES != returnedStatus) { handleRuntimeError(returnedStatus); return; }

    // execute machine command
    returnedStatus = machine.executeCommand();
    if(RETURN_SUCCES != returnedStatus) { handleRuntimeError(returnedStatus); return; }

    /* TODO: maybe save some system information */

    // send pozitive feedback to Serial if everything went ok
    Serial.print( F("CNC>>>done") );
  }

}
