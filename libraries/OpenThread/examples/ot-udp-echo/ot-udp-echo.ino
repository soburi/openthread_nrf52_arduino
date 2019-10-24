/*
  UDP Echo

  A simple UDP communication demo.

  created 24 Jan 2016
  modified 29 May 2018
  by Tokita Hiroshi
*/

#include <UdpSocket.h>
#include <IPAddress.h>
#include <OpenThread.h>

#define CHANNEL 26
#define PANID   0xABCD
#define PSK     "J01NME"
#define EXTPANID {0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22}

const size_t MAX_PAYLOAD_LEN = 40;
const unsigned short LOCAL_PORT = 7;
const unsigned short DEST_PORT  = 7;
const int INTERVAL = 15;

UDPSocket Udp;
IPAddress server(0xfd11, 0x1111, 0x1122, 0x0000, 0x3b2, 0x587, 0x9759, 0x6504); // Configure server manually.

String message;
char recvBuffer[MAX_PAYLOAD_LEN];
long lastsend;
uint16_t seq_id;

extern "C" void udp_init();

void setup() {
  udp_init();
  Serial.begin(115200);
  Serial.println("Start udp-echo-cleint");
  OpenThread.begin();

  uint8_t epid[] = EXTPANID;
  OpenThread.extpanid(epid);
  OpenThread.ifconfig(true);

  Serial.println("Start udp-echo-cleint");
  OTErr err = OpenThread.joiner_start(PSK);
  Serial.println(err);

  if (err) {
    Serial.print("Thread join failed: ");
    Serial.println(err);
  }

  err = OpenThread.thread(true);

  if(err) {
    Serial.print("Thread process can't start: ");
    Serial.println(err);
  }

  Serial.print("Server is [");
  Serial.print(server);
  Serial.println("]");

  //Serial.print("Client is [");
  //Serial.print(OpenThread.globalAddress());
  //Serial.println("]");

  Udp.begin(LOCAL_PORT);
  Serial.print("Start listen port:");
  Serial.println(LOCAL_PORT);
}

void loop() {

  long now = millis();

  // Periodically send.
  if ((now - lastsend) > (INTERVAL * 1000)) {
    // format message
    message = "Hello ";
    message.concat(++seq_id);

    Serial.print("Send to [");
    Serial.print(server);
    Serial.print("]: '");
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
    Udp.read(recvBuffer, packetSize);
    Serial.print("Response from [");
    Serial.print(Udp.remoteIP());
    Serial.print("]: '");
    Serial.print(recvBuffer);
    Serial.println("'");
  }

}
