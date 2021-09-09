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

#define _GNU_SOURCE 1

#include "OpenThreadCli.h"
#include <openthread/cli.h>
#include <openthread/openthread-freertos.h>
#include <stdarg.h>
#include <stdio.h>

ssize_t OpenThreadCliClass::cookie_write(void *cookie, const char *data, size_t len)
{
    Stream* strm = reinterpret_cast<Stream*>(cookie);
    return strm->write(data, len);
}

int OpenThreadCliClass::outputCallback(void *aContext, const char* aFormat, va_list aArgument)
{
  static const cookie_io_functions_t vtable = { NULL, cookie_write, NULL, NULL };

  FILE* f = fopencookie(aContext, "w", vtable);
  return vfprintf(f, aFormat, aArgument);
}

size_t OpenThreadCliClass::write(uint8_t b)
{
  if(b == '\r' || b == '\n') {
    cmdline_buffer[cmdline_i] = '\0';
    otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "otCliConsoleInputLine %s", cmdline_buffer);

    if(cmdline_i > 0) {
      cliStream->write(b);
      otCliInputLine(reinterpret_cast<char*>(cmdline_buffer));
    }
    cmdline_i = 0;
  }
  else if(b == '\b') {
    if(cmdline_i > 0) cmdline_buffer[--cmdline_i] = '\0';
  }
  else {
    cmdline_buffer[cmdline_i++] = b;
  }
  return 1;
}

int OpenThreadCliClass::available()
{
  return (cliStream ? cliStream->available() : 0);
}
int OpenThreadCliClass::read()
{
  return (cliStream ? cliStream->read() : 0);
}
int OpenThreadCliClass::peek()
{
  return (cmdline_i>0) ? cmdline_buffer[cmdline_i-1] : 0;
}

void OpenThreadCliClass::flush()
{
  if(cliStream) cliStream->flush();
}

void OpenThreadCliClass::begin(Stream& strm, bool echoback, const char* prompt)
{
  otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "OpenThreadCliClass::begin(Stream& strm)");
  cliStream = &strm;
  enableEchoback = echoback;
  promptString = prompt;
  otCliInit(otrGetInstance(), outputCallback, cliStream);
}

void OpenThreadCliClass::process()
{
  while(cliStream && cliStream->available() > 0 ) {
    otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "OpenThreadCliClass::process()");
    char c = cliStream->read();
    write(c);
    if(enableEchoback)            cliStream->write(c);
    if(promptString && c == '\n') cliStream->write(promptString);
  }
}

OpenThreadCliClass OpenThreadCli;
