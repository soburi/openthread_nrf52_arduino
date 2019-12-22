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
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
#include <openthread/border_router.h>
#endif
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/message.h>
#include <openthread/thread.h>
#include <openthread/dataset.h>
#if OPENTHREAD_CONFIG_APPLICATION_COAP_ENABLE
#include <openthread/coap.h>
#endif
#if OPENTHREAD_CONFIG_APPLICATION_COAP_SECURE_ENABLE
#include <openthread/coap_secure.h>
#endif
#if OPENTHREAD_FTD
#include <openthread/thread_ftd.h>
#include <openthread/dataset_ftd.h>
#endif
#if OPENTHREAD_CONFIG_DIAG_ENABLE
#include <openthread/diag.h>
#endif
#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
#include <openthread/commissioner.h>
#endif
#if OPENTHREAD_CONFIG_JOINER_ENABLE
#include <openthread/joiner.h>
#endif
#include <openthread/netdata.h>
#if OPENTHREAD_CONFIG_SERVICE_ENABLE
#include <openthread/server.h>
#endif
#include <openthread/platform/radio.h>
#include <openthread-system.h>
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

#define OT_IS_DECL(ty, na, cat, n2)  const ty na ();
#define OT_GETTER_DECL(ty, na, cat, n2)  const ty na ();
#define OT_SETTER_DECL(ty, na, cat, n2)  const otError na (ty arg);
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
  const ty na()

#define OT_V_FUNC_1_DECL(na, cat, n2, t1) \
  void na(t1)

#define OT_FUNC_1_DECL(ty, na, cat, n2, t1) \
  const ty na(t1)

#define OT_FUNC_2_DECL(ty, na, cat, n2, t1, t2) \
  const ty na(t1, t2)

#define OT_FUNC_3_DECL(ty, na, cat, n2, t1, t2, t3) \
  const ty na(t1, t2, t3)

#define OT_FUNC_4_DECL(ty, na, cat, n2, t1, t2, t3, t4) \
  const ty na(t1, t2, t3, t4)

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

  operator const otError() { return err; }
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
  OT_FUNC_0_DECL(otError, commissioner_start, Commissioner, Start);
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
  int eidcache_num();
  int _eidcache_num();
  OT_FUNC_2_DECL(otError, eidcache, Thread, GetEidCacheEntry, int, otEidCacheEntry*);
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
  inline const char* otErrorToString(otError err) { return otThreadErrorToString(err); }


#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
  class OTBorderRouter {
  friend class OpenThreadClass;
  public:
    otError GetNetData(bool, uint8_t*, uint8_t*);
    otError AddOnMeshPrefix(const otBorderRouterConfig*);
    otError RemoveOnMeshPrefix(const otIp6Prefix*);
    otError GetNextOnMeshPrefix(otNetworkDataIterator*, otBorderRouterConfig*);
    otError AddRoute(const otExternalRouteConfig*);
    otError RemoveRoute(const otIp6Prefix*);
    otError GetNextRoute(otNetworkDataIterator*, otExternalRouteConfig*);
    otError Register();
  };

  OTBorderRouter BorderRouter;
#endif

#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
  class OTCommissioner {
  friend class OpenThreadClass;
  public:
    otError Start(otCommissionerStateCallback, otCommissionerJoinerCallback, void*);
    otError Stop();
    otError AddJoiner(const otExtAddress*, const char*, uint32_t);
    otError RemoveJoiner(const otExtAddress*);
    otError SetProvisioningUrl(const char*);
    otError AnnounceBegin(uint32_t, uint8_t, uint16_t, const otIp6Address*);
    otError EnergyScan(uint32_t, uint8_t, uint16_t, uint16_t, const otIp6Address*, otCommissionerEnergyReportCallback, void*);
    otError PanIdQuery(uint16_t, uint32_t, const otIp6Address*, otCommissionerPanIdConflictCallback, void*);
    otError SendMgmtGet(const uint8_t*, uint8_t);
    otError SendMgmtSet(const otCommissioningDataset*, const uint8_t*, uint8_t);
    uint16_t GetSessionId();
    otCommissionerState GetState();
    otError GeneratePskc(const char*, const char*, const otExtendedPanId*, uint8_t*);
  };

  OTCommissioner Commissioner;
