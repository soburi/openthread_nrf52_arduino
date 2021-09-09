/*
  Copyright (c) 2019-2021 Tokita, Hiroshi  All right reserved.

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

#include "OpenThread.h"

#include <Print.h>

#include <openthread/config.h>

#include "Arduino.h"

#if defined(ARDUINO_NRF52_ADAFRUIT)
#include <Adafruit_TinyUSB.h> // for Serial
#endif

void OpenThreadClass::init()
{
  otrInit(0, NULL);
  //otCliUartInit(otrGetInstance());
  otrUserInit();
}

int OpenThreadClass::begin()
{
  otrStart();
  return 1;
}

OpenThreadClass OpenThread;

