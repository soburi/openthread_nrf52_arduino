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
#include <openthread/commissioner.h>
#include <openthread/icmp6.h>
#include <openthread/joiner.h>
#include <openthread/link.h>
#if OPENTHREAD_CONFIG_ENABLE_TIME_SYNC
#include <openthread/network_time.h>

#include <openthread/dns.h>

#if OPENTHREAD_FTD
#include <openthread/dataset_ftd.h>
#include <openthread/thread_ftd.h>
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
#include <openthread/border_router.h>
#endif
#if OPENTHREAD_CONFIG_SERVICE_ENABLE
#include <openthread/server.h>
#endif
#endif

#include <openthread/heap.h>

#include <openthread/openthread-freertos.h>

#include "Arduino.h"

static void* otCAlloc(size_t n, size_t size)
{
  return memset(pvPortMalloc(n * size), 0, n * size);
}

static void otFree(void *p_ptr)
{
  vPortFree(p_ptr);
}

void OpenThreadClass::init()
{
#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
  otHeapSetCAllocFree(otCAlloc, otFree);
#endif /* OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE */

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

