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

#include "OpenThread.h"

#include <openthread/openthread-freertos.h>

#include <Print.h>

#include <openthread/config.h>
#include <openthread/commissioner.h>
#include <openthread/icmp6.h>
#include <openthread/joiner.h>
#include <openthread/link.h>
#if OPENTHREAD_CONFIG_ENABLE_TIME_SYNC
#include <openthread/network_time.h>

#if OPENTHREAD_ENABLE_FTD
#include <openthread/dataset_ftd.h>
#include <openthread/thread_ftd.h>
#endif

#if OPENTHREAD_ENABLE_BORDER_ROUTER
#include <openthread/border_router.h>
#endif
#if OPENTHREAD_ENABLE_SERVICE
#include <openthread/server.h>
#endif
#endif

#include <FreeRTOS.h>
#include <task.h>

#include "Arduino.h"

#define COMMISSION_BIT (1 << 1)

int OpenThreadClass::begin()
{
  return 1;
}

#define HDR(x) #x "="
#define OTFUN(x) OpenThread.x()
#define DUMP(x) n += p.print(HDR(x)); n += p.println(OTFUN(x))
#define DUMPF(x, f) n += p.print(HDR(x)); n += p.println(OTFUN(x), f)

int OpenThreadClass::dump(Print& p)
{
  int n = 0;
  //DUMP(networkname);
  OTExtendedPanId epan = extpanid();
  n += p.print("extpanid="); n += p.println(epan);
  DUMPF(panid, HEX);
  OTExtAddress eadr = extaddr();
  n += p.print("extaddr="); n += p.println(eadr);

  OTExtAddress e64 = eui64();
  n += p.print("eui64="); n += p.println(e64);

#if OPENTHREAD_ENABLE_FTD
  OTPskc pk = pskc();
  n += p.print("pskc="); n += p.println(pk);
#endif
  OTMasterKey mkey = masterkey();
  n += p.print("masterkey="); n += p.println(mkey);
  DUMP(channel);
  DUMPF(rloc16, HEX);
  DUMP(ipaddr);
  DUMP(version);

#if OPENTHREAD_ENABLE_COMMISSIONER
  DUMP(commissioner);
#endif
  DUMP(ifconfig);
  OTLinkModeConfig lmode = mode();
  n += p.print("mode="); n += p.println(lmode);
  DUMP(promiscuous);
#if OPENTHREAD_ENABLE_FTD
  DUMP(routerrole);
#endif
  DUMP(state);
  DUMP(singleton);
  DUMP(txpower);

  return n;
}

#define OT_API_CALL_RET(ty, ...) \
    { ty ret; \
    do                       \
    {                        \
        otrLock();           \
        ret = __VA_ARGS__;   \
        otrUnlock();         \
        otrTaskNotifyGive(); \
    } while (0);             \
    return ret; }


#define OT_GETTER_IMPL(ty, na, cat, n2)  \
const ty OpenThreadClass::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, Get ## n2) ); \
}

#define OT_V_FUNC_0_IMPL(na, cat, n2)  \
void OpenThreadClass::  na  () \
{ \
  OT_API_CALL( OT_CALL_FUNC0(cat, n2) ); \
}

#define OT_FUNC_0_IMPL(ty, na, cat, n2)  \
const ty OpenThreadClass::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, n2) ); \
}

#define OT_V_FUNC_1_IMPL(na, cat, n2, t1)  \
void OpenThreadClass::  na  (t1 a1) \
{ \
  OT_API_CALL( OT_CALL_FUNC1(cat, n2) ); \
}

#define OT_FUNC_1_IMPL(ty, na, cat, n2, t1)  \
const ty OpenThreadClass::  na  (t1 a1) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC1(cat, n2) ); \
}

#define OT_FUNC_2_IMPL(ty, na, cat, n2, t1, t2)  \
const ty OpenThreadClass::  na  (t1 a1, t2 a2) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC2(cat, n2) ); \
}

#define OT_FUNC_3_IMPL(ty, na, cat, n2, t1, t2, t3)  \
const ty OpenThreadClass::  na  (t1 a1, t2 a2, t3 a3) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC3(cat, n2) ); \
}