#endif

  class OTDataset {
  friend class OpenThreadClass;
  public:
    bool IsCommissioned();
    otError GetActive(otOperationalDataset*);
    otError SetActive(const otOperationalDataset*);
    otError GetPending(otOperationalDataset*);
    otError SetPending(const otOperationalDataset*);
    otError SendMgmtActiveGet(const otOperationalDatasetComponents*, const uint8_t*, uint8_t, const otIp6Address*);
    otError SendMgmtActiveSet(const otOperationalDataset*, const uint8_t*, uint8_t);
    otError SendMgmtPendingGet(const otOperationalDatasetComponents*, const uint8_t*, uint8_t, const otIp6Address*);
    otError SendMgmtPendingSet(const otOperationalDataset*, const uint8_t*, uint8_t);
    uint32_t GetDelayTimerMinimal();
    otError SetDelayTimerMinimal(uint32_t);
  };

  OTDataset Dataset;

  class OTError {
  friend class OpenThreadClass;
  public:
    inline const char* ErrorToString(otError err) { return otThreadErrorToString(err); }
  };

  OTError Error;

  class OTInstance {
  friend class OpenThreadClass;
  public:
    otError SetStateChangedCallback(otStateChangedCallback, void*);
    void RemoveStateChangeCallback(otStateChangedCallback, void*);
    void Reset();
    void FactoryReset();
    inline const char* GetVersionString(void) { return otGetVersionString(); }
  };

  OTInstance Instance;

  class OTIp6 {
  friend class OpenThreadClass;
  public:
    otError SetEnabled(bool);
    bool IsEnabled();
    otError AddUnicastAddress(const otNetifAddress*);
    otError RemoveUnicastAddress(const otIp6Address*);
    const otNetifAddress* GetUnicastAddresses();

    static inline bool IsAddressEqual(const otIp6Address* a1, const otIp6Address* a2)
    {
      return otIp6IsAddressEqual(a1, a2);
    }
    static inline otError AddressFromString(const char* str, otIp6Address* addr)
    {
      return otIp6AddressFromString(str, addr);
    }
    uint8_t PrefixMatch(const otIp6Address* addr1, const otIp6Address* addr2)
    {
      return otIp6PrefixMatch(addr1, addr2);
    }
  };

  OTIp6 Ip6;

#if OPENTHREAD_CONFIG_JOINER_ENABLE
  class OTJoiner {
  friend class OpenThreadClass;
  public:
    otError Start(const char*, const char*, const char*, const char*, const char*, const char*, otJoinerCallback, void*);
    void Stop();
    otJoinerState GetState();
    void GetId(otExtAddress*);
  };

  OTJoiner Joiner;
#endif

  class OTLink {
  friend class OpenThreadClass;
  public:
    otError ActiveScan(uint32_t, uint16_t, otHandleActiveScanResult, void*);
    bool IsActiveScanInProgress();
    otError EnergyScan(uint32_t, uint16_t, otHandleEnergyScanResult, void*);
    bool IsEnergyScanInProgress();
    otError SendDataRequest();
    bool IsInTransmitState();
    otError OutOfBandTransmitRequest(otRadioFrame*);
    uint8_t GetChannel();
    otError SetChannel(uint8_t);
    const otExtAddress* GetExtendedAddress();
    otError SetExtendedAddress(const otExtAddress*);
    void GetFactoryAssignedIeeeEui64(otExtAddress*);
    otPanId GetPanId();
    otError SetPanId(otPanId);
    uint32_t GetPollPeriod();
    otError SetPollPeriod(uint32_t);
    otShortAddress GetShortAddress();
#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE
    otMacFilterAddressMode FilterGetAddressMode();
    otError FilterSetAddressMode(otMacFilterAddressMode);
    otError FilterAddAddress(const otExtAddress*);
    otError FilterRemoveAddress(const otExtAddress*);
    void FilterClearAddresses();
    otError FilterGetNextAddress(otMacFilterIterator*, otMacFilterEntry*);
    otError FilterAddRssIn(const otExtAddress*, int8_t);
    otError FilterRemoveRssIn(const otExtAddress*);
    void FilterClearRssIn();
    otError FilterGetNextRssIn(otMacFilterIterator*, otMacFilterEntry*);
#endif
    const otMacCounters* GetCounters();
  };

  OTLink Link;

  class OTMessage {
  friend class OpenThreadClass;
  public:
    void GetBufferInfo(otBufferInfo*);
  };

  OTMessage Message;

  class OTNetData {
  friend class OpenThreadClass;
  public:
    otError Get(bool, uint8_t*, uint8_t*);
    otError GetNextOnMeshPrefix(otNetworkDataIterator*, otBorderRouterConfig*);
    uint8_t GetVersion();
    uint8_t GetStableVersion();
  };

  OTNetData NetData;

#if OPENTHREAD_CONFIG_SERVICE_ENABLE
  class OTServer {
  friend class OpenThreadClass;
  public:
    otError GetNetDataLocal(bool, uint8_t*, uint8_t*);
    otError AddService(const otServiceConfig*);
    otError RemoveService(uint32_t, uint8_t*, uint8_t);
    otError GetNextService(otNetworkDataIterator*, otServiceConfig*);
    otError Register();
  };

  OTServer Server;
