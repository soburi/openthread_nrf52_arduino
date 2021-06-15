/*
  Copyright (c) 2020-2021 Tokita, Hiroshi  All right reserved.

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

#include <Arduino.h>
#include <Stream.h>
#include <RingBuffer.h>

#define OPENTHREAD_CONFIG_CLI_UART_RX_BUFFER_SIZE 1024

class OpenThreadCliClass : public Stream {
  Stream* cliStream;
  uint8_t cmdline_buffer[OPENTHREAD_CONFIG_CLI_UART_RX_BUFFER_SIZE];
  int cmdline_i;
  bool enableEchoback;
  const char* promptString;

public:
  void begin(Stream& strm, bool enable_echoback=true, const char* prompt="> ");
  size_t write(uint8_t);
  int available();
  int read();
  int peek();
  void flush();
  void process();

private:
  static int consoleCallback(const char *aBuf, uint16_t aBufLength, void *aContext);
};

extern OpenThreadCliClass OpenThreadCli;
