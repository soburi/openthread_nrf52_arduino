/*
  Server.cpp - Server class for Raspberry Pi
  Copyright (c) 2016 Hristo Gochkov  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <memory>
#include "ServerSocket.h"
#undef EDEADLK
#undef ENAMETOOLONG
#undef ENOLCK
#undef ENOSYS
#undef ENOTEMPTY
#undef ELOOP
#undef ENOMSG
#undef EIDRM
#undef EMULTIHOP
#undef EBADMSG
#undef EOVERFLOW
#undef EILSEQ
#undef ENOTSOCK
#undef EDESTADDRREQ
#undef EMSGSIZE
#undef EPROTOTYPE
#undef ENOPROTOOPT
#undef EPROTONOSUPPORT
#undef EAFNOSUPPORT
#undef EADDRINUSE
#undef EADDRNOTAVAIL
#undef ENETDOWN
#undef ENETUNREACH
#undef ENETRESET
#undef ECONNABORTED
#undef EISCONN
#undef ENOTCONN
#undef ETOOMANYREFS
#undef ETIMEDOUT
#undef EHOSTDOWN
#undef EHOSTUNREACH
#undef EALREADY
#undef EINPROGRESS
#undef ESTALE
#undef EDQUOT

#undef errno

#include <lwip/sockets.h>
#include <lwip/netdb.h>

#undef write
#undef close

int ServerSocket::setTimeout(uint32_t seconds){
  struct timeval tv;
  tv.tv_sec = seconds;
  tv.tv_usec = 0;
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
    return -1;
  return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

size_t ServerSocket::write(const uint8_t *data, size_t len){
  return 0;
}

void ServerSocket::stopAll(){}

ClientSocket ServerSocket::available(){
  if(!_listening)
    return ClientSocket();
  int client_sock;
  if (_accepted_sockfd >= 0) {
    client_sock = _accepted_sockfd;
    _accepted_sockfd = -1;
  }
  else {
  struct sockaddr_in _client;
  int cs = sizeof(struct sockaddr_in);
    client_sock = lwip_accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
  }
  if(client_sock >= 0){
    int val = 1;
    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == ERR_OK) {
      val = _noDelay;
      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == ERR_OK)
        return ClientSocket(client_sock);
    }
  }
  return ClientSocket();
}

void ServerSocket::begin(uint16_t port){
  if(_listening)
    return;
  if(port){
      _port = port;
  }
  struct sockaddr_in server;
  sockfd = socket(AF_INET , SOCK_STREAM, 0);
  if (sockfd < 0)
    return;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(_port);
  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    return;
  if(listen(sockfd , _max_clients) < 0)
    return;
  lwip_fcntl(sockfd, F_SETFL, O_NONBLOCK);
  _listening = true;
  _noDelay = false;
  _accepted_sockfd = -1;
}

void ServerSocket::setNoDelay(bool nodelay) {
    _noDelay = nodelay;
}

bool ServerSocket::getNoDelay() {
    return _noDelay;
}

bool ServerSocket::hasClient() {
    if (_accepted_sockfd >= 0) {
      return true;
    }
    struct sockaddr_in _client;
    int cs = sizeof(struct sockaddr_in);
    _accepted_sockfd = lwip_accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    if (_accepted_sockfd >= 0) {
      return true;
    }
    return false;
}

void ServerSocket::end(){
  lwip_close(sockfd);
  sockfd = -1;
  _listening = false;
}

void ServerSocket::close(){
  end();
}

void ServerSocket::stop(){
  end();
}