#define OT_FUNC_4_IMPL(ty, na, cat, n2, t1, t2, t3, t4)  \
const ty OpenThreadClass::  na  (t1 a1, t2 a2, t3 a3, t4 a4) \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC4(cat, n2) ); \
}

#define OT_IS_IMPL(ty, na, cat, n2)  \
const ty OpenThreadClass::  na  () \
{ \
  OT_API_CALL_RET(ty, OT_CALL_FUNC0(cat, Is ## n2) ); \
}

#define OT_SETTER_IMPL(ty, na, cat, n2)  \
const otError OpenThreadClass:: na (ty a1) \
{ \
  OT_API_CALL_RET(otError, OT_CALL_FUNC1(cat, Set ## n2) ); \
}

#define OT_V_SETTER_IMPL(ty, na, cat, n2)  \
void OpenThreadClass:: na (ty a1) \
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
OT_SETGET_IMPL(uint8_t, channel, Link, Channel);
#if OPENTHREAD_ENABLE_FTD
OT_FUNC_2_IMPL(otError, child, Thread, GetChildInfoByIndex, int, otChildInfo*);
OT_SETGET_IMPL(uint8_t, childmax, Thread, MaxAllowedChildren);
#endif
OT_V_SETGET_IMPL(uint32_t, childtimeout, Thread, ChildTimeout);
// x coap
// x coaps
#if OPENTHREAD_ENABLE_COMMISSIONER
OT_GETTER_IMPL(otCommissionerState, commissioner, Commissioner, State);
OT_FUNC_0_IMPL(otError, commissioner_start, Commissioner, Start);
OT_FUNC_0_IMPL(otError, commissioner_stop, Commissioner, Stop);
OT_SETTER_IMPL(const char*, commissioner_provisioningurl, Commissioner, ProvisioningUrl);
//otError commissioner_announce(uint32_t mask, uint8_t count, uint16_t period, IPAddress& addr);
//otError commissioner_energy(uint32_t mask, uint8_t count, uint16_t period, uint16_t duration, IPAddress& addr, otCommissionerEnergyReportCallback cb, void* ctx);
//otError commissioner_panid(uint16_t, uint32_t, IPAddress&, otCommissionerPanIdConflictCallback, void* ctx);
OT_GETTER_IMPL(uint16_t, commissioner_sessionid, Commissioner, SessionId);
#endif
#if OPENTHREAD_ENABLE_FTD
OT_V_SETGET_IMPL(uint32_t, contextreusedelay, Thread, ContextIdReuseDelay);
#endif
//uint32_t counter(int type);
OT_FUNC_1_IMPL(otError, dataset_active, Dataset, GetActive, otOperationalDataset*);
OT_FUNC_1_IMPL(otError, dataset_pending, Dataset, GetPending, otOperationalDataset*);
OT_FUNC_1_IMPL(otError, dataset_commit_active, Dataset, SetActive, otOperationalDataset*);
OT_FUNC_1_IMPL(otError, dataset_commit_pending, Dataset, SetPending, otOperationalDataset*);
OT_FUNC_4_IMPL(otError, dataset_mgmtget_active, Dataset, SendMgmtActiveGet, otOperationalDatasetComponents*, uint8_t*, uint8_t, otIp6Address*);
OT_FUNC_4_IMPL(otError, dataset_mgmtget_pending, Dataset, SendMgmtPendingGet, otOperationalDatasetComponents*, uint8_t*, uint8_t, otIp6Address*);
//otError dataset_mgmtget_active(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len) { return dataset_mgmtget_active(dataset, tlvs, len, NULL); }
//otError dataset_mgmtget_pending(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len) { return dataset_mgmtget_pending(dataset, tlvs, len, NULL); }
//otError dataset_mgmtget_active(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr);
//otError dataset_mgmtget_pending(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr);
OT_FUNC_3_IMPL(otError, dataset_mgmtset_active, Dataset, SendMgmtActiveSet, otOperationalDataset*, uint8_t*, uint8_t);
OT_FUNC_3_IMPL(otError, dataset_mgmtset_pending, Dataset, SendMgmtPendingSet, otOperationalDataset*, uint8_t*, uint8_t);
#if OPENTHREAD_ENABLE_FTD
OT_SETGET_IMPL(uint32_t, delaytimermin, Dataset, DelayTimerMinimal);
#endif
// x diag
//otError discover(uint32_t chbits, otHandleActiveScanResult callback, void* context);
// x dns
//int eidcache_num();
#if OPENTHREAD_ENABLE_FTD
OT_FUNC_2_IMPL(otError, eidcache, Thread, GetEidCacheEntry, int, otEidCacheEntry*);
#endif
//OTExtAddress eui64();
// x exit
// x logfilename
OT_SETGET_IMPL(OTExtAddress, extaddr, Link, ExtendedAddress);
OT_SETGET_IMPL(OTExtendedPanId, extpanid, Thread, ExtendedPanId);
OT_V_FUNC_0_IMPL(factoryreset, Instance, FactoryReset);
OT_SET_IS_IMPL(bool, ifconfig, Ip6, Enabled);
// x instance
// x instancelist
//int ipaddr_num();
//IPAddress ipaddr(int idx=0);
//otError ipaddr_add(IPAddress& addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc);
//otError ipaddr_del(IPAddress&);
//int ipmaddr_num();
//IPAddress ipmaddr(int idx=0);
//otError ipmaddr_add(IPAddress& addr);
//otError ipmaddr_del(IPAddress& addr);
#if OPENTHREAD_ENABLE_JOINER
//otError joiner_start(char* pskc, char* provision, otJoinerCallback, void*);
OT_V_FUNC_0_IMPL(joiner_stop, Joiner, Stop);
//OTExtaddress joinerid();
#endif
#if OPENTHREAD_ENABLE_FTD
OT_GETTER_IMPL(uint16_t, joinerport, Thread, JoinerUdpPort);
#endif
OT_V_SETGET_IMPL(uint32_t, keysequencecounter, Thread, KeySequenceCounter);
OT_V_SETGET_IMPL(uint32_t, keyswitchguadtime, Thread, KeySwitchGuardTime);
OT_FUNC_1_IMPL(otError, leaderdata, Thread, GetLeaderData, otLeaderData*);
#if OPENTHREAD_ENABLE_FTD
OT_V_SETGET_IMPL(uint32_t, leaderpartitionid, Thread, LocalLeaderPartitionId);
OT_V_SETGET_IMPL(uint8_t, leaderweight, Thread, LocalLeaderWeight);
#endif
// x macfilter
OT_SETGET_IMPL(OTMasterKey, masterkey, Thread, MasterKey);
OT_SETGET_IMPL(otLinkModeConfig, mode, Thread, LinkMode);
#if OPENTHREAD_ENABLE_FTD
//otError neighbor(int idx, otNeighborInfo*);
#endif
// x neighborregister
// x neighborshow
// x networkdiagnostic
#if OPENTHREAD_ENABLE_FTD
OT_V_SETGET_IMPL(uint8_t, networkidtimeout, Thread, NetworkIdTimeout);
#endif
//OT_GETTER_IMPL(OTNetworkName, networkname, Thread, NetworkName);
// x networktime
OT_SETGET_IMPL(uint16_t, panid, Link, PanId);
//otError parent(otRouterInfo* parent);
#if OPENTHREAD_ENABLE_FTD
OT_SETGET_IMPL(uint8_t, parentpriority, Thread, ParentPriority);
#endif
//otError ping(IPAddress& addr, const uint8_t*, uint16_t);
OT_SETGET_IMPL(uint32_t, pollperiod, Link, PollPeriod);
OT_SET_IS_IMPL(bool, promiscuous, Link, Promiscuous);
//otError promiscuous(otLinkPcapCallback, void* ctx=NULL);
// x prefix
#if OPENTHREAD_ENABLE_FTD
//OT_SETGET_IMPL(uint8_t*, pskc, Thread, Pskc);
OT_FUNC_1_IMPL(otError, releaserouterid, Thread, ReleaseRouterId, uint8_t);
#endif
OT_V_FUNC_0_IMPL(reset, Instance, Reset);
OT_GETTER_IMPL(uint16_t, rloc16, Thread, Rloc16);
// x route
#if OPENTHREAD_ENABLE_FTD
OT_FUNC_2_IMPL(otError, router, Thread, GetRouterInfo, int, otRouterInfo*);
OT_V_SETGET_IMPL(uint8_t, routerdowngradethreshold, Thread, RouterDowngradeThreshold);
OT_V_SET_IS_IMPL(bool, routerrole, Thread, RouterRoleEnabled);
OT_V_SETGET_IMPL(uint8_t, routerselectionjitter, Thread, RouterSelectionJitter);
OT_V_SETGET_IMPL(uint8_t, routerupgradethreshold, Thread, RouterUpgradeThreshold);
#endif
OT_FUNC_4_IMPL(otError, activescan, Link, ActiveScan, uint16_t, uint32_t, otHandleActiveScanResult, void*);
OT_FUNC_4_IMPL(otError, energyscan, Link, EnergyScan, uint16_t, uint32_t, otHandleEnergyScanResult, void*);
OT_IS_IMPL(bool, singleton, Thread, Singleton);
// x sntp
OT_GETTER_IMPL(otDeviceRole, state, Thread, DeviceRole);
//otError state(otDeviceRole role);
OT_SETTER_IMPL(bool, thread, Thread, Enabled);
OT_SETTER_IMPL(int8_t, txpower, PlatRadio, TransmitPower);
OT_FUNC_1_IMPL(otError, txpower, PlatRadio, GetTransmitPower, int8_t*);
//const char* version() { return otGetVersionString(); }


#if OPENTHREAD_ENABLE_COMMISSIONER
otError OpenThreadClass::commissioner_announce(uint32_t mask, uint8_t count, uint16_t period, IPAddress& addr)
{
  OT_API_CALL_RET(otError,
    _commissioner_announce(mask, count, period, addr)
  );
}

otError OpenThreadClass::_commissioner_announce(uint32_t mask, uint8_t count, uint16_t period, IPAddress& addr)
{
  struct otIp6Address address;
  otError error;
  OT_COPY_IP6(address, addr);
  error = otCommissionerAnnounceBegin(otrGetInstance(), mask, count, period, &address);
  return error;
}
otError OpenThreadClass::commissioner_energy(uint32_t mask, uint8_t count, uint16_t period, uint16_t duration, IPAddress& addr, otCommissionerEnergyReportCallback cb, void* ctx)
{
  OT_API_CALL_RET(otError,
    _commissioner_energy(mask, count, period, duration, addr, cb, ctx)
  );
}
otError OpenThreadClass::_commissioner_energy(uint32_t mask, uint8_t count, uint16_t period, uint16_t duration, IPAddress& addr, otCommissionerEnergyReportCallback cb, void* ctx)
{
  struct otIp6Address address;
  otError error;
  OT_COPY_IP6(address, addr);
  error = otCommissionerEnergyScan(otrGetInstance(), mask, count, period, duration, &address, cb, ctx);
  return error;
}
otError OpenThreadClass::commissioner_panid(uint16_t panid, uint32_t mask, IPAddress& addr, otCommissionerPanIdConflictCallback cb, void* ctx)
{
  OT_API_CALL_RET(otError,
    _commissioner_panid(panid, mask, addr, cb, ctx)
  );
}
otError OpenThreadClass::_commissioner_panid(uint16_t panid, uint32_t mask, IPAddress& addr, otCommissionerPanIdConflictCallback cb, void* ctx)
{
  struct otIp6Address address;
  otError error;
  OT_COPY_IP6(address, addr);
  error = otCommissionerPanIdQuery(otrGetInstance(), panid, mask, &address, cb, ctx);
  return error;
}
#endif

const OTMacCounters OpenThreadClass::counter(int type)
{
  OT_API_CALL_RET(const otMacCounters*,
    otLinkGetCounters(otrGetInstance())
  );
}

otError OpenThreadClass::dataset_mgmtget_active(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr)
{
  struct otIp6Address address;
  OT_COPY_IP6(address, addr);
  return dataset_mgmtget_active(dataset, tlvs, len, &address);
}

otError OpenThreadClass::dataset_mgmtget_pending(otOperationalDatasetComponents* dataset, uint8_t* tlvs, uint8_t len, IPAddress& addr)
{
  struct otIp6Address address;
  OT_COPY_IP6(address, addr);
  return dataset_mgmtget_pending(dataset, tlvs, len, &address);
}

otError OpenThreadClass::discover(uint32_t chbits, otHandleActiveScanResult callback, void* context)
{
  OT_API_CALL_RET(otError,
    otThreadDiscover(otrGetInstance(), chbits, OT_PANID_BROADCAST, false, false, callback, context)
  );
}


void OpenThreadClass::discover_sync_callback(otActiveScanResult *aResult, void *aContext)
{
/* TODO
  struct discover_data* data= reinterpret_cast<struct discover_data*>(aContext);
  if(aResult) {
    if(data->size == data->count) {
      data->error = OT_ERROR_NO_BUFS;
      k_sem_give(&data->sem);
    }
    else {
      memcpy(&data->table[data->count], aResult, sizeof(otActiveScanResult));
      data->count++;
    }
  }
  else {
    if(data->error != OT_ERROR_NO_BUFS) {
      k_sem_give(&data->sem);
    }
  }
*/
}

otError OpenThreadClass::discover(otActiveScanResult* table, size_t tablesize, uint32_t chbits)
{
/* TODO
  memset(&sync_discover_context, 0, sizeof(struct discover_data));

  sync_discover_context.table = table;
  sync_discover_context.size = tablesize;
  k_sem_init(&sync_discover_context.sem, 0, 1);

  otError err = otThreadDiscover(otrGetInstance(), chbits, OT_PANID_BROADCAST, false, false, discover_sync_callback, &sync_discover_context);
  if(err) return err;

  k_sem_take(&sync_discover_context.sem, K_FOREVER);

  return sync_discover_context.error;
*/
  return OT_ERROR_NONE;
}

const OTExtAddress OpenThreadClass::eui64()
{
  static otExtAddress extadr = { 0 };

  if(extadr.m8[0] == 0 && extadr.m8[1] == 0 && extadr.m8[2] == 0 && extadr.m8[3] == 0 &&
     extadr.m8[4] == 0 && extadr.m8[5] == 0 && extadr.m8[6] == 0 && extadr.m8[7] == 0) {
    OT_API_CALL(
      otLinkGetFactoryAssignedIeeeEui64(otrGetInstance(), &extadr)
    );
  }

  return &extadr;
}

#if OPENTHREAD_ENABLE_FTD
int OpenThreadClass::eidcache_num()
{
  OT_API_CALL_RET(int,
    _eidcache_num()
  );
}

int OpenThreadClass::_eidcache_num()
{
  int ret = 0;
  for (uint8_t i = 0;; i++)
  {
    otEidCacheEntry entry;
    if(otThreadGetEidCacheEntry(otrGetInstance(), i, &entry)) {
      ret = (i-1);
      break;
    }
  }

  return ret;
}
#endif

int OpenThreadClass::ipaddr_num()
{
  OT_API_CALL_RET(int,
    _ipaddr_num()
  );
}

int OpenThreadClass::_ipaddr_num()
{
  int count = 0;

  for (const otNetifAddress *addr = otIp6GetUnicastAddresses(otrGetInstance()); addr; addr = addr->mNext) {
    count++;
  }
  return count;
}

IPAddress OpenThreadClass::ipaddr(int idx)
{
  OT_API_CALL_RET(IPAddress,
    _ipaddr(idx)
  );
}

IPAddress OpenThreadClass::_ipaddr(int idx)
{
  int count = 0;
  const otNetifAddress* addr = NULL;
  uint16_t* u16null = NULL;

  for (addr = otIp6GetUnicastAddresses(otrGetInstance()); addr; addr = addr->mNext) {
    if(count == idx) break;
  }
  return (addr ? addr->mAddress.mFields.m16 : u16null);
}

otError OpenThreadClass::ipaddr_add(IPAddress& addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc)
{
  OT_API_CALL_RET(otError,
    _ipaddr_add(addr, prefixlen, preferred, valid, scopeoverridevalid, scopeoverride, rloc)
  );
}

otError OpenThreadClass::_ipaddr_add(IPAddress& addr, uint8_t prefixlen, bool preferred, bool valid, bool scopeoverridevalid, uint32_t scopeoverride, bool rloc)
{
  otError err;
  struct otNetifAddress address;

  OT_COPY_IP6(address.mAddress, addr);
  address.mPrefixLength = prefixlen;
  address.mPreferred = preferred;
  address.mValid = valid;
  address.mScopeOverrideValid = scopeoverridevalid;
  address.mScopeOverride = scopeoverride;
  address.mRloc = rloc;

  err = otIp6AddUnicastAddress(otrGetInstance(), &address);
  return err;
}

otError OpenThreadClass::ipaddr_del(IPAddress& addr)
{
  OT_API_CALL_RET(otError,
    _ipaddr_del(addr)
  );
}

otError OpenThreadClass::_ipaddr_del(IPAddress& addr)
{
  otError err;
  struct otIp6Address address;
  OT_COPY_IP6(address, addr);
  err = otIp6RemoveUnicastAddress(otrGetInstance(), &address);
  return err;
}

int OpenThreadClass::ipmaddr_num()
{
  OT_API_CALL_RET(int,
    _ipmaddr_num()
  );
}

int OpenThreadClass::_ipmaddr_num()
{

  int count = 0;
  for (const otNetifMulticastAddress *addr = otIp6GetMulticastAddresses(otrGetInstance()); addr; addr = addr->mNext) {
    count++;
  }
  return count;
}

IPAddress OpenThreadClass::ipmaddr(int idx)
{
  OT_API_CALL_RET(IPAddress,
    _ipmaddr(idx)
  );
}

IPAddress OpenThreadClass::_ipmaddr(int idx)
{
  int count = 0;
  const otNetifMulticastAddress* addr = NULL;
  static const uint16_t* u16null = NULL;

  for (addr = otIp6GetMulticastAddresses(otrGetInstance()); addr; addr = addr->mNext) {
    if(count == idx) break;
  }
  return (addr ? addr->mAddress.mFields.m16 : u16null);
}

otError OpenThreadClass::ipmaddr_add(IPAddress& addr)
{
  OT_API_CALL_RET(otError,
    _ipmaddr_add(addr)
  );
}

otError OpenThreadClass::_ipmaddr_add(IPAddress& addr)
{
  otError error;
  struct otIp6Address address;
  OT_COPY_IP6(address, addr);
  error = otIp6SubscribeMulticastAddress(otrGetInstance(), &address);
  return error;
}

otError OpenThreadClass::ipmaddr_del(IPAddress& addr)
{
  OT_API_CALL_RET(otError,
    _ipmaddr_del(addr)
  );
}

otError OpenThreadClass::_ipmaddr_del(IPAddress& addr)
{
  otError error;
  struct otIp6Address address;
  OT_COPY_IP6(address, addr);
  error = otIp6UnsubscribeMulticastAddress(otrGetInstance(), &address);
  return error;
}

#if OPENTHREAD_ENABLE_JOINER

const OTExtAddress OpenThreadClass::joinerid()
{
  static otExtAddress extadr = { 0 };

  if(extadr.m8[0] == 0 && extadr.m8[1] == 0 && extadr.m8[2] == 0 && extadr.m8[3] == 0 &&
     extadr.m8[4] == 0 && extadr.m8[5] == 0 && extadr.m8[6] == 0 && extadr.m8[7] == 0) {
    OT_API_CALL(
      otJoinerGetId(otrGetInstance(), &extadr)
    );
  }

  return &extadr;
}

otError OpenThreadClass::joiner_start(const char* pskc, const char* provision, otJoinerCallback cb, void* ctx)
{
  OT_API_CALL_RET(otError,
    _joiner_start(pskc, provision, cb, ctx)
  );
}

otError OpenThreadClass::_joiner_start(const char* pskc, const char* provision, otJoinerCallback cb, void* ctx)
{
  otError err = OT_ERROR_NONE;
  err = otJoinerStart(otrGetInstance(), pskc, provision, PACKAGE_NAME, "OPENTHREAD_ENABLE_PLATFORM_INFO",
                      PACKAGE_VERSION, NULL, cb, ctx);
  return err;
}

void OpenThreadClass::joiner_start_sync_callback(otError aResult, void *aContext)
{
  otError* ptrerr = reinterpret_cast<otError*>(aContext);
  *ptrerr = aResult;
  signal_mainloop(COMMISSION_BIT);
}

otError OpenThreadClass::joiner_start(const char* pskc, const char* provision)
{
  otError cbErr;
  otError err = joiner_start(pskc, provision, joiner_start_sync_callback, &cbErr);
  if(err) return err;

  wait_mainloop(COMMISSION_BIT);

  return cbErr;
}

#endif

#if OPENTHREAD_ENABLE_FTD
otError OpenThreadClass::neighbor(int index, otNeighborInfo* neighborInfo)
{
  OT_API_CALL_RET(otError,
    _neighbor(index, neighborInfo)
  );
}
otError OpenThreadClass::_neighbor(int index, otNeighborInfo* neighborInfo)
{
  otError err = OT_ERROR_NONE;
  otNeighborInfoIterator iterator = OT_NEIGHBOR_INFO_ITERATOR_INIT;
  for(int i=0; i<=index; i++) {
    err = otThreadGetNextNeighborInfo(otrGetInstance(), &iterator, neighborInfo);
    if(err) break;
  }
  return err;
}
#endif

//OT_GETTER_IMPL(OTNetworkName, networkname, Thread, NetworkName);
const OTNetworkName OpenThreadClass::networkname()
{
  OT_API_CALL_RET(const char*,
    otThreadGetNetworkName(otrGetInstance())
  );
}

const otError OpenThreadClass::networkname(OTNetworkName netname)
{
  OT_API_CALL_RET(otError,
    otThreadSetNetworkName(otrGetInstance(), netname)
  );
}

otError OpenThreadClass::ping(IPAddress& addr, const uint8_t* buffer, uint16_t buflen)
{
  OT_API_CALL_RET(otError,
    _ping(addr, buffer, buflen)
  );
}

otError OpenThreadClass::_ping(IPAddress& addr, const uint8_t* buffer, uint16_t buflen)
{
  otMessageInfo messageInfo = {0};
  otError error = OT_ERROR_NONE;

  OT_COPY_IP6(messageInfo.mPeerAddr, addr);


  otMessage* message = otIp6NewMessageFromBuffer(otrGetInstance(), buffer, buflen, NULL);
  if(!message) {
    error = OT_ERROR_NO_BUFS;
    goto exit;
  }
    
  error = otIcmp6SendEchoRequest(otrGetInstance(), message, &messageInfo, 1);

  if (error != OT_ERROR_NONE && message != NULL) {
    otMessageFree(message);
  }
exit:
  return error;
}

otError OpenThreadClass::promiscuous(otLinkPcapCallback cb, void* ctx)
{
  OT_API_CALL_RET(otError,
    _promiscuous(cb, ctx)
  );
}

otError OpenThreadClass::_promiscuous(otLinkPcapCallback cb, void* ctx)
{
  otError err = OT_ERROR_NONE;
  if(cb) {
    otLinkSetPcapCallback(otrGetInstance(), cb, ctx);
    err = otLinkSetPromiscuous(otrGetInstance(), true);
  }
  else {
    otLinkSetPcapCallback(otrGetInstance(), NULL, NULL);
    err = otLinkSetPromiscuous(otrGetInstance(), false);
  }
  return err;
}

otError OpenThreadClass::state(otDeviceRole role)
{
  OT_API_CALL_RET(otError,
    _state(role)
  );
}

otError OpenThreadClass::_state(otDeviceRole role)
{
  otError err = OT_ERROR_NONE;
  switch (role)
  {
    case OT_DEVICE_ROLE_DETACHED:
      err = otThreadBecomeDetached(otrGetInstance());
      break;
    case OT_DEVICE_ROLE_CHILD:
      err = otThreadBecomeChild(otrGetInstance());
      break;

#if OPENTHREAD_FTD
    case OT_DEVICE_ROLE_ROUTER:
      err = otThreadBecomeRouter(otrGetInstance());
      break;
    case OT_DEVICE_ROLE_LEADER:
      err = otThreadBecomeLeader(otrGetInstance());
      break;
#endif // OPENTHREAD_FTD
    default:
      break;
  }
  return err;
}

OpenThreadClass OpenThread;
