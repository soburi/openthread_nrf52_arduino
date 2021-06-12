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

#include "OpenThreadCli.h"
#include <openthread/cli.h>
#include <openthread/openthread-freertos.h>

int OpenThreadCliClass::consoleCallback(const char *aBuf, uint16_t aBufLength, void *aContext)
{
  otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "OpenThreadCliClass::consoleCallback");
  char buf[128];
  strncpy(buf, aBuf, aBufLength < 127 ? aBufLength : 127);
  buf[aBufLength] = 0;
  otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "%s", buf);

  Stream* strm = reinterpret_cast<Stream*>(aContext);
  return (strm) ? strm->write(aBuf, aBufLength) : aBufLength;
}

size_t OpenThreadCliClass::write(uint8_t b)
{
  if(b == '\r' || b == '\n') {
    cmdline_buffer[cmdline_i] = '\0';
    otPlatLog(OT_LOG_LEVEL_INFO, OT_LOG_REGION_PLATFORM, "otCliConsoleInputLine %s", cmdline_buffer);

    if(cmdline_i > 0) {
      cliStream->write(b);
      otCliConsoleInputLine(reinterpret_cast<char*>(cmdline_buffer), cmdline_i);
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
  otCliConsoleInit(otrGetInstance(), consoleCallback, cliStream);
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
