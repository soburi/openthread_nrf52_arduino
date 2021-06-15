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

#pragma once

#include <Print.h>
#include <IPAddress.h>

#include <openthread/thread.h>
#include <openthread/dataset.h>
#include <openthread/netdata.h>
#include <openthread/ip6.h>
#if OPENTHREAD_FTD
#include <openthread/thread_ftd.h>
#include <openthread/dataset_ftd.h>
#endif

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
  operator const T*() const { return value; }
  operator T*() { return const_cast<T*>(value); }
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
  operator const T*() const { return value; }
  operator T*() { return const_cast<T*>(value); }
  operator const char*() { return reinterpret_cast<const char*>(value->m8); }
private:
  const T* value;
};

typedef class OTm8Buffer<otExtAddress> OTExtAddress;
typedef class OTm8Buffer<otExtendedPanId> OTExtendedPanId;
typedef class OTm8Buffer<otMasterKey> OTMasterKey;
typedef class OTm8String<otNetworkName> OTNetworkName;
typedef class OTm8Buffer<otPskc> OTPskc;
typedef class OTm8Buffer<otMeshLocalPrefix> OTMeshLocalPrefix;

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
  OTMacCounters() : value(&default_value) { }
  OTMacCounters(const otMacCounters* v) : value(v) { }
  const otMacCounters* operator->() { return value; }
  otMacCounters operator*() { return *value; }

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
  size_t printTo(Print& p) const;
private:
  static otMacCounters default_value;
  const otMacCounters* value;
};

class OTMleCounters : public Printable {
public:
  OTMleCounters() : value(&default_value) { }
  OTMleCounters(const otMleCounters* v) : value(v) { }

  operator const otMleCounters*() const { return value; }
  const otMleCounters* operator->() { return value; }
  otMleCounters operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otMleCounters default_value;
  const otMleCounters* value;
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

#if OPENTHREAD_FTD
class OTChildInfo : public Printable {
public:
  OTChildInfo() : value(&default_value) { }
  OTChildInfo(const otChildInfo* v) : value(v) { }
  operator const otChildInfo*() const { return value; }
  operator otChildInfo*() { return const_cast<otChildInfo*>(value); }
  const otChildInfo* operator->() { return value; }
  otChildInfo operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otChildInfo default_value;
  const otChildInfo* value;
};
#endif

class OTExternalRouteConfig : public Printable {
public:
  OTExternalRouteConfig() : value(&default_value) { }
  OTExternalRouteConfig(const otExternalRouteConfig* v) : value(v) { }
  operator const otExternalRouteConfig*() const { return value; }
  operator otExternalRouteConfig*() { return const_cast<otExternalRouteConfig*>(value); }
  const otExternalRouteConfig* operator->() { return value; }
  otExternalRouteConfig operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otExternalRouteConfig default_value;
  const otExternalRouteConfig* value;
};

class OTRouterInfo : public Printable {
public:
  OTRouterInfo() : value(&default_value) { }
  OTRouterInfo(const otRouterInfo* v) : value(v) { }
  operator const otRouterInfo*() const { return value; }
  operator otRouterInfo*() { return const_cast<otRouterInfo*>(value); }
  const otRouterInfo* operator->() { return value; }
  otRouterInfo operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otRouterInfo default_value;
  const otRouterInfo* value;
};

#if OPENTHREAD_FTD
class OTCacheEntryInfo : public Printable {
public:
  OTCacheEntryInfo() : value(&default_value) { }
  OTCacheEntryInfo(const otCacheEntryInfo* v) : value(v) { }
  operator const otCacheEntryInfo*() const { return value; }
  operator otCacheEntryInfo*() { return const_cast<otCacheEntryInfo*>(value); }
  const otCacheEntryInfo* operator->() { return value; }
  otCacheEntryInfo operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otCacheEntryInfo default_value;
  const otCacheEntryInfo* value;
};
#endif

class OTNeighborInfo : public Printable {
public:
  OTNeighborInfo() : value(&default_value) { }
  OTNeighborInfo(const otNeighborInfo* v) : value(v) { }
  operator const otNeighborInfo*() const { return value; }
  operator otNeighborInfo*() { return const_cast<otNeighborInfo*>(value); }
  const otNeighborInfo* operator->() { return value; }
  otNeighborInfo operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otNeighborInfo default_value;
  const otNeighborInfo* value;
};

class OTBorderRouterConfig : public Printable {
public:
  OTBorderRouterConfig() : value(&default_value) { }
  OTBorderRouterConfig(const otBorderRouterConfig* v) : value(v) { }
  operator const otBorderRouterConfig*() const { return value; }
  operator otBorderRouterConfig*() { return const_cast<otBorderRouterConfig*>(value); }
  const otBorderRouterConfig* operator->() { return value; }
  otBorderRouterConfig operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otBorderRouterConfig default_value;
  const otBorderRouterConfig* value;
};

class OTMacFilterEntry : public Printable {
public:
  OTMacFilterEntry() : value(&default_value) { }
  OTMacFilterEntry(const otMacFilterEntry* v) : value(v) { }
  operator const otMacFilterEntry*() const { return value; }
  operator otMacFilterEntry*() { return const_cast<otMacFilterEntry*>(value); }
  const otMacFilterEntry* operator->() { return value; }
  otMacFilterEntry operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otMacFilterEntry default_value;
  const otMacFilterEntry* value;
};

class OTNetifAddress : public Printable {
public:
  OTNetifAddress() : value(&default_value) { }
  OTNetifAddress(const otNetifAddress* v) : value(v) { }
  operator const otNetifAddress*() const { return value; }
  operator otNetifAddress*() { return const_cast<otNetifAddress*>(value); }
  const otNetifAddress* operator->() { return value; }
  otNetifAddress operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otNetifAddress default_value;
  const otNetifAddress* value;
};

class OTNetifMulticastAddress : public Printable {
public:
  OTNetifMulticastAddress() : value(&default_value) { }
  OTNetifMulticastAddress(const otNetifMulticastAddress* v) : value(v) { }
  operator const otNetifMulticastAddress*() const { return value; }
  operator otNetifMulticastAddress*() { return const_cast<otNetifMulticastAddress*>(value); }
  const otNetifMulticastAddress* operator->() { return value; }
  otNetifMulticastAddress operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otNetifMulticastAddress default_value;
  const otNetifMulticastAddress* value;
};


class OTSecurityPolicy : public Printable {
public:
  OTSecurityPolicy() : value(&default_value) { }
  OTSecurityPolicy(const otSecurityPolicy* v) : value(v) { }
  operator const otSecurityPolicy*() const { return value; }
  operator otSecurityPolicy*() { return const_cast<otSecurityPolicy*>(value); }
  const otSecurityPolicy* operator->() { return value; }
  otSecurityPolicy operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otSecurityPolicy default_value;
  const otSecurityPolicy* value;
};

class OTActiveScanResult : public Printable {
public:
  OTActiveScanResult() : value(&default_value) { }
  OTActiveScanResult(const otActiveScanResult* v) : value(v) { }
  operator const otActiveScanResult*() const { return value; }
  operator otActiveScanResult*() { return const_cast<otActiveScanResult*>(value); }
  const otActiveScanResult* operator->() { return value; }
  otActiveScanResult operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otActiveScanResult default_value;
  const otActiveScanResult* value;
};

class OTEnergyScanResult : public Printable {
public:
  OTEnergyScanResult() : value(&default_value) { }
  OTEnergyScanResult(const otEnergyScanResult* v) : value(v) { }
  operator const otEnergyScanResult*() const { return value; }
  operator otEnergyScanResult*() { return const_cast<otEnergyScanResult*>(value); }
  const otEnergyScanResult* operator->() { return value; }
  otEnergyScanResult operator*() { return *value; }

