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

#include <Arduino.h>

#include <openthread/config.h>
#include <openthread/commissioner.h>
#include <openthread/icmp6.h>
#include <openthread/joiner.h>
#include <openthread/link.h>
#include <openthread/network_time.h>

#include <openthread/dns.h>

#if OPENTHREAD_FTD
#include <openthread/dataset_ftd.h>
#include <openthread/thread_ftd.h>
#endif

#include <openthread/border_router.h>
#include <openthread/server.h>

#include <openthread/heap.h>

extern "C" {
#include <openthread/openthread-freertos.h>
}

#define OT_CALL_FUNC0(cls, fn) ot##cls##fn (otrGetInstance())
#define OT_CALL_FUNC1(cls, fn) ot##cls##fn (otrGetInstance(), a1)
#define OT_CALL_FUNC2(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2)
#define OT_CALL_FUNC3(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3)
#define OT_CALL_FUNC4(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4)
#define OT_CALL_FUNC5(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5)
#define OT_CALL_FUNC6(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6)
#define OT_CALL_FUNC7(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6, a7)
#define OT_CALL_FUNC8(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6, a7, a8)

#define OT_IS_DECL(ty, na, cat, n2)  ty na ();
#define OT_GETTER_DECL(ty, na, cat, n2)  ty na ();
#define OT_SETTER_DECL(ty, na, cat, n2)  otError na (ty arg);
#define OT_V_SETTER_DECL(ty, na, cat, n2)  void na (ty arg);

#define OT_SETGET_DECL(ty, na, cat, n2)  \
  OT_SETTER_DECL(ty, na, cat, n2); \
  OT_GETTER_DECL(ty, na, cat, n2) \

#define OT_SET_IS_DECL(ty, na, cat, n2)  \
  OT_SETTER_DECL(ty, na, cat, n2); \
  OT_IS_DECL(ty, na, cat, n2) \

#define OT_V_SETGET_DECL(ty, na, cat, n2)  \
  OT_V_SETTER_DECL(ty, na, cat, n2); \
  OT_GETTER_DECL(ty, na, cat, n2) \

#define OT_V_SET_IS_DECL(ty, na, cat, n2)  \
  OT_V_SETTER_DECL(ty, na, cat, n2); \
  OT_IS_DECL(ty, na, cat, n2) \

#define OT_V_FUNC_0_DECL(na, cat, n2) \
  void na()

#define OT_FUNC_0_DECL(ty, na, cat, n2) \
  ty na()

#define OT_V_FUNC_1_DECL(na, cat, n2, t1) \
  void na(t1)

#define OT_FUNC_1_DECL(ty, na, cat, n2, t1) \
  ty na(t1)

#define OT_FUNC_2_DECL(ty, na, cat, n2, t1, t2) \
  ty na(t1, t2)

#define OT_FUNC_3_DECL(ty, na, cat, n2, t1, t2, t3) \
  ty na(t1, t2, t3)

#define OT_FUNC_4_DECL(ty, na, cat, n2, t1, t2, t3, t4) \
  ty na(t1, t2, t3, t4)


#define OT_GETTER_IMPL(ty, na, cat, n2)  \
ty OTCMD::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, Get ## n2) ); \
}

#define OT_V_FUNC_0_IMPL(na, cat, n2)  \
void OTCMD::  na  () \
{ \
  OT_API_CALL( OT_CALL_FUNC0(cat, n2) ); \
}

#define OT_FUNC_0_IMPL(ty, na, cat, n2)  \
ty OTCMD::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, n2) ); \
}

#define OT_V_FUNC_1_IMPL(na, cat, n2, t1)  \
void OTCMD::  na  (t1 a1) \
{ \
  OT_API_CALL( OT_CALL_FUNC1(cat, n2) ); \
}

#define OT_FUNC_1_IMPL(ty, na, cat, n2, t1)  \
ty OTCMD::  na  (t1 a1) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC1(cat, n2) ); \
}

#define OT_FUNC_2_IMPL(ty, na, cat, n2, t1, t2)  \
ty OTCMD::  na  (t1 a1, t2 a2) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC2(cat, n2) ); \
}

#define OT_FUNC_3_IMPL(ty, na, cat, n2, t1, t2, t3)  \
ty OTCMD::  na  (t1 a1, t2 a2, t3 a3) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC3(cat, n2) ); \
}

#define OT_FUNC_4_IMPL(ty, na, cat, n2, t1, t2, t3, t4)  \
ty OTCMD::  na  (t1 a1, t2 a2, t3 a3, t4 a4) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC4(cat, n2) ); \
}

#define OT_FUNC_5_IMPL(ty, na, cat, n2, t1, t2, t3, t4, t5)  \
ty OTCMD::  na  (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC5(cat, n2) ); \
}

#define OT_FUNC_6_IMPL(ty, na, cat, n2, t1, t2, t3, t4, t5, t6)  \
ty OTCMD::  na  (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC6(cat, n2) ); \
}

#define OT_FUNC_7_IMPL(ty, na, cat, n2, t1, t2, t3, t4, t5, t6, t7)  \
ty OTCMD::  na  (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC7(cat, n2) ); \
}

#define OT_IS_IMPL(ty, na, cat, n2)  \
ty OTCMD::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, Is ## n2) ); \
}

