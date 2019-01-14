#include "system.h"
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
    Serial.print("cnc>>connected");
}




void loop() {
  uint8_t returnedStatus = 0u;

  // reset arduino if connection is lost
  if(!Serial) { performSoftwareReset(); }

  // read from serial until a line is complete
  returnedStatus = readLine(Serial.read(), rx_buffer, RX_BUFF_SIZE);
  if( RETURN_SUCCES != returnedStatus ) { handleRuntimeError(returnedStatus); return; }

  // if buffer is ready (an entire line is recieved)
  if( true == rx_buffer.ready)
  {
    // DEBUG
    Serial.print("1.INPUT: <<"); Serial.print(rx_buffer.data); Serial.print(">>\n");

    // parse line
    returnedStatus = machine.parseLine(rx_buffer);
    if(RETURN_SUCCES != returnedStatus) { handleRuntimeError(returnedStatus); return; }

    // execute machine command
    returnedStatus = machine.executeMovementCommand();
    if(RETURN_SUCCES != returnedStatus) { handleRuntimeError(returnedStatus); return; }

    /* TODO: maybe save some system information */

    // send pozitive feedback to Serial if everything went ok
    memset(&rx_buffer, 0u, sizeof(Rx_buffer_t));
    Serial.print( F("CNC>>>done\n") );
  }

}
