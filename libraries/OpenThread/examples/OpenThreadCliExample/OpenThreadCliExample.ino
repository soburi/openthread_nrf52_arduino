/*
  OpenThread Cli

  Running OpenThread-CLI on Serial.

  created 2 Feb 2020
  by Tokita Hiroshi
*/

#include <OpenThread.h>
#include <OpenThreadCli.h>

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  Serial.println("-------------------------");
  Serial.println("---- OpenThread CLI -----");
  Serial.println("-------------------------");
  Serial.println();
  
  OpenThread.begin();

  // Bind CLI and Serial.
  OpenThreadCli.begin(Serial);
}

void loop() {
  // Nothing to do here, handled in serialEvent.
}

void serialEvent() {
  // Redirect data received on serial to CLI,
  // works like this code.
  //  
  // while(Serial.available()) {
  //   OpenThreadCli.write(Serial.read());
  // }
  
  OpenThreadCli.process();
}
