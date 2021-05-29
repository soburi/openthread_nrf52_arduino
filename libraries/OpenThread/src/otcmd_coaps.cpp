/*
  Copyright (c) 2021 Tokita, Hiroshi  All right reserved.

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

extern "C" {
#include <openthread/openthread-freertos.h>
}


otError OTCMD::OTCoapSecure::_request(otCoapCode coapCode, const otIp6Address* dest, char const* url, otCoapType coapType, uint8_t* data, uint16_t datalen, otCoapResponseHandler handler, void* context)
{
  otError       error   = OT_ERROR_NONE;
  otMessage *   message = NULL;
  otMessageInfo messageInfo;

  message = otCoapNewMessage(otrGetInstance(), NULL);
  VerifyOrExit(message != NULL, error = OT_ERROR_NO_BUFS);

  otCoapMessageInit(message, coapType, coapCode);
  otCoapMessageGenerateToken(message, 2);

  if(url) {
    error = otCoapMessageAppendUriPathOptions(message, url);
  }

  if(data && datalen > 0) {
    error = otCoapMessageSetPayloadMarker(message);
    error = otMessageAppend(message, data, datalen);
  }

  memset(&messageInfo, 0, sizeof(messageInfo));
  messageInfo.mPeerAddr = *dest;
  messageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;
  error = otCoapSecureSendRequest(otrGetInstance(), message, handler, context);

exit:
  if ((error != OT_ERROR_NONE) && (message != NULL))
  {
    otMessageFree(message);
  }
  return error;
}



otError OTCMD::OTCoapSecure::request_get(const otIp6Address* dest, char const* url, otCoapType coapType, uint8_t* data, uint16_t datalen, otCoapResponseHandler handler, void* context)
{
  return _request(OT_COAP_CODE_GET, dest, url, coapType, data, datalen, handler, context);
}

otError OTCMD::OTCoapSecure::request_post(const otIp6Address* dest, char const* url, otCoapType coapType, uint8_t* data, uint16_t datalen, otCoapResponseHandler handler, void* context)
{
  return _request(OT_COAP_CODE_POST, dest, url, coapType, data, datalen, handler, context);
}

otError OTCMD::OTCoapSecure::request_put(const otIp6Address* dest, char const* url, otCoapType coapType, uint8_t* data, uint16_t datalen, otCoapResponseHandler handler, void* context)
{
  return _request(OT_COAP_CODE_PUT, dest, url, coapType, data, datalen, handler, context);
}

otError OTCMD::OTCoapSecure::request_delete(const otIp6Address* dest, char const* url, otCoapType coapType, uint8_t* data, uint16_t datalen, otCoapResponseHandler handler, void* context)
{
  return _request(OT_COAP_CODE_DELETE, dest, url, coapType, data, datalen, handler, context);
}

otError OTCMD::OTCoapSecure::start()
{
  return otCoapSecureStart(otrGetInstance(), OT_DEFAULT_COAP_PORT);
}

void OTCMD::OTCoapSecure::stop()
{
  otCoapSecureStop(otrGetInstance());
}

otError OTCMD::OTCoapSecure::connect(const otIp6Address* addr, uint16_t port, otHandleCoapSecureClientConnect handler, void* context)
{
  otSockAddr sockaddr;
  sockaddr.mAddress = *addr;
  sockaddr.mPort = port;
  return otCoapSecureConnect(otrGetInstance(), &sockaddr, handler, context);
}

void OTCMD::OTCoapSecure::disconnect()
{
  otCoapSecureDisconnect(otrGetInstance());
}

otError OTCMD::OTCoapSecure::resource(const char* url, otCoapRequestHandler handler, void* context)
{
  mResource.mUriPath = url;
  mResource.mContext = context;
  mResource.mHandler = handler;

  return otCoapSecureAddResource(otrGetInstance(), &mResource);
}

