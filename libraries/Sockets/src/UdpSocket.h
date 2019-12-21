/*
  Copyright (c) 2016-2019 Tokita, Hiroshi  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef UDPSocket_h
#define UDPSocket_h

#include <Arduino.h>
#include <Udp.h>
#include <openthread/udp.h>
#include "ringbuf.h"

class UDPSocket : public UDP {
private:
  otUdpSocket socket;
  otMessageInfo msginfo;

  otIp6Address remote_ip;
  uint16_t remote_port;
  uint16_t remaining;

  char   tx_buffer[1460];
  size_t tx_buffer_len;
  char   rx_buffer[1460];
  size_t rx_buffer_len;

  struct ringbuf rxbuf;

  struct received_packet {
    uint16_t length;
    struct otIp6Address ipaddr;
    uint16_t port;
  };

public:
  UDPSocket();
  ~UDPSocket();
  uint8_t begin(IPAddress a, uint16_t p);
  inline uint8_t begin(uint16_t p) { return begin(IN6ADDR.ANY_INIT, p); }
  void stop();
  int beginPacket(IPAddress ip, uint16_t port);
  int beginPacket(const char *host, uint16_t port);
  int endPacket();
  inline size_t write(uint8_t b) { return write(&b, 1); }
  size_t write(const uint8_t *buffer, size_t size);
  int parsePacket();
  inline int available() { return remaining; }
  inline int read() { uint8_t ch = 0; int err = read(&ch, 1); return err ? err : ch; }
  inline int read(unsigned char* buffer, size_t len) { return read(reinterpret_cast<char*>(buffer), len); }
  int read(char* buffer, size_t len);
  int peek();
  inline void flush() { }
  inline IPAddress remoteIP() { return remote_ip.mFields.m16; }
  inline uint16_t remotePort() { return remote_port; }
  using Print::write; // pull in write(str) and write(buf, size) from Print

private:
  void onReceived(otMessage *aMessage, const otMessageInfo *aMessageInfo);
  static inline void receive_handler(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo) {
    reinterpret_cast<UDPSocket*>(aContext)->onReceived(aMessage, aMessageInfo);
  }
};

#endif /* UDPSocket_h */
