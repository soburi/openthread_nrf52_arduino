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

#pragma once

#include <Print.h>

#include "OpenThread_types.hpp"
#include "OpenThread_cmds.hpp"
#include "otapi.hpp"

#define OTAPI_PACKAGE_NAME "OpenThread"
#define OTAPI_PACKAGE_VERSION "1.0.0"

class OpenThreadClass : public OpenThread_cmds {
public:

  class Api {
  public:

#if (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
    OTAPI::BackboneRouter BackboneRouter;
#endif
#if OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
    OTAPI::BorderAgent BorderAgent;
#endif
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
    OTAPI::BorderRouter BorderRouter;
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD
    OTAPI::ChannelManager ChannelManager;
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
    OTAPI::ChannelMonitor ChannelMonitor;
#endif
#if OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE
    OTAPI::ChildSupervision ChildSupervision;
#endif
#if OPENTHREAD_CONFIG_COAP_API_ENABLE
    OTAPI::Coap Coap;
#endif
#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE
    OTAPI::CoapSecure CoapSecure;
#endif
#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE && OPENTHREAD_FTD
    OTAPI::Commissioner Commissioner;
#endif
    OTAPI::Crypto Crypto;
    OTAPI::Dataset Dataset;
#if OPENTHREAD_CONFIG_DIAG_ENABLE
    OTAPI::Diag Diag;
#endif
#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
    OTAPI::DnsClient DnsClient;
#endif

    OTAPI::Entropy Entropy;
    OTAPI::Heap Heap;
    OTAPI::Icmp6 Icmp6;
    OTAPI::Instance Instance;
    OTAPI::Ip6 Ip6;

#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
    OTAPI::JamDetection JamDetection;
#endif
#if OPENTHREAD_CONFIG_JOINER_ENABLE
    OTAPI::Joiner Joiner;
#endif

    OTAPI::Link Link;

#if OPENTHREAD_CONFIG_LINK_RAW_ENABLE || OPENTHREAD_RADIO 
    OTAPI::LinkRaw LinkRaw;
#endif

    OTAPI::Logging Logging;
    OTAPI::Message Message;
    OTAPI::NetData NetData;

#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
    OTAPI::NetworkTime NetworkTime;
#endif

    OTAPI::Plat Plat;
    OTAPI::RandomCrypto RandomCrypto;
    OTAPI::RandomNonCrypto RandomNonCrypto;

#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
    OTAPI::Server Server;
#endif
#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
    OTAPI::SntpClient SntpClient;
#endif

    OTAPI::Tasklets Tasklets;
    OTAPI::Thread Thread;
    OTAPI::Udp Udp;

    inline const char* VersionString() { return otGetVersionString(); }
    inline const char* RadioVersionString() { return otGetRadioVersionString(otrGetInstance()); }
    inline const char* otErrorToString(otError err) { return otThreadErrorToString(err); }
    inline otError SetStateChangedCallback(otStateChangedCallback cb, void* user) {
      return otSetStateChangedCallback(otrGetInstance(), cb, user);
    }
    inline void RemoveStateChangeCallback(otStateChangedCallback cb, void* user) {
      otRemoveStateChangeCallback(otrGetInstance(), cb, user);
    }
  };

  void init();
  int begin();

  static Api api;

private:
};

extern OpenThreadClass OpenThread;