  size_t printTo(Print& p) const;
private:
  static otEnergyScanResult default_value;
  const otEnergyScanResult* value;
};


#if OPENTHREAD_FTD
class OTChildIterator {
  otError error;
  uint16_t index;
  otChildInfo entry;

public:
  OTChildIterator() : index(0) {}
  OTChildIterator(const uint16_t i) { index = i; }
  OTChildIterator& operator=(const uint16_t i) { index = i; return *this; }
  bool end() { return error; }
  OTChildInfo get() { return (error ? nullptr : &entry); }
  OTChildInfo next();
};

class OTChildIpIterator {
  otError error;
  otChildIp6AddressIterator iter;
  otIp6Address entry;
  uint16_t index;

public:
  OTChildIpIterator(uint16_t i) : iter(OT_CHILD_IP6_ADDRESS_ITERATOR_INIT), index(i) {}
  bool end() { return error; }
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  IPAddress get() { return (error ? nullptr : entry.mFields.m16); }
#pragma GCC diagnostic pop
  IPAddress next();
};
#endif

class OTRouteIterator {
  otError error;
  otNetworkDataIterator iter;
  otExternalRouteConfig entry;

public:
  OTRouteIterator() : iter(OT_NETWORK_DATA_ITERATOR_INIT) {}
  OTRouteIterator(const otNetworkDataIterator i) { iter = i; }
  OTRouteIterator& operator=(const otNetworkDataIterator i) { iter = i; return *this; }
  bool end() { return error; }
  OTExternalRouteConfig get() { return (error ? nullptr : &entry); }

  OTExternalRouteConfig next();
};

class OTRouterIterator {
  otError error;
  uint16_t index;
  otRouterInfo entry;

public:
  OTRouterIterator() : index(0) {}
  bool end() { return error; }
  OTRouterIterator(const uint16_t i) { index = i; }
  OTRouterIterator& operator=(const uint16_t i) { index = i; return *this; }
  OTRouterInfo get() { return (error ? nullptr : &entry); }

