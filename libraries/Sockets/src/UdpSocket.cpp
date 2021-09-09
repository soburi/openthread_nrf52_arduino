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

#include "UdpSocket.h"
#include <openthread/udp.h>
#include <openthread/instance.h>
#include <openthread/openthread-freertos.h>

#define log_e(...) ADALOG("UDPSKT", __VA_ARGS__)

UDPSocket::UDPSocket(){
  ringbuf_init(&rxbuf, rx_buffer, sizeof(rx_buffer) );
}

UDPSocket::~UDPSocket(){
   stop();
}

static int _begin(otUdpSocket* socket, otSockAddr* addr, otUdpReceive handler, void* context) {
  otError err = OT_ERROR_NONE;

  OT_API_CALL(err = otUdpClose(otrGetInstance(), socket));
  if(err != OT_ERROR_NONE) {
    log_e("could not close socket: %s", otThreadErrorToString(err));
    return 0;
  }

  socket->mContext = NULL;

  OT_API_CALL(err = otUdpOpen(otrGetInstance(), socket, handler, context));
  if(err != OT_ERROR_NONE) {
    log_e("could not create socket: %s", otThreadErrorToString(err));
    return 0;
  }

  OT_API_CALL(err = otUdpBind(otrGetInstance(), socket, addr));
  if(err != OT_ERROR_NONE) {
    log_e("could not bind socket: %s", otThreadErrorToString(err));
    OT_API_CALL(otUdpClose(otrGetInstance(), socket));
    socket->mContext = NULL;
    return 0;
  }
  return 1;
}

uint8_t UDPSocket::begin(IPAddress address, uint16_t port){
  int ret = 0;
  struct otSockAddr addr = {0};
  addr.mPort = port;
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  COPY_V6ADDR(addr.mAddress.mFields.m16, address.v6);
#pragma GCC diagnostic pop
  //scope id

  tx_buffer_len = 0;
  OT_API_CALL(ret = _begin(&socket, &addr, receive_handler, this) );

  return ret;
}

void UDPSocket::stop(){
  otError err = OT_ERROR_NONE;
  OT_API_CALL(err = otUdpClose(otrGetInstance(), &socket));
  if(err != OT_ERROR_NONE) {
    log_e("could not close socket: %s", otThreadErrorToString(err));
  }
  socket.mContext = NULL;
  tx_buffer_len = 0;
}

int UDPSocket::beginPacket(IPAddress ip, uint16_t port){
  if(!port) return 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  COPY_V6ADDR(msginfo.mPeerAddr.mFields.m16, ip.v6);
#pragma GCC diagnostic pop
  msginfo.mPeerPort = port;

  tx_buffer_len = 0;
  return 1;
}

int UDPSocket::beginPacket(const char *host, uint16_t port){
/* TODO
  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL){
    log_e("could not get host from dns: %d", errno);
    return 0;
  }
  return beginPacket(IPAddress((const uint16_t *)(server->h_addr_list[0])), port);
*/
  return 0;
}

static int _endPacket(otUdpSocket* socket, const char* buf, size_t buflen, otMessageInfo* msginfo) {
  otError err = OT_ERROR_NONE;

  otMessage* msg = otUdpNewMessage(otrGetInstance(), NULL);
  if(!msg) {
    log_e("could not allocate message: ");
    return 0;
  }

  err = otMessageAppend(msg, buf, buflen);
  if(err != OT_ERROR_NONE) {
    log_e("could not append data to message: %s", otThreadErrorToString(err));
    if(msg != NULL) otMessageFree(msg);
    return 0;
  }

  OT_API_CALL(err = otUdpSend(otrGetInstance(), socket, msg, msginfo));
  if(err != OT_ERROR_NONE){
    log_e("could not send data: %s", otThreadErrorToString(err));
    if(msg != NULL) otMessageFree(msg);
    return 0;
  }

  return 1;
}

int UDPSocket::endPacket(){
  int ret = 0;
  OT_API_CALL(ret = _endPacket(&socket, tx_buffer, tx_buffer_len, &msginfo) );
  return ret;
}

int UDPSocket::parsePacket() {
  
  if(ringbuf_elements(&rxbuf) < (sizeof(struct received_packet)-remaining) ) {
    return 0;
  }

  // discard old packet
  while(remaining) {
    read();
  }

  struct received_packet pack;

  for(uint32_t i=0; i<sizeof(struct received_packet); i++) {
    reinterpret_cast<char*>(&pack)[i] = static_cast<char>(ringbuf_get(&rxbuf));
  }

  remaining   = pack.length - sizeof(struct received_packet);
  remote_port = pack.port;
  remote_ip   = pack.ipaddr;
  
  return remaining;
}

void UDPSocket::onReceived(otMessage *aMessage, const otMessageInfo *aMessageInfo) {
  const uint16_t msg_len = otMessageGetLength(aMessage);
  const uint16_t store_size = static_cast<uint16_t>(sizeof(struct received_packet) + msg_len);
  struct received_packet pack = { store_size, aMessageInfo->mPeerAddr, aMessageInfo->mPeerPort };
  //log_e("UDPSocket::receive %d %d %d\n", sizeof(struct received_packet), store_size, msg_len );

  if( store_size > (ringbuf_size(&rxbuf)-remaining) ) {
    //too large packet.
    return;
  }

  //log_e("UDPSocket::receive ringbuf_size() %d ringbuf_elements() %d\n", ringbuf_size(&rxbuf) , ringbuf_elements(&rxbuf) );
  while( store_size < ringbuf_elements(&rxbuf) ) {
    // discard old packet.
    parsePacket();
  }

  for(uint32_t i=0; i<sizeof(struct received_packet); i++) {
    int r = ringbuf_put(&rxbuf, ((uint8_t*)&pack)[i]);
    if(r == 0) {
      break;
    }
  }

  for(int i=0; i<msg_len; i++) {
    char readbuf = 0;
    otMessageRead(aMessage, i, &readbuf, 1);
    int r = ringbuf_put(&rxbuf, readbuf);
    if(r == 0) {
      break;
    }
  }
  //log_e("UDPSocket::receive ringbuf_size() %d ringbuf_elements() %d\n", ringbuf_size(&rxbuf) , ringbuf_elements(&rxbuf) );
}

size_t UDPSocket::write(const uint8_t *buffer, size_t size){
  size_t i;
  for(i=0; (i<size) && (tx_buffer_len<sizeof(tx_buffer)); i++) {
    tx_buffer[tx_buffer_len++] = buffer[i];
  }
  return i;
}

int UDPSocket::read(char* buffer, size_t len){
  size_t toread = (remaining < len ? remaining : len);
  for(size_t i=0; i<toread; i++) {
    int r = ringbuf_get(&rxbuf);
    buffer[i] = static_cast<char>(r);
    remaining--;
  }
  return toread;
}

int UDPSocket::peek(){
  return ringbuf_peek(&rxbuf);
}

