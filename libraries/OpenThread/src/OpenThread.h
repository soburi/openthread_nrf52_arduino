/*
  Copyright (c) 2019 Tokita, Hiroshi  All right reserved.

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

#ifndef SOCKETS_OPENTHREAD_H
#define SOCKETS_OPENTHREAD_H

#include "IPAddress.h"
#include "Print.h"

#include <nrf52840.h>

#include <openthread/config.h>
#include <openthread/backbone_router.h>
#include <openthread/backbone_router_ftd.h>
#include <openthread/border_agent.h>
#include <openthread/border_router.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/message.h>
#include <openthread/thread.h>
#include <openthread/dataset.h>
#include <openthread/coap.h>
#include <openthread/coap_secure.h>
#include <openthread/thread_ftd.h>
#include <openthread/dataset_ftd.h>
#include <openthread/diag.h>
#include <openthread/commissioner.h>
#include <openthread/joiner.h>
#include <openthread/netdata.h>
#include <openthread/server.h>
#include <openthread/platform/radio.h>

#include <openthread/icmp6.h>
#include <openthread/dns.h>
#include <openthread/dns.h>
#include <openthread/jam_detection.h>
#include <openthread/link_raw.h>
#include <openthread/netdiag.h>
#include <openthread/network_time.h>
#include <openthread/sntp.h>
#include <openthread/udp.h>
#include <openthread/tasklet.h>
#include <openthread/channel_manager.h>
#include <openthread/channel_monitor.h>
#include <openthread/child_supervision.h>
#include <openthread/cli.h>
#include <openthread/ncp.h>
#include <openthread/diag.h>
#include <openthread/platform/diag.h>

#include <openthread/openthread-freertos.h>

#define OT_CALL_FUNC0(cls, fn) ot##cls##fn (otrGetInstance())
#define OT_CALL_FUNC1(cls, fn) ot##cls##fn (otrGetInstance(), a1)
#define OT_CALL_FUNC2(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2)
#define OT_CALL_FUNC3(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3)
#define OT_CALL_FUNC4(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4)
#define OT_CALL_FUNC5(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5)
#define OT_CALL_FUNC6(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6)
#define OT_CALL_FUNC7(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6, a7)
#define OT_CALL_FUNC8(cls, fn) ot##cls##fn (otrGetInstance(), a1, a2, a3, a4, a5, a6, a7, a8)

#define OT_DECL_ARGS0()
#define OT_DECL_ARGS1(T1) T1 a1
#define OT_DECL_ARGS2(T1, T2) T1 a1, T2 a2
#define OT_DECL_ARGS3(T1, T2, T3) T1 a1, T2 a2, T3 a3
#define OT_DECL_ARGS4(T1, T2, T3, T4) T1 a1, T2 a2, T3 a3, T4 a4
#define OT_DECL_ARGS5(T1, T2, T3, T4, T5) T1 a1, T2 a2, T3 a3, T4 a4, T5 a5
#define OT_DECL_ARGS6(T1, T2, T3, T4, T5, T6) T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6
#define OT_DECL_ARGS7(T1, T2, T3, T4, T5, T6, T7) T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7
#define OT_DECL_ARGS8(T1, T2, T3, T4, T5, T6, T7, T8) T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8

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

#define OTCLS(cls) OT ## cls
#define OT_DECL__FUNC(n, cls, rettype, fn, ...) \
rettype OpenThreadClass::  OTCLS(cls)  ::  fn (OT_DECL_ARGS ## n(__VA_ARGS__)) \
{ \
  rettype ret = OT_CALL_FUNC ## n(cls, fn); \
  return ret; \
}

#define OT_DECL_VFUNC(n, cls, rettype, fn, ...) \
rettype OpenThreadClass::  OTCLS(cls)  ::  fn (OT_DECL_ARGS ## n(__VA_ARGS__)) \
{ \
  OT_CALL_FUNC ## n(cls, fn); \
}

template<typename T> class OTm8Buffer : public Printable {
public:
  OTm8Buffer() : value(&default_value) { }
  OTm8Buffer(const T* v) : value(v) { }
  OTm8Buffer(const uint8_t* v) : value( reinterpret_cast<const T*>(v) ) { }
  size_t printTo(Print& p) const {
    size_t n=0;
    for(unsigned int i=0; i<sizeof(T); i++) {
      uint8_t byte = value->m8[i];
      if(byte <= 0xF) n += p.print('0');
      n += p.print(byte, HEX);
    }
    return n;
  }
  operator const uint8_t*() { return value->m8; }
  operator const T*() { return value; }
private:
  static T default_value;
  const T* value;
};

template<typename T> class OTm8String : public Printable {
public:
  OTm8String(const T* v) : value(v) { }
  OTm8String(const char* v) : value( reinterpret_cast<const T*>(v) ) { }
  size_t printTo(Print& p) const {
    return p.print(reinterpret_cast<const char*>(value->m8));
  }
  operator const char*() { return reinterpret_cast<const char*>(value->m8); }
private:
  const T* value;
};

typedef class OTm8Buffer<otExtAddress> OTExtAddress;
typedef class OTm8Buffer<otExtendedPanId> OTExtendedPanId;
typedef class OTm8Buffer<otMasterKey> OTMasterKey;
typedef class OTm8String<otNetworkName> OTNetworkName;
typedef class OTm8Buffer<otPskc> OTPskc;

class OTLinkModeConfig : public Printable{
public:
  OTLinkModeConfig(const otLinkModeConfig v) : conf(v) { }
  size_t printTo(Print& p) const {
    size_t n = 0;
    if (conf.mRxOnWhenIdle)       n += p.print("r");
    if (conf.mSecureDataRequests) n += p.print("s");
    if (conf.mDeviceType)         n += p.print("d");
    if (conf.mNetworkData)        n += p.print("n");
    return n;
  }

  operator const otLinkModeConfig() { return conf; }
private:
  otLinkModeConfig conf;
};

class OTMacCounters : public Printable {
public:
  OTMacCounters(const otMacCounters* v) : value(v) { }

  uint32_t TxTotal() const { return value->mTxTotal; }
  uint32_t TxUnicast() const { return value->mTxUnicast; }
  uint32_t TxBroadcast() const { return value->mTxBroadcast; }
  uint32_t TxAckRequested() const { return value->mTxAckRequested; }
  uint32_t TxAcked() const { return value->mTxAcked; }
  uint32_t TxNoAckRequested() const { return value->mTxNoAckRequested; }
  uint32_t TxData() const { return value->mTxData; }
  uint32_t TxDataPoll() const { return value->mTxDataPoll; }
  uint32_t TxBeacon() const { return value->mTxBeacon; }
  uint32_t TxBeaconRequest() const { return value->mTxBeaconRequest; }
  uint32_t TxOther() const { return value->mTxOther; }
  uint32_t TxRetry() const { return value->mTxRetry; }
  uint32_t TxErrCca() const { return value->mTxErrCca; }
  uint32_t TxErrAbort() const { return value->mTxErrAbort; }
  uint32_t TxErrBusyChannel() const { return value->mTxErrBusyChannel; }
  uint32_t RxTotal() const { return value->mRxTotal; }
  uint32_t RxUnicast() const { return value->mRxUnicast; }
  uint32_t RxBroadcast() const { return value->mRxBroadcast; }
  uint32_t RxData() const { return value->mRxData; }
  uint32_t RxDataPoll() const { return value->mRxDataPoll; }
  uint32_t RxBeacon() const { return value->mRxBeacon; }
  uint32_t RxBeaconRequest() const { return value->mRxBeaconRequest; }
  uint32_t RxOther() const { return value->mRxOther; }
  uint32_t RxAddressFiltered() const { return value->mRxAddressFiltered; }
  uint32_t RxDestAddrFiltered() const { return value->mRxDestAddrFiltered; }
  uint32_t RxDuplicated() const { return value->mRxDuplicated; }
  uint32_t RxErrNoFrame() const { return value->mRxErrNoFrame; }
  uint32_t RxErrUnknownNeighbor() const { return value->mRxErrUnknownNeighbor; }
  uint32_t RxErrInvalidSrcAddr() const { return value->mRxErrInvalidSrcAddr; }
  uint32_t RxErrSec() const { return value->mRxErrSec; }
  uint32_t RxErrFcs() const { return value->mRxErrFcs; }
  uint32_t RxErrOther() const { return value->mRxErrOther; }
  operator const otMacCounters*() const { return value; }
  size_t printTo(Print& p) const {
    size_t n = 0; 
    n += p.print("TxTotal: "); n += p.println(value->mTxTotal);
    n += p.print("TxUnicast: "); n += p.println(value->mTxUnicast);
    n += p.print("TxBroadcast: "); n += p.println(value->mTxBroadcast);
    n += p.print("TxAckRequested: "); n += p.println(value->mTxAckRequested);
    n += p.print("TxAcked: "); n += p.println(value->mTxAcked);
    n += p.print("TxNoAckRequested: "); n += p.println(value->mTxNoAckRequested);
    n += p.print("TxData: "); n += p.println(value->mTxData);
    n += p.print("TxDataPoll: "); n += p.println(value->mTxDataPoll);
    n += p.print("TxBeacon: "); n += p.println(value->mTxBeacon);
    n += p.print("TxBeaconRequest: "); n += p.println(value->mTxBeaconRequest);
    n += p.print("TxOther: "); n += p.println(value->mTxOther);
    n += p.print("TxRetry: "); n += p.println(value->mTxRetry);
    n += p.print("TxErrCca: "); n += p.println(value->mTxErrCca);
    n += p.print("TxErrAbort: "); n += p.println(value->mTxErrAbort);
    n += p.print("TxErrBusyChannel: "); n += p.println(value->mTxErrBusyChannel);
    n += p.print("RxTotal: "); n += p.println(value->mRxTotal);
    n += p.print("RxUnicast: "); n += p.println(value->mRxUnicast);
    n += p.print("RxBroadcast: "); n += p.println(value->mRxBroadcast);
    n += p.print("RxData: "); n += p.println(value->mRxData);
    n += p.print("RxDataPoll: "); n += p.println(value->mRxDataPoll);
    n += p.print("RxBeacon: "); n += p.println(value->mRxBeacon);
    n += p.print("RxBeaconRequest: "); n += p.println(value->mRxBeaconRequest);
    n += p.print("RxOther: "); n += p.println(value->mRxOther);
    n += p.print("RxAddressFiltered: "); n += p.println(value->mRxAddressFiltered);
    n += p.print("RxDestAddrFiltered: "); n += p.println(value->mRxDestAddrFiltered);
    n += p.print("RxDuplicated: "); n += p.println(value->mRxDuplicated);
    n += p.print("RxErrNoFrame: "); n += p.println(value->mRxErrNoFrame);
    n += p.print("RxErrUnknownNeighbor: "); n += p.println(value->mRxErrUnknownNeighbor);
    n += p.print("RxErrInvalidSrcAddr: "); n += p.println(value->mRxErrInvalidSrcAddr);
    n += p.print("RxErrSec: "); n += p.println(value->mRxErrSec);
    n += p.print("RxErrFcs: "); n += p.println(value->mRxErrFcs);
    n += p.print("RxErrOther: "); n += p.println(value->mRxErrOther);
    return n;
  }
private:
  const otMacCounters* value;
};

class OTErr: public Printable {
public:
  OTErr(const otError e=OT_ERROR_NONE) : err(e) { }
  size_t printTo(Print& p) const {
    const char* msg = otThreadErrorToString(err);
    if(msg) return p.print(msg);
    else return 0;
  }

  operator otError() { return err; }
private:
  otError err;
};

class OpenThreadClass {
public:
  int begin();
  int dump(Print& p);
  // x help
  
  OT_V_FUNC_1_DECL(bufferinfo, Message, GetBufferInfo, otBufferInfo*);
  OT_SETGET_DECL(uint8_t, channel, Link, Channel);
#if OPENTHREAD_FTD
  OT_FUNC_2_DECL(otError, child, Thread, GetChildInfoByIndex, int, otChildInfo*);
  // childip
  OT_SETGET_DECL(uint8_t, childmax, Thread, MaxAllowedChildren);
#endif
  OT_V_SETGET_DECL(uint32_t, childtimeout, Thread, ChildTimeout);
  // x coap
  // x coaps
#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
  OT_GETTER_DECL(otCommissionerState, commissioner, Commissioner, State);
  OT_FUNC_3_DECL(otError, commissioner_start, Commissioner, Start, otCommissionerStateCallback, otCommissionerJoinerCallback, void*);
  OT_FUNC_0_DECL(otError, commissioner_stop, Commissioner, Stop);
  OT_SETTER_DECL(const char*, commissioner_provisioningurl, Commissioner, ProvisioningUrl);
  otError commissioner_announce(uint32_t mask, uint8_t count, uint16_t period, IPAddress& addr);
  otError _commissioner_announce(uint32_t mask, uint8_t count, uint16_t period, IPAddress& addr);
  otError commissioner_energy(uint32_t mask, uint8_t count, uint16_t period, uint16_t duration, IPAddress& addr, otCommissionerEnergyReportCallback cb, void* ctx);
  otError _commissioner_energy(uint32_t mask, uint8_t count, uint16_t period, uint16_t duration, IPAddress& addr, otCommissionerEnergyReportCallback cb, void* ctx);
  otError commissioner_panid(uint16_t, uint32_t, IPAddress&, otCommissionerPanIdConflictCallback, void* ctx);
  otError _commissioner_panid(uint16_t, uint32_t, IPAddress&, otCommissionerPanIdConflictCallback, void* ctx);
  OT_GETTER_DECL(uint16_t, commissioner_sessionid, Commissioner, SessionId);
#endif
#if OPENTHREAD_FTD
  OT_V_SETGET_DECL(uint32_t, contextreusedelay, Thread, ContextIdReuseDelay);
#endif
  const OTMacCounters counter(int type);
  OT_FUNC_1_DECL(otError, dataset_active, Dataset, GetActive, otOperationalDataset*);
  OT_FUNC_1_DECL(otError, dataset_pending, Dataset, GetPending, otOperationalDataset*);
  OT_FUNC_1_DECL(otError, dataset_commit_active, Dataset, SetActive, otOperationalDataset*);
  OT_FUNC_1_DECL(otError, dataset_commit_pending, Dataset, SetPending, otOperationalDataset*);
  OT_FUNC_4_DECL(otError, dataset_mgmtget_active, Dataset, SendMgmtActiveGet, otOperationalDatasetComponents*, uint8_t*, uint8_t, otIp6Address*);
  OT_FUNC_4_DECL(otError, dataset_mgmtget_pending, Dataset, SendMgmtPendingGet, otOperationalDatasetComponents*, uint8_t*, uint8_t, otIp6Address*);
  otError dataset_mgmtget_active(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len) { return dataset_mgmtget_active(dataset, tlvs, len, NULL); }
  otError dataset_mgmtget_pending(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len) { return dataset_mgmtget_pending(dataset, tlvs, len, NULL); }
  otError dataset_mgmtget_active(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr);
  otError dataset_mgmtget_pending(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr);
  OT_FUNC_3_DECL(otError, dataset_mgmtset_active, Dataset, SendMgmtActiveSet, otOperationalDataset*, uint8_t*, uint8_t);
  OT_FUNC_3_DECL(otError, dataset_mgmtset_pending, Dataset, SendMgmtPendingSet, otOperationalDataset*, uint8_t*, uint8_t);
#if OPENTHREAD_FTD
  OT_SETGET_DECL(uint32_t, delaytimermin, Dataset, DelayTimerMinimal);
#endif
  // x diag
  otError discover(uint32_t chbits, otHandleActiveScanResult callback, void* context);
  otError discover(otActiveScanResult* table, size_t tablesize, uint32_t chbits=0xFFFFFFFF);
  // x dns
#if OPENTHREAD_FTD
//  int eidcache_num();
//  int _eidcache_num();
//  OT_FUNC_2_DECL(otError, eidcache, Thread, GetEidCacheEntry, int, otEidCacheEntry*);
#endif
  const OTExtAddress eui64();
  // x exit
  // x logfilename
  OT_SETGET_DECL(OTExtAddress, extaddr, Link, ExtendedAddress);
  OT_SETGET_DECL(OTExtendedPanId, extpanid, Thread, ExtendedPanId);
  OT_V_FUNC_0_DECL(factoryreset, Instance, FactoryReset);
  OT_SET_IS_DECL(bool, ifconfig, Ip6, Enabled);
  int ipaddr_num();
  int _ipaddr_num();
  IPAddress ipaddr(int idx=0);
  IPAddress _ipaddr(int idx=0);
  otError ipaddr_add(IPAddress& addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc);
  otError _ipaddr_add(IPAddress& addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc);
  otError ipaddr_del(IPAddress&);
  otError _ipaddr_del(IPAddress&);
  int ipmaddr_num();
  int _ipmaddr_num();
  IPAddress ipmaddr(int idx=0);
  IPAddress _ipmaddr(int idx=0);
  otError ipmaddr_add(IPAddress& addr);
  otError _ipmaddr_add(IPAddress& addr);
  otError ipmaddr_del(IPAddress& addr);
  otError _ipmaddr_del(IPAddress& addr);
#if OPENTHREAD_CONFIG_JOINER_ENABLE
  otError joiner_start(const char* pskc, const char* provision, otJoinerCallback, void*);
  otError _joiner_start(const char* pskc, const char* provision, otJoinerCallback, void*);
  otError joiner_start(const char* pskc, const char* provision=NULL);
  OT_V_FUNC_0_DECL(joiner_stop, Joiner, Stop);
  const OTExtAddress joinerid();
#endif
#if OPENTHREAD_FTD
  OT_GETTER_DECL(uint16_t, joinerport, Thread, JoinerUdpPort);
#endif
  OT_V_SETGET_DECL(uint32_t, keysequencecounter, Thread, KeySequenceCounter);
  OT_V_SETGET_DECL(uint32_t, keyswitchguadtime, Thread, KeySwitchGuardTime);
  OT_FUNC_1_DECL(otError, leaderdata, Thread, GetLeaderData, otLeaderData*);
#if OPENTHREAD_FTD
  OT_V_SETGET_DECL(uint32_t, leaderpartitionid, Thread, LocalLeaderPartitionId);
  OT_V_SETGET_DECL(uint8_t, leaderweight, Thread, LocalLeaderWeight);
#endif
  // x macfilter
  OT_SETGET_DECL(OTMasterKey, masterkey, Thread, MasterKey);
  OT_SETGET_DECL(otLinkModeConfig, mode, Thread, LinkMode);
#if OPENTHREAD_FTD
  otError neighbor(int idx, otNeighborInfo*);
  otError _neighbor(int idx, otNeighborInfo*);
#endif
  // x netdataregister
  // x netdatashow
  // x networkdiagnostic
#if OPENTHREAD_FTD
  OT_V_SETGET_DECL(uint8_t, networkidtimeout, Thread, NetworkIdTimeout);
#endif
  OT_SETGET_DECL(OTNetworkName, networkname, Thread, NetworkName);
  // x networktime
  OT_SETGET_DECL(uint16_t, panid, Link, PanId);
  otError parent(otRouterInfo* parent);
#if OPENTHREAD_FTD
  OT_SETGET_DECL(uint8_t, parentpriority, Thread, ParentPriority);
#endif
  otError ping(IPAddress& addr, const uint8_t*, uint16_t);
  otError _ping(IPAddress& addr, const uint8_t*, uint16_t);
  OT_SETGET_DECL(uint32_t, pollperiod, Link, PollPeriod);
  OT_SET_IS_DECL(bool, promiscuous, Link, Promiscuous);
  otError promiscuous(otLinkPcapCallback, void* ctx=NULL);
  otError _promiscuous(otLinkPcapCallback, void* ctx=NULL);
  // x prefix
#if OPENTHREAD_FTD
  OT_SETGET_DECL(OTPskc, pskc, Thread, Pskc);
  OT_FUNC_1_DECL(otError, releaserouterid, Thread, ReleaseRouterId, uint8_t);
#endif
  OT_V_FUNC_0_DECL(reset, Instance, Reset);
  OT_GETTER_DECL(uint16_t, rloc16, Thread, Rloc16);
  // x route
#if OPENTHREAD_FTD
  OT_FUNC_2_DECL(otError, router, Thread, GetRouterInfo, int, otRouterInfo*);
  OT_V_SETGET_DECL(uint8_t, routerdowngradethreshold, Thread, RouterDowngradeThreshold);
  OT_SET_IS_DECL(bool, routereligible, Thread, RouterEligible);
  OT_V_SETGET_DECL(uint8_t, routerselectionjitter, Thread, RouterSelectionJitter);
  OT_V_SETGET_DECL(uint8_t, routerupgradethreshold, Thread, RouterUpgradeThreshold);
#endif
  OT_FUNC_4_DECL(otError, activescan, Link, ActiveScan, uint16_t, uint32_t, otHandleActiveScanResult, void*);
  OT_FUNC_4_DECL(otError, energyscan, Link, EnergyScan, uint16_t, uint32_t, otHandleEnergyScanResult, void*);
  // x service
  OT_IS_DECL(bool, singleton, Thread, Singleton);
  // x sntp
  OT_GETTER_DECL(otDeviceRole, state, Thread, DeviceRole);
  otError state(otDeviceRole role);
  otError _state(otDeviceRole role);
  OT_SETTER_DECL(bool, thread, Thread, Enabled);
  OT_SETTER_DECL(int8_t, txpower, PlatRadio, TransmitPower);
  OT_FUNC_1_DECL(otError, txpower, PlatRadio, GetTransmitPower, int8_t*);
  int8_t txpower() { int8_t ret; otError err = txpower(&ret); return err ? 0 : ret; }
  inline const char* version() { return otGetVersionString(); }
  inline const char* VersionString() { return otGetVersionString(); }
  inline const char* RadioVersionString() { return otGetRadioVersionString(otrGetInstance()); }
  inline const char* otErrorToString(otError err) { return otThreadErrorToString(err); }
  inline otError SetStateChangedCallback(otStateChangedCallback cb, void* user) {
    return otSetStateChangedCallback(otrGetInstance(), cb, user);
  }
  inline void RemoveStateChangeCallback(otStateChangedCallback cb, void* user) {
    otRemoveStateChangeCallback(otrGetInstance(), cb, user);
  }
#include "OpenThread_APIs.inc"

  OTBackboneRouter BackboneRouter;
  OTBorderAgent BorderAgent;
  OTBorderRouter BorderRouter;
  OTChannelManager ChannelManager;
  OTChannelMonitor ChannelMonitor;
  OTChildSupervision ChildSupervision;
  OTCli Cli;
  OTCoap Coap;
  OTCoapSecure CoapSecure;
  OTCommissioner Commissioner;
  OTCrypto Crypto;
  OTDataset Dataset;
  OTDiag Diag;
  OTDnsClient DnsClient;
  OTEntropy Entropy;
  OTHeap Heap;
  OTIcmp6 Icmp6;
  OTInstance Instance;
  OTIp6 Ip6;
  OTJamDetection JamDetection;
  OTJoiner Joiner;
  OTLink Link;
  OTLogging Logging;
  OTMessage Message;
  OTNcp Ncp;
  OTNetData NetData;
  OTNetworkTime NetworkTime;
  OTPlat Plat;
  OTRandomCrypto RandomCrypto;
  OTRandomNonCrypto RandomNonCrypto;
  OTServer Server;
  OTSntpClient SntpClient;
  OTTasklets Tasklets;
  OTThread Thread;
  OTUdp Udp;
private:

  struct discover_data {
    //struct k_sem sem;
    otError error;
    otActiveScanResult* table;
    size_t size;
    size_t count;
  };
  struct discover_data sync_discover_context;

  static void joiner_start_sync_callback(otError aResult, void *aContext);
  static void discover_sync_callback(otActiveScanResult *aResult, void *aContext);
};

extern OpenThreadClass OpenThread;

#endif