#define OT_SETTER_IMPL(ty, na, cat, n2)  \
otError OTCMD:: na (ty a1) \
{ \
  OT_API_CALL_RET(otError, OT_CALL_FUNC1(cat, Set ## n2) ); \
}

#define OT_V_SETTER_IMPL(ty, na, cat, n2)  \
void OTCMD:: na (ty a1) \
{ \
  OT_API_CALL( OT_CALL_FUNC1(cat, Set ## n2) ); \
}

#define OT_SETGET_IMPL(ty, na, cat, n2)  \
  OT_GETTER_IMPL(ty, na, cat, n2) \
  OT_SETTER_IMPL(ty, na, cat, n2)

#define OT_SET_IS_IMPL(ty, na, cat, n2)  \
  OT_IS_IMPL(ty, na, cat, n2) \
  OT_SETTER_IMPL(ty, na, cat, n2)

#define OT_V_SETGET_IMPL(ty, na, cat, n2)  \
  OT_GETTER_IMPL(ty, na, cat, n2) \
  OT_V_SETTER_IMPL(ty, na, cat, n2)

#define OT_V_SET_IS_IMPL(ty, na, cat, n2)  \
  OT_IS_IMPL(ty, na, cat, n2) \
  OT_V_SETTER_IMPL(ty, na, cat, n2)

#define OT_COPY_IP6(ip6, addr) {\
    ip6.mFields.m16[0] = addr.v6[0]; \
    ip6.mFields.m16[1] = addr.v6[1]; \
    ip6.mFields.m16[2] = addr.v6[2]; \
    ip6.mFields.m16[3] = addr.v6[3]; \
    ip6.mFields.m16[4] = addr.v6[4]; \
    ip6.mFields.m16[5] = addr.v6[5]; \
    ip6.mFields.m16[6] = addr.v6[6]; \
    ip6.mFields.m16[7] = addr.v6[7]; \
}

// x help
OT_V_FUNC_1_IMPL(bufferinfo, Message, GetBufferInfo, otBufferInfo*);

#if (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
//otError bbr(otBackboneRouterConfig&);
#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
OT_V_SETTER_IMPL(bool, _bbr, BackboneRouter, Enabled);
void bbr_disable() { _bbr(false); }
void bbr_enable() { _bbr(true); }
OT_V_SETGET_IMPL(uint8_t, bbr_jitter, BackboneRouter, RegistrationJitter);
//otError bbr_register();
//otBackboneRouterState bbr_state();
//otError bbr_config(uint8_t, uint16_t, uint32_t);
#endif // OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
#endif // (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)

OT_SETGET_IMPL(uint8_t, channel, Link, Channel);
OT_GETTER_IMPL(uint32_t, channel_supported, PlatRadio, SupportedChannelMask);
OT_GETTER_IMPL(uint32_t, channel_preferred, PlatRadio, PreferredChannelMask);
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
//TODO otError channel_monitor();
//otError channel_monitor_start();
//otError channel_monitor_stop();
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD
//otError channel_manager();
//otError channel_manager_change(uint8_t);
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
//otError channel_manager_select(bool);
#endif
//otError channel_manager_auto(bool);
//otError channel_manager_delay(uint8_t);
//otError channel_manager_interval(uint32_t);
//otError channel_manager_supported(uint32_t);
//otError channel_manager_favored(uint32_t);
#endif



#if OPENTHREAD_FTD
OTChildIterator OTCMD::child()
{
  return 0;
}

OTChildIpIterator OTCMD::childip(uint16_t index)
{
  return OTChildIpIterator(index);
}
OT_GETTER_IMPL(uint8_t, childip_max, Thread, MaxChildIpAddresses);
#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
OT_SETTER_IMPL(uint8_t, childip_max, Thread, MaxChildIpAddresses);
#endif
OT_SETGET_IMPL(uint8_t, childmax, Thread, MaxAllowedChildren);
#endif
OT_V_SETGET_IMPL(uint32_t, childtimeout, Thread, ChildTimeout);
// x coap
// x coaps


#if OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
//TODO	coex
//???	coex_enable
//???	coex_disable
//???	coex_metrics
#endif // OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE

#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE && OPENTHREAD_FTD
OT_GETTER_IMPL(otCommissionerState, commissioner, Commissioner, State);
OT_FUNC_3_IMPL(otError, commissioner_start, Commissioner, Start, otCommissionerStateCallback, otCommissionerJoinerCallback, void*);
OT_FUNC_0_IMPL(otError, commissioner_stop, Commissioner, Stop);
OT_SETTER_IMPL(const char*, commissioner_provisioningurl, Commissioner, ProvisioningUrl);
OT_FUNC_4_IMPL(otError, commissioner_announce, Commissioner, AnnounceBegin, uint32_t, uint8_t, uint16_t, const otIp6Address*);
OT_FUNC_7_IMPL(otError, commissioner_energy, Commissioner, EnergyScan, uint32_t, uint8_t, uint16_t, uint16_t, const otIp6Address*, otCommissionerEnergyReportCallback, void*);
OT_FUNC_5_IMPL(otError, commissioner_panid, Commissioner, PanIdQuery, uint16_t, uint32_t, const otIp6Address*, otCommissionerPanIdConflictCallback, void*);
OT_GETTER_IMPL(uint16_t, commissioner_sessionid, Commissioner, SessionId);
OT_FUNC_3_IMPL(otError, commissioner_joiner_add, Commissioner, AddJoiner, const otExtAddress*, const char*, uint32_t);
OT_FUNC_1_IMPL(otError, commissioner_joiner_remove, Commissioner, RemoveJoiner, const otExtAddress*);
#endif

#if OPENTHREAD_FTD
OT_V_SETGET_IMPL(uint32_t, contextreusedelay, Thread, ContextIdReuseDelay);
#endif

OTMacCounters OTCMD::counters_mac()
{
  const otMacCounters* counter;
  OT_API_CALL(counter = otLinkGetCounters(otrGetInstance()) );
  return counter;
}

OT_V_FUNC_0_IMPL(counters_mac_reset, Link, ResetCounters);

OTMleCounters OTCMD::counters_mle()
{
  const otMleCounters* counter;
  OT_API_CALL(counter = otThreadGetMleCounters(otrGetInstance()) );
  return counter;
}

OT_V_FUNC_0_IMPL(counters_mle_reset, Thread, ResetMleCounters);

// x dataset

#if OPENTHREAD_FTD
OT_SETGET_IMPL(uint32_t, delaytimermin, Dataset, DelayTimerMinimal);
#endif
OT_FUNC_4_IMPL(otError, diag, Diag, ProcessCmd, uint8_t, char**, char*, size_t);
//otError OTCMD::discover(uint32_t, otHandleActiveScanResult, void*);
//otError OTCMD::discover(otActiveScanResult*, size_t, uint32_t);
#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
otError OTCMD::dns_resolve(const char* hostname, otDnsAddressCallback handler, void* context, const otDnsQueryConfig* config);
#endif
#if OPENTHREAD_FTD
OTEidCacheIterator OTCMD::eidcache()
{
  const otCacheEntryIterator initval = {0};
  return initval;
}
#endif
OT_V_FUNC_1_IMPL(eui64, Link, GetFactoryAssignedIeeeEui64, otExtAddress*);
OTExtAddress OTCMD::eui64() { static otExtAddress eaddr; eui64(&eaddr); return &eaddr; }
#if OPENTHREAD_POSIX
// x exit
#endif
#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_DEBUG_UART) && OPENTHREAD_POSIX
// &&&	logfilename	\<filename\>
#endif
OT_SETTER_IMPL(const otExtAddress*, extaddr, Link, ExtendedAddress);
OTExtAddress OTCMD::extaddr()
{
  const otExtAddress* extaddr;
  OT_API_CALL(extaddr = otLinkGetExtendedAddress(otrGetInstance() ) );
  return extaddr;
}
OT_SETTER_IMPL(const otExtendedPanId*, extpanid, Thread, ExtendedPanId);
OTExtendedPanId OTCMD::extpanid()
{
  const otExtendedPanId* extpanid;
  OT_API_CALL(extpanid = otThreadGetExtendedPanId(otrGetInstance() ) );
  return extpanid;
}
OT_V_FUNC_0_IMPL(factoryreset, Instance, FactoryReset);
OT_FUNC_0_IMPL(bool, ifconfig, Ip6, IsEnabled);
OT_FUNC_1_IMPL(otError, _ifconfig, Ip6, SetEnabled, bool);
otError OTCMD::ifconfig_up() { return _ifconfig(true); }
otError OTCMD::ifconfig_down() { return _ifconfig(false); }
OTIpaddrIterator OTCMD::ipaddr()
{
  const otNetifAddress* addr;
  OT_API_CALL(addr = otIp6GetUnicastAddresses(otrGetInstance()) );
  return addr;
}
OT_FUNC_1_IMPL(otError, _ipaddr_add, Ip6, AddUnicastAddress, const struct otNetifAddress*);
OT_FUNC_1_IMPL(otError, ipaddr_del, Ip6, RemoveUnicastAddress, const otIp6Address*);
IPAddress OTCMD::ipaddr_linklocal()
{
  const otIp6Address* addr;
  OT_API_CALL(addr = otThreadGetLinkLocalIp6Address(otrGetInstance()) );
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  return addr->mFields.m16;
#pragma GCC diagnostic pop
}
IPAddress OTCMD::ipaddr_mleid()
{
  const otIp6Address* addr;
  OT_API_CALL(addr = otThreadGetMeshLocalEid(otrGetInstance()) );
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  return addr->mFields.m16;
#pragma GCC diagnostic pop
}
IPAddress OTCMD::ipaddr_rloc()
{
  const otIp6Address* addr;
  OT_API_CALL(addr = otThreadGetRloc(otrGetInstance()) );
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  return addr->mFields.m16;
#pragma GCC diagnostic pop
}
OTIpmaddrIterator OTCMD::ipmaddr() 
{
  const otNetifMulticastAddress* addr;
  OT_API_CALL(addr = otIp6GetMulticastAddresses(otrGetInstance()) );
  return addr;
}
OT_FUNC_1_IMPL(otError, ipmaddr_add, Ip6, SubscribeMulticastAddress, const otIp6Address*);
OT_FUNC_1_IMPL(otError, ipmaddr_del, Ip6, UnsubscribeMulticastAddress, const otIp6Address*);
OT_FUNC_0_IMPL(bool, ipmaddr_promiscuous, Ip6, IsMulticastPromiscuousEnabled);
OT_V_SETTER_IMPL(bool, ipmaddr_promiscuous, Ip6, MulticastPromiscuousEnabled);
#if OPENTHREAD_CONFIG_JOINER_ENABLE
OTExtAddress OTCMD::joiner_id() { OT_API_CALL_RET(OTExtAddress, otJoinerGetId(otrGetInstance())); }
//otError OTCMD::joiner_start(const char* pskc, const char* provision, otJoinerCallback cb, void* ctx);
//otError OTCMD::joiner_start(const char* pskc, const char* provision);
OT_V_FUNC_0_IMPL(joiner_stop, Joiner, Stop);
#endif
#if OPENTHREAD_FTD
OT_GETTER_IMPL(uint16_t, joinerport, Thread, JoinerUdpPort);
#endif
OT_V_SETGET_IMPL(uint32_t, keysequence_counter, Thread, KeySequenceCounter);
OT_V_SETGET_IMPL(uint32_t, keysequence_guardtime, Thread, KeySwitchGuardTime);
OT_FUNC_1_IMPL(otError, leaderdata, Thread, GetLeaderData, otLeaderData*);
#if OPENTHREAD_FTD
OT_GETTER_IMPL(uint32_t, partitionid, Thread, PartitionId);
OT_V_SETGET_IMPL(uint32_t, partitionid_preferred, Thread, PreferredLeaderPartitionId);
OT_V_SETGET_IMPL(uint8_t, leaderweight, Thread, LocalLeaderWeight);
#endif

#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE
OTMacFilterIterator OTCMD::macfilter()
{
  return OT_MAC_FILTER_ITERATOR_INIT;
}

OTMacFilterAddrIterator OTCMD::macfilter_addr()
{
  return OT_MAC_FILTER_ITERATOR_INIT;
}
OTMacFilterRssIterator OTCMD::macfilter_rss()
{
  return OT_MAC_FILTER_ITERATOR_INIT;
}
OT_FUNC_2_IMPL(otError, macfilter_addr_add, LinkFilter, AddRssIn, const otExtAddress*, uint8_t);
otError OTCMD::macfilter_addr_add(uint8_t rss) { return macfilter_addr_add(nullptr, rss); }
OT_V_FUNC_1_IMPL(macfilter_addr_remove, LinkFilter, RemoveAddress, const otExtAddress*);
OT_V_FUNC_0_IMPL(macfilter_addr_clear, LinkFilter, ClearAddresses);

OT_FUNC_2_IMPL(otError, macfilter_rss_add, LinkFilter, AddRssIn, const otExtAddress*, uint8_t);
otError OTCMD::macfilter_rss_add(uint8_t rss) { return macfilter_rss_add(NULL, rss); }
OT_V_FUNC_1_IMPL(macfilter_rss_remove, LinkFilter, RemoveRssIn, const otExtAddress*);
OT_V_FUNC_0_IMPL(macfilter_rss_clear, LinkFilter, ClearAllRssIn);

OT_V_SETGET_IMPL(otMacFilterAddressMode, _macfilter_addr_mode, LinkFilter, AddressMode);
void OTCMD::macfilter_addr_disable() { _macfilter_addr_mode(OT_MAC_FILTER_ADDRESS_MODE_DISABLED); }
void OTCMD::macfilter_addr_denylist() { _macfilter_addr_mode(OT_MAC_FILTER_ADDRESS_MODE_DENYLIST); }
void OTCMD::macfilter_addr_allowlist() { _macfilter_addr_mode(OT_MAC_FILTER_ADDRESS_MODE_ALLOWLIST); }
#endif

OT_V_SETGET_IMPL(uint8_t, mac_retries_direct, Link, MaxFrameRetriesDirect);
#if OPENTHREAD_FTD
OT_V_SETGET_IMPL(uint8_t, mac_retries_indirect, Link, MaxFrameRetriesIndirect);
#endif

OT_SETTER_IMPL(const otMasterKey*, masterkey, Thread, MasterKey);
OTMasterKey OTCMD::masterkey()
{
  const otMasterKey* masterkey;
  OT_API_CALL(masterkey = otThreadGetMasterKey(otrGetInstance() ) );
  return masterkey;
}
OT_SETTER_IMPL(otLinkModeConfig, mode, Thread, LinkMode);
OTLinkModeConfig OTCMD::mode()
{
  OT_API_CALL_RET(otLinkModeConfig,
    otThreadGetLinkMode(otrGetInstance())
  );
}
#if OPENTHREAD_FTD
OTNeighborIterator OTCMD::neighbor()
{
  return OT_NEIGHBOR_INFO_ITERATOR_INIT;
}

#endif
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE || OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
//otError OTCMD::netdataregister();
//otError OTCMD::netdatashow(uint8_t*, uint8_t&);
#endif
#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
OT_FUNC_5_IMPL(otError, networkdiagnostic_get, Thread, SendDiagnosticGet, const otIp6Address*, const uint8_t*, uint8_t, otReceiveDiagnosticGetCallback, void*);
OT_FUNC_3_IMPL(otError, networkdiagnostic_reset, Thread, SendDiagnosticReset, const otIp6Address*, uint8_t*, uint8_t);
#endif
#if OPENTHREAD_FTD
OT_V_SETGET_IMPL(uint8_t, networkidtimeout, Thread, NetworkIdTimeout);
#endif
OT_FUNC_0_IMPL(const char*, networkname, Thread, GetNetworkName);
OT_FUNC_1_IMPL(otError, networkname, Thread, SetNetworkName, const char*);
#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
// x networktime
#endif
OT_SETGET_IMPL(uint16_t, panid, Link, PanId);
OT_FUNC_1_IMPL(otError, parent, Thread, GetParentInfo, otRouterInfo*);
#if OPENTHREAD_FTD
OT_SETGET_IMPL(uint8_t, parentpriority, Thread, ParentPriority);
#endif
//otError ping(IPAddress& addr, const uint8_t*, uint16_t);
OT_SETGET_IMPL(uint32_t, pollperiod, Link, PollPeriod);
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
OTPrefixIterator OTCMD::prefix()
{
  return OT_NETWORK_DATA_ITERATOR_INIT;
}
//otError OTCMD::prefix_add(const otIp6Prefix*, otRoutePreference, bool, bool, bool, bool, bool, bool, bool);
OT_FUNC_1_IMPL(otError, _prefix_add, BorderRouter, AddOnMeshPrefix, const otBorderRouterConfig*);
OT_FUNC_1_IMPL(otError, prefix_remove, BorderRouter, RemoveOnMeshPrefix, const otIp6Prefix*);
#endif
OT_FUNC_0_IMPL(bool, promiscuous, Link, IsPromiscuous);
#if OPENTHREAD_FTD
OT_SETTER_IMPL(uint8_t, preferrouterid, Thread, PreferredRouterId);
OT_SETTER_IMPL(const otPskc*, pskc, Thread, Pskc);
OTPskc OTCMD::pskc()
{
  const otPskc* pskc;
  OT_API_CALL( pskc = otThreadGetPskc(otrGetInstance() ) );
  return pskc;
}

OT_FUNC_1_IMPL(otError, releaserouterid, Thread, ReleaseRouterId, uint8_t);
#endif
OT_V_FUNC_0_IMPL(reset, Instance, Reset);
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
OTRouteIterator OTCMD::route()
{
  return OT_NETWORK_DATA_ITERATOR_INIT;
}
//otError OTCMD::route_add(otRoutePreference preference, bool stable);
OT_FUNC_1_IMPL(otError, route_add, BorderRouter, AddRoute, const otExternalRouteConfig*);
OT_FUNC_1_IMPL(otError, route_remove, BorderRouter, RemoveRoute, const otIp6Prefix*);
#endif
#if OPENTHREAD_FTD
OTRouterIterator OTCMD::router()
{
  return 0;
}
OT_FUNC_2_IMPL(otError, router, Thread, GetRouterInfo, int, otRouterInfo*);
OT_V_SETGET_IMPL(uint8_t, routerdowngradethreshold, Thread, RouterDowngradeThreshold);
OT_FUNC_0_IMPL(bool, routereligible, Thread, IsRouterEligible);
OT_V_SETTER_IMPL(uint8_t, _routereligible, Thread, RouterEligible);
void OTCMD::routereligible_enable() { _routereligible(true); }
void OTCMD::routereligible_disable() { _routereligible(false); }
OT_V_SETGET_IMPL(uint8_t, routerselectionjitter, Thread, RouterSelectionJitter);
OT_V_SETGET_IMPL(uint8_t, routerupgradethreshold, Thread, RouterUpgradeThreshold);
#endif
OT_GETTER_IMPL(uint16_t, rloc16, Thread, Rloc16);
//OT_FUNC_4_IMPL(otError, scan, Link, ActiveScan, uint16_t, uint32_t, otHandleActiveScanResult, void*);

otError OTCMD::scan(uint32_t scanChannels, uint16_t scanDuration, otHandleActiveScanResult handler, void* context)
{
  OT_API_CALL_RET(otError, 
    otLinkActiveScan(otrGetInstance(), scanChannels, scanDuration, handler, context);
  );
}


void scan_callback(otActiveScanResult *aResult, void *aContext)
{
  OTActiveScanResultSyncIterator* ctx = reinterpret_cast<OTActiveScanResultSyncIterator*>(aContext);
  ctx->receive(aResult);
  xSemaphoreGive(ctx->scan_sem);
}

otError OTCMD::scan(uint32_t scanChannel, uint16_t scanDuration, OTActiveScanResultSyncIterator* ctx)
{
  otError err = scan(scanChannel, scanDuration, scan_callback, ctx);

  if(err) {
    ctx->set_error(err);
    return err;
  }

  if(!xSemaphoreTake(ctx->scan_sem, portMAX_DELAY) ) {
    ctx->set_error(OT_ERROR_GENERIC);
  }

  return err;
}

otError OTCMD::scan_energy(uint32_t scanChannels, uint16_t scanDuration, otHandleEnergyScanResult handler, void* context)
{
  OT_API_CALL_RET(otError, 
    otLinkEnergyScan(otrGetInstance(), scanChannels, scanDuration, handler, context);
  );
}

void scan_energy_callback(otEnergyScanResult *aResult, void *aContext)
{
  OTEnergyScanResultSyncIterator* ctx = reinterpret_cast<OTEnergyScanResultSyncIterator*>(aContext);
  ctx->receive(aResult);
  xSemaphoreGive(ctx->scan_sem);
}

otError OTCMD::scan_energy(uint32_t scanChannel, uint16_t scanDuration, OTEnergyScanResultSyncIterator* ctx)
{
  otError err = scan_energy(scanChannel, scanDuration, scan_energy_callback, ctx);

  if(err) {
    ctx->set_error(err);
    return err;
  }

  if(!xSemaphoreTake(ctx->scan_sem, portMAX_DELAY) ) {
    ctx->set_error(OT_ERROR_GENERIC);
  }

  return err;
}

//OT_FUNC_4_IMPL(otError, scan_energy, Link, EnergyScan, uint16_t, uint32_t, otHandleEnergyScanResult, void*);
#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
//otError OTCMD::service_add(uint32_t, uint8_t*, uint8_t, uint8_t*, uint8_t);
//otError OTCMD::service_add(uint32_t, const char*, const char*);
//otError OTCMD::service_remove(uint32_t, const char*);
OT_FUNC_3_IMPL(otError, service_remove, Server, RemoveService, uint32_t, uint8_t*, uint8_t);
#endif
OT_IS_IMPL(bool, singleton, Thread, Singleton);
#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
//otError OTCMD::sntp_query(const otIp6Address* addr, uint16_t port, otSntpResponseHandler handler, void* context);
#endif
OT_GETTER_IMPL(otDeviceRole, state, Thread, DeviceRole);
OT_FUNC_0_IMPL(otError, state_child, Thread, BecomeChild);
OT_FUNC_0_IMPL(otError, state_detached, Thread, BecomeDetached);
#if OPENTHREAD_FTD
OT_FUNC_0_IMPL(otError, state_router, Thread, BecomeRouter);
OT_FUNC_0_IMPL(otError, state_leader, Thread, BecomeLeader);
#endif
OT_SETTER_IMPL(bool, _thread, Thread, Enabled);
otError OTCMD::thread_start() { return _thread(true); }
otError OTCMD::thread_stop() { return _thread(false); }
uint16_t OTCMD::thread_version() { return otThreadGetVersion(); }
//dataset
OT_SETTER_IMPL(int8_t, txpower, PlatRadio, TransmitPower);
OT_FUNC_1_IMPL(otError, _txpower, PlatRadio, GetTransmitPower, int8_t*);
int8_t OTCMD::txpower() { int8_t r; otError e = _txpower(&r); return e ? 0 : r; }
const char* OTCMD::version() { return otGetVersionString(); }

otError OTCMD::discover(uint32_t chbits, otHandleActiveScanResult callback, void* context)
{
  OT_API_CALL_RET(otError,
    otThreadDiscover(otrGetInstance(), chbits, OT_PANID_BROADCAST, false, false, callback, context)
  );
}

void discover_callback(otActiveScanResult *aResult, void *aContext)
{
  OTActiveScanResultSyncIterator* ctx = reinterpret_cast<OTActiveScanResultSyncIterator*>(aContext);
  ctx->receive(aResult);
  xSemaphoreGive(ctx->scan_sem);
}

otError OTCMD::discover(uint32_t chbits, OTActiveScanResultSyncIterator* ctx)
{
  otError err = discover(chbits, discover_callback, ctx);
  if(err) {
    ctx->set_error(err);
    return err;
  }

  if(!xSemaphoreTake(ctx->scan_sem, portMAX_DELAY) ) {
    ctx->set_error(OT_ERROR_GENERIC);
  }

  return err;
}

otError OTCMD::ipaddr_add(const otIp6Address* addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc)
{
  struct otNetifAddress address;

  memcpy(&address.mAddress, addr, sizeof(otIp6Address));
  address.mPrefixLength = prefixlen;
  address.mPreferred = preferred;
  address.mValid = valid;
  address.mScopeOverrideValid = scopeoverridevalid;
  address.mScopeOverride = scopeoverride;
  address.mRloc = rloc;

  return  _ipaddr_add(&address);
}


#if OPENTHREAD_CONFIG_JOINER_ENABLE

otError OTCMD::joiner_start(const char* pskc, const char* provision, otJoinerCallback cb, void* ctx)
{
  OT_API_CALL_RET(otError,
    otJoinerStart(otrGetInstance(), pskc, provision, OTAPI_PACKAGE_NAME, OPENTHREAD_CONFIG_PLATFORM_INFO,
                      OTAPI_PACKAGE_VERSION, NULL, cb, ctx);
  );
}

void joiner_start_sync_callback(otError aResult, void *aContext)
{
  OTJoinerContext* ctx = reinterpret_cast<OTJoinerContext*>(aContext);
  ctx->error = aResult;
  xSemaphoreGive(ctx->join_sem);
}

otError OTCMD::joiner_start(const char* pskc, const char* provision, OTJoinerContext* ctx)
{
  otError err = joiner_start(pskc, provision, joiner_start_sync_callback, ctx);
  if(err) {
    ctx->error = err;
    return err;
  }

  if(!xSemaphoreTake(ctx->join_sem, portMAX_DELAY) ) {
    ctx->error = OT_ERROR_GENERIC;
  }

  return err;
}

#endif

otError OTCMD::ping(const otIp6Address* addr, const uint8_t* buffer, uint16_t buflen)
{
  return OT_ERROR_NONE; //TODO
}

otError OTCMD::ping_stop()
{
  return OT_ERROR_NONE; //TODO
}

otError OTCMD::promiscuous_enable(otLinkPcapCallback cb, void* ctx)
{
  OT_API_CALL(otLinkSetPcapCallback(otrGetInstance(), cb, ctx); );
  OT_API_CALL_RET(otError,
    otLinkSetPromiscuous(otrGetInstance(), true);
  );
}

otError OTCMD::promiscuous_disable()
{
  OT_API_CALL( otLinkSetPcapCallback(otrGetInstance(), NULL, NULL); );
  OT_API_CALL_RET(otError,
    otLinkSetPromiscuous(otrGetInstance(), false);
  );
}

typedef otError (*state_func)(otInstance*);

otError OTCMD::state(otDeviceRole role)
{
  state_func statefunc = nullptr;
  switch (role)
  {
    case OT_DEVICE_ROLE_DETACHED:
      statefunc = otThreadBecomeDetached;
      break;
    case OT_DEVICE_ROLE_CHILD:
      statefunc = otThreadBecomeChild;
      break;

#if OPENTHREAD_FTD
    case OT_DEVICE_ROLE_ROUTER:
      statefunc = otThreadBecomeRouter;
      break;
    case OT_DEVICE_ROLE_LEADER:
      statefunc = otThreadBecomeLeader;
      break;
#endif // OPENTHREAD_FTD
    default:
      return OT_ERROR_INVALID_ARGS;
  }
  OT_API_CALL_RET(otError,
    statefunc(otrGetInstance());
  );
}

#if OPENTHREAD_FTD
otError OTCMD::commissioner_joiner_add(const char* pskd, uint32_t timeout)
{
  return commissioner_joiner_add(NULL, pskd, timeout);
}

otError OTCMD::commissioner_joiner_remove()
{
  return commissioner_joiner_remove(NULL);
}

otError OTCMD::commissioner_mgmtget(otMeshcopTlvType* tlvtypes, uint8_t count)
{
  uint8_t tlvs[32];

  for(uint8_t i=0; i<count; i++) {
    tlvs[i] = tlvtypes[i];
  }

  OT_API_CALL_RET(otError,
    otCommissionerSendMgmtGet(otrGetInstance(), tlvs, count);
  );
}

otError OTCMD::commissioner_mgmtset(uint32_t rloc, uint32_t sessionid, uint32_t port, uint8_t* steeringdata, uint8_t steeringlen, otMeshcopTlvType* tlvtypes, uint8_t count)
{
  uint8_t tlvs[32];
  otCommissioningDataset dataset;

  if(rloc <= 0xFFFFL) {
    dataset.mIsLocatorSet = true;
    dataset.mLocator = static_cast<uint16_t>(rloc);
  }

  if(sessionid <= 0xFFFFL) {
    dataset.mIsSessionIdSet = true;
    dataset.mSessionId = static_cast<uint16_t>(sessionid);
  }

  if(steeringdata != NULL) {
    dataset.mIsSteeringDataSet = true;
    memcpy(dataset.mSteeringData.m8, steeringdata, steeringlen);
    dataset.mSteeringData.mLength = steeringlen;
  }

  if(port <= 0xFFFFL) {
    dataset.mIsJoinerUdpPortSet = true;
    dataset.mJoinerUdpPort = static_cast<uint16_t>(port);
  }

  for(uint8_t i=0; i<count; i++) {
    tlvs[i] = tlvtypes[i];
  }

  OT_API_CALL_RET(otError,
    otCommissionerSendMgmtSet(otrGetInstance(), &dataset, tlvs, count);
  );
}
#endif

otError OTCMD::dataset_active(otOperationalDataset& dataset) {
  return OTCMD::dataset_init_active(dataset);
}

otError OTCMD::dataset_pending(otOperationalDataset& dataset) {
  return OTCMD::dataset_init_pending(dataset);
}

otError OTCMD::dataset_commit_active(otOperationalDataset& dataset) {
  OT_API_CALL_RET(otError,
    otDatasetSetActive(otrGetInstance(), &dataset);
  );
}

otError OTCMD::dataset_commit_pending(otOperationalDataset& dataset) {
  OT_API_CALL_RET(otError,
    otDatasetSetPending(otrGetInstance(), &dataset);
  );
}


otError OTCMD::dataset_init_active(otOperationalDataset& dataset) {
  OT_API_CALL_RET(otError,
    otDatasetGetActive(otrGetInstance(), &dataset);
  );
}

otError OTCMD::dataset_init_pending(otOperationalDataset& dataset) {
  OT_API_CALL_RET(otError,
    otDatasetGetPending(otrGetInstance(), &dataset);
  );
}

#if OPENTHREAD_FTD
otError OTCMD::dataset_init_new(otOperationalDataset& dataset) {
  OT_API_CALL_RET(otError,
    otDatasetCreateNewNetwork(otrGetInstance(), &dataset);
  );
}
#endif

void OTCMD::dataset_clear(otOperationalDataset& dataset) {
  memset(&dataset, 0, sizeof(otOperationalDataset));
}

void OTCMD::dataset_delay(otOperationalDataset& dataset, uint32_t delay) {
  dataset.mDelay = delay;
  dataset.mComponents.mIsDelayPresent = true;
}

void OTCMD::dataset_extpanid(otOperationalDataset& dataset, const otExtendedPanId* panid)
{
  memcpy(&dataset.mExtendedPanId, panid, sizeof(otExtendedPanId));
  dataset.mComponents.mIsExtendedPanIdPresent = true;
}

void OTCMD::dataset_masterkey(otOperationalDataset& dataset, const otMasterKey* masterkey)
{
  memcpy(&dataset.mMasterKey, masterkey, sizeof(otMasterKey));
  dataset.mComponents.mIsMasterKeyPresent = true;
}

void OTCMD::dataset_meshlocalprefix(otOperationalDataset& dataset, const otMeshLocalPrefix* prefix)
{
  memcpy(&dataset.mMeshLocalPrefix, prefix, sizeof(otMeshLocalPrefix));
  dataset.mComponents.mIsMeshLocalPrefixPresent = true;
}

void OTCMD::dataset_networkname(otOperationalDataset& dataset, const char* netname)
{
  memcpy(&dataset.mNetworkName, netname, sizeof(otNetworkName));
  dataset.mComponents.mIsNetworkNamePresent = true;
}

void OTCMD::dataset_panid(otOperationalDataset& dataset, otPanId panid)
{
  dataset.mPanId = panid;
  dataset.mComponents.mIsPanIdPresent = true;
}

void OTCMD::dataset_activetimestamp(otOperationalDataset& dataset, uint64_t timestamp)
{
  dataset.mActiveTimestamp = timestamp;
  dataset.mComponents.mIsActiveTimestampPresent = true;
}
void OTCMD::dataset_pendingtimestamp(otOperationalDataset& dataset, uint64_t timestamp)
{
  dataset.mPendingTimestamp = timestamp;
  dataset.mComponents.mIsPendingTimestampPresent = true;
}
void OTCMD::dataset_channel(otOperationalDataset& dataset, uint16_t channel)
{
  dataset.mChannel = channel;
  dataset.mComponents.mIsChannelPresent = true;
}
void OTCMD::dataset_channelmask(otOperationalDataset& dataset, uint32_t channelmask)
{
  dataset.mChannelMask = channelmask;
  dataset.mComponents.mIsChannelMaskPresent = true;
}

void OTCMD::dataset_securitypolicy(otOperationalDataset& dataset, const otSecurityPolicy* securitypolicy)
{
  dataset.mSecurityPolicy = *securitypolicy;
  dataset.mComponents.mIsSecurityPolicyPresent = true;
}

#if OPENTHREAD_FTD
void OTCMD::dataset_pskc(otOperationalDataset& dataset, const otPskc& pskc_)
{
  memcpy(&dataset.mNetworkName, &pskc_, sizeof(otPskc));
  dataset.mComponents.mIsPskcPresent = true;
}
#endif


otError OTCMD::dataset_mgmtgetcommand_active(otOperationalDatasetComponents& components, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtActiveGet(otrGetInstance(), &components, tlvs, static_cast<uint8_t>(len), NULL );
  );
}

otError OTCMD::dataset_mgmtgetcommand_active(otOperationalDatasetComponents& components, const otIp6Address& addr, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtActiveGet(otrGetInstance(), &components, tlvs, static_cast<uint8_t>(len), &addr );
  );
}

otError OTCMD::dataset_mgmtgetcommand_pending(otOperationalDatasetComponents& components, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtPendingGet(otrGetInstance(), &components, tlvs, static_cast<uint8_t>(len), NULL );
  );
}