  OTRouterInfo next();
};

#if OPENTHREAD_FTD
class OTEidCacheIterator {
  otError error;
  otCacheEntryIterator iter;
  otCacheEntryInfo entry;

public:
  bool end() { return error; }
  OTEidCacheIterator(const otCacheEntryIterator& i) { iter = i; }
  OTEidCacheIterator& operator=(const otCacheEntryIterator& i) { iter = i; return *this; }
  OTCacheEntryInfo get() { return (error ? nullptr : &entry); }
  OTCacheEntryInfo next();
};
#endif

class OTNeighborIterator {
  otError error;
  otNeighborInfoIterator iter;
  otNeighborInfo entry;

public:
  OTNeighborIterator() : iter(OT_NEIGHBOR_INFO_ITERATOR_INIT) {}
  OTNeighborIterator(const otNeighborInfoIterator& i) : iter(i) {}
  OTNeighborIterator& operator=(const otNeighborInfoIterator& i) { iter = i; return *this; }
  bool end() { return error; }
  OTNeighborInfo get() { return (error ? nullptr : &entry); }

  OTNeighborInfo next();
};

class OTPrefixIterator {
  otError error;
  otNetworkDataIterator iter;
  otBorderRouterConfig entry;

public:
  OTPrefixIterator() : iter(OT_NETWORK_DATA_ITERATOR_INIT) {}
  OTPrefixIterator(const otNetworkDataIterator i) { iter = i; }
  OTPrefixIterator& operator=(const otNetworkDataIterator i) { iter = i; return *this; }
  bool end() { return error; }
  OTBorderRouterConfig get() { return (error ? nullptr : &entry); }
  OTBorderRouterConfig next();
};

class OTMacFilterAddrIterator {
  otError error;
  otMacFilterIterator iter;
  otMacFilterEntry entry;

public:
  OTMacFilterAddrIterator() : iter(OT_MAC_FILTER_ITERATOR_INIT) {}
  OTMacFilterAddrIterator(const otMacFilterIterator& i) { iter = i; }
  OTMacFilterAddrIterator& operator=(const otMacFilterIterator& i) { iter = i; return *this; }
  bool end() { return error; }
  OTMacFilterEntry get() { return (error ? nullptr : &entry); }
  OTMacFilterEntry next();
};

class OTMacFilterRssIterator {
  otError error;
  otMacFilterIterator iter;
  otMacFilterEntry entry;

public:
  OTMacFilterRssIterator() : iter(OT_MAC_FILTER_ITERATOR_INIT) {}
  OTMacFilterRssIterator(const otMacFilterIterator& i) { iter = i; }
  OTMacFilterRssIterator& operator=(const otMacFilterIterator& i) { iter = i; return *this; }
  bool end() { return error; }
  OTMacFilterEntry get() { return (error ? nullptr : &entry); }
  OTMacFilterEntry next();
};

class OTMacFilterIterator {
  otError error;
  otMacFilterIterator iter;
  otMacFilterEntry entry;
  bool addr_end;

  void _next();
public:
  OTMacFilterIterator() : iter(OT_MAC_FILTER_ITERATOR_INIT) {}
  OTMacFilterIterator(const otMacFilterIterator& i) { iter = i; }
  OTMacFilterIterator& operator=(const otMacFilterIterator& i) { iter = i; return *this; }
  bool end() { return error; }
  OTMacFilterEntry get() { return (error ? nullptr : &entry); }
  OTMacFilterEntry next();
};

class OTIpaddrIterator {
  const otNetifAddress* addr;

public:
  OTIpaddrIterator();
  OTIpaddrIterator(const otNetifAddress* a) { addr = a; }
  OTIpaddrIterator& operator=(const otNetifAddress* a) { addr = a; return *this; }
  bool end() { return !addr; }
  OTNetifAddress get() { return addr; }
  OTNetifAddress next() { addr = addr->mNext; return addr; }
};

class OTIpmaddrIterator{
  const otNetifMulticastAddress* addr;

public:
  OTIpmaddrIterator();
  OTIpmaddrIterator(const otNetifMulticastAddress* a) { addr = a; }
  OTIpmaddrIterator& operator=(const otNetifMulticastAddress* a) { addr = a; return *this; }
  bool end() { return !addr; }
  OTNetifMulticastAddress get() { return addr; }
  OTNetifMulticastAddress next() { addr = addr->mNext; return addr; }
};

#include "Arduino.h"

struct OTActiveScanResultInfo {
  otActiveScanResult result;
  otError error;
  bool done;
};

class OTActiveScanResultSyncIterator {
  struct OTActiveScanResultInfo data[2];
  int current = 0;
public:
  SemaphoreHandle_t scan_sem;

  OTActiveScanResultSyncIterator();
  void set_error(otError e);
  void receive(otActiveScanResult* result);
  bool end();
  OTActiveScanResult get();
  OTActiveScanResult next();
};

struct OTEnergyScanResultInfo {
  otEnergyScanResult result;
  otError error;
};


class OTEnergyScanResultSyncIterator {
  struct OTEnergyScanResultInfo data[17];
  int current = 0;
  int write_current = 0;
public:
  SemaphoreHandle_t scan_sem;

  OTEnergyScanResultSyncIterator();
  void set_error(otError e);
  void receive(otEnergyScanResult* result);
  bool end();
  OTEnergyScanResult get();
  OTEnergyScanResult next();
};


class OTJoinerContext {
public:
  otError error;
  SemaphoreHandle_t join_sem;
  OTJoinerContext();
};