#endif

  class OTThread {
  friend class OpenThreadClass;
  public:
    otError SetEnabled(bool aEnabled);
    bool IsSingleton();

    otError Discover(uint32_t aScanChannels, uint16_t aPanId, bool aJoiner, bool aEnableEui64Filtering, otHandleActiveScanResult aCallback, void* aCallbackContext);

    bool IsDiscoverInProgress();
    uint32_t GetChildTimeout();
    void SetChildTimeout(uint32_t aTimeout);
    const otExtendedPanId* GetExtendedPanId();
    otError SetExtendedPanId(const otExtendedPanId *aExtendedPanId);
    otError GetLeaderRloc(otIp6Address *aLeaderRloc);
    otLinkModeConfig GetLinkMode();
    otError SetLinkMode(otLinkModeConfig aConfig);
    const otMasterKey* GetMasterKey();
    otError SetMasterKey(const otMasterKey *aKey);
    const otIp6Address* GetMeshLocalEid();
    const otMeshLocalPrefix* GetMeshLocalPrefix();
    otError SetMeshLocalPrefix(const otMeshLocalPrefix *aMeshLocalPrefix);
    const char* GetNetworkName();
    otError SetNetworkName(const char *aNetworkName);
    uint32_t GetKeySequenceCounter();
    void SetKeySequenceCounter(uint32_t aKeySequenceCounter);
    uint32_t GetKeySwitchGuardTime();
    void SetKeySwitchGuardTime(uint32_t aKeySwitchGuardTime);
    otError BecomeDetached();
    otError BecomeChild();
    otError GetNextNeighborInfo(otNeighborInfoIterator *aIterator, otNeighborInfo * aInfo);
    otDeviceRole GetDeviceRole();
    otError GetLeaderData(otLeaderData *aLeaderData);
    uint8_t GetLeaderRouterId();
    uint8_t GetLeaderWeight();
    uint32_t GetPartitionId();
    uint16_t GetRloc16();
    otError GetParentInfo(otRouterInfo *aParentInfo);
    otError GetParentAverageRssi(int8_t *aParentRssi);
    otError GetParentLastRssi(int8_t *aLastRssi);
    otError SendDiagnosticGet(const otIp6Address *aDestination, const uint8_t aTlvTypes[], uint8_t aCount);
    otError SendDiagnosticReset(const otIp6Address *aDestination, const uint8_t aTlvTypes[], uint8_t aCount);
    const otIpCounters* GetIp6Counters();

    void RegisterParentResponseCallback(otThreadParentResponseCallback aCallback, void* aContext);

    uint8_t GetMaxAllowedChildren();
    otError SetMaxAllowedChildren(uint8_t aMaxChildren);
    otError SetPreferredRouterId(uint8_t aRouterId);
    uint8_t GetLocalLeaderWeight();
    void SetLocalLeaderWeight(uint8_t aWeight);
    uint32_t GetLocalLeaderPartitionId();
    void SetLocalLeaderPartitionId(uint32_t aPartitionId);
    uint16_t GetJoinerUdpPort();
    otError SetJoinerUdpPort(uint16_t aJoinerUdpPort);
    uint32_t GetContextIdReuseDelay();
    void SetContextIdReuseDelay(uint32_t aDelay);
    uint8_t GetNetworkIdTimeout();
    void SetNetworkIdTimeout(uint8_t aTimeout);
    uint8_t GetRouterUpgradeThreshold();
    void SetRouterUpgradeThreshold(uint8_t aThreshold);
    otError ReleaseRouterId(uint8_t aRouterId);
    otError BecomeRouter();
    otError BecomeLeader();
    uint8_t GetRouterDowngradeThreshold();
    void SetRouterDowngradeThreshold(uint8_t aThreshold);
    uint8_t GetRouterSelectionJitter();
    void SetRouterSelectionJitter(uint8_t aRouterJitter);
#if OPENTHREAD_FTD
    otError GetChildInfoById(uint16_t aChildId, otChildInfo *aChildInfo);
    otError GetChildInfoByIndex(uint8_t aChildIndex, otChildInfo *aChildInfo);
#endif
    uint8_t GetRouterIdSequence();
    uint8_t GetMaxRouterId();
    otError GetRouterInfo(uint16_t aRouterId, otRouterInfo *aRouterInfo);
#if OPENTHREAD_FTD
    otError GetEidCacheEntry(uint8_t aIndex, otEidCacheEntry *aEntry);
#endif
    const otPskc* GetPskc();
    otError SetPskc(const otPskc *aPskc);
    int8_t GetParentPriority();
    otError SetParentPriority(int8_t aParentPriority);
  };

  OTThread Thread;

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