otError OTCMD::dataset_mgmtgetcommand_pending(otOperationalDatasetComponents& components, const otIp6Address& addr, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtPendingGet(otrGetInstance(), &components, tlvs, static_cast<uint8_t>(len), &addr );
  );
}

otError OTCMD::dataset_mgmtsetcommand_active(otOperationalDataset& dataset, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtActiveSet(otrGetInstance(), &dataset, tlvs, static_cast<uint8_t>(len) );
  );
}
otError OTCMD::dataset_mgmtsetcommand_pending(otOperationalDataset& dataset, const uint8_t* tlvs, const size_t len) {
  OT_API_CALL_RET(otError,
    otDatasetSendMgmtPendingSet(otrGetInstance(), &dataset, tlvs, static_cast<uint8_t>(len) );
  );
}



#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE

otError OTCMD::dns_resolve(const char* hostname, otDnsAddressCallback handler, void* context, const otDnsQueryConfig* config)
{
  OT_API_CALL_RET(otError,
    otDnsClientResolveAddress(otrGetInstance(), hostname, handler, context, config);
  );
}

#endif


#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE
otError OTCMD::macfilter_rss_add_lqi(const otExtAddress* addr, uint8_t linkquality)
{
  OT_API_CALL_RET(otError,
    otLinkFilterAddRssIn(otrGetInstance(), addr, otLinkConvertLinkQualityToRss(otrGetInstance(), linkquality) );
  );
}

