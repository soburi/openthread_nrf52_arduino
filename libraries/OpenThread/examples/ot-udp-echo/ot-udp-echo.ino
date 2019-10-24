/*
  UDP Echo

  A simple UDP communication demo.

  created 28 Oct 2019
  by Tokita Hiroshi
*/

#include <UdpSocket.h>
#include <IPAddress.h>
#include <OpenThread.h>

const uint8_t CHANNEL = 15;
const uint16_t PANID = 0x1234;
const char PSK[] = "J01NME";
const uint8_t EXTPANID[] = {0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22};
IPAddress server(0xfd11, 0x0022, 0x0000, 0x0000, 0xc3bb, 0x92c9, 0x38e9, 0xa9f8); // Configure server manually.

UDPSocket Udp;

const size_t MAX_PAYLOAD_LEN = 40;
const unsigned short DEST_PORT  = 7;
const int INTERVAL = 5;

String message;
char recvBuffer[MAX_PAYLOAD_LEN];
long lastsend = INTERVAL * -1000;
uint16_t seq_id = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Start udp-echo-cleint");
  OpenThread.begin();

  OpenThread.channel(CHANNEL);
  OpenThread.panid(PANID);
  OpenThread.extpanid(EXTPANID);
  OpenThread.ifconfig(true);

  Serial.print("Channel = ");
  Serial.println(OpenThread.channel() );
  Serial.print("PANID = 0x");
  Serial.print(OpenThread.panid(), HEX);
  Serial.println();
  Serial.print("Extend PANID = 0x");
  Serial.println(OpenThread.extpanid() );
  Serial.print("EUI64 = ");
  Serial.println(OpenThread.eui64());

  OTErr err;
  do {
    err = OpenThread.joiner_start(PSK);

    if (err) {
      Serial.print("Thread join failed: ");
      Serial.println(err);
      continue;
    }

    err = OpenThread.thread(true);

    if(err) {
      Serial.print("Thread process can't start: ");
      Serial.println(err);
      continue;
    }
  } while(err != 0);
}

void loop() {
  long now = millis();

  // Periodically send.
  if ((now - lastsend) > (INTERVAL * 1000)) {
    // format message
    message = "Hello ";
    message.concat(++seq_id);

    Serial.print("      Send to [");
    Serial.print(server);
    Serial.printf("]:%d -> '", DEST_PORT);
    Serial.print(message);
    Serial.println("'");

    // send packet
    Udp.beginPacket(server, DEST_PORT);
    Udp.write(message.c_str(), message.length() );
    Udp.endPacket();

    lastsend = now;
  }

  while (int packetSize = Udp.parsePacket()) {
    // read the packet into packetBufffer
    Serial.print("Response from [");
    Serial.print(Udp.remoteIP());
    Serial.printf("]:%d <- '", Udp.remotePort());
    Udp.read(recvBuffer, packetSize);
    Serial.print(recvBuffer);
    Serial.println("'");
  }

  delay(100);
}