otError OTCMD::macfilter_rss_add_lqi(uint8_t linkquality)
{
  return macfilter_rss_add_lqi(NULL, linkquality);
}
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE || OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
otError OTCMD::netdataregister()
{
  OT_API_CALL_RET(otError,
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
    otBorderRouterRegister(otrGetInstance());
#else
    otServerRegister(otrGetInstance());
#endif
  );
}

otError OTCMD::netdatashow(uint8_t* data, uint8_t& len)
{
  OT_API_CALL_RET(otError,
    otNetDataGet(otrGetInstance(), false, data, &len);
  );
}
#endif

#if OPENTHREAD_FTD
otError OTCMD::pskc_passphrase(const char* passphrase)
{
  otPskc pskc;
  otError error;
  OT_API_CALL_RET(otError,
    otDatasetGeneratePskc(passphrase,
                              reinterpret_cast<const otNetworkName *>(otThreadGetNetworkName(otrGetInstance())),
                              otThreadGetExtendedPanId(otrGetInstance()), &pskc);
  );

  OT_API_CALL_RET(otError,
    otThreadSetPskc(otrGetInstance(), &pskc);
  );
  return error;
}
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
otError OTCMD::prefix_add(const otIp6Prefix* prefix, otRoutePreference preference, 
		bool preferred, bool slaac, bool dhcp, bool config, bool defaultroute, bool onmesh, bool stable)
{
  otBorderRouterConfig cfg = { 0 };
  cfg.mPreference = preference;
  cfg.mPrefix = *prefix;

  cfg.mPreferred = preferred;
  cfg.mSlaac = slaac;
  cfg.mDhcp = dhcp;
  cfg.mConfigure = config;
  cfg.mDefaultRoute = defaultroute;
  cfg.mOnMesh = onmesh;
  cfg.mStable = stable;

  return _prefix_add(&cfg);
}
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
otError OTCMD::route_add(otRoutePreference preference, bool stable)
{
  otExternalRouteConfig cfg = { 0 };
  cfg.mPreference = preference;
  cfg.mStable = stable;
  return route_add(&cfg);
}
#endif

#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
otError OTCMD::service_add(uint32_t no, uint8_t* servicedata, uint8_t servicedatasize, uint8_t* serverdata, uint8_t serverdatasize)
{
  otServiceConfig cfg = { 0 };
  cfg.mEnterpriseNumber = no;
  if(servicedatasize > sizeof(cfg.mServiceData)) return OT_ERROR_NO_BUFS;
  if(serverdatasize  > sizeof(cfg.mServerConfig.mServerData)) return OT_ERROR_NO_BUFS;
  memcpy(cfg.mServiceData, servicedata, servicedatasize);
  memcpy(cfg.mServerConfig.mServerData, serverdata, serverdatasize);
  cfg.mServiceDataLength = servicedatasize;
  cfg.mServerConfig.mServerDataLength = serverdatasize;
  cfg.mServerConfig.mStable = true;

  OT_API_CALL_RET(otError,
    otServerAddService(otrGetInstance(), &cfg);
  );
}
otError OTCMD::service_add(uint32_t no, const char* servicedata, const char* serverdata)
{
  return service_add(no, 
    const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(servicedata)), static_cast<uint8_t>(strlen(servicedata)),
    const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(serverdata)),  static_cast<uint8_t>(strlen(serverdata)) );
}


otError OTCMD::service_remove(uint32_t no, const char* servicedata)
{
  return service_remove(no, 
    const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(servicedata)), static_cast<uint8_t>(strlen(servicedata)));
}
#endif

#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
otError OTCMD::sntp_query(const otIp6Address* addr, uint16_t port, otSntpResponseHandler handler, void* context)
{
  otSntpQuery query = { 0 };
  otMessageInfo msginfo = { 0 };

  msginfo.mPeerAddr = *addr;
  msginfo.mPeerPort = port;

  query.mMessageInfo = &msginfo;

  OT_API_CALL_RET(otError,
    otSntpClientQuery(otrGetInstance(), &query, handler, context);
  );
}
#endif

