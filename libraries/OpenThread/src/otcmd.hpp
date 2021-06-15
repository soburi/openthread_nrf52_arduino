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

#include "OpenThread_types.hpp"

#include <openthread-core-config.h>
#include <openthread/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <openthread/error.h>
#include <openthread/backbone_router.h>
#include <openthread/backbone_router_ftd.h>
#include <openthread/border_agent.h>
#include <openthread/coap.h>
#include <openthread/coap_secure.h>
#include <openthread/commissioner.h>
#include <openthread/dns.h>
#include <openthread/diag.h>
#include <openthread/heap.h>
#include <openthread/icmp6.h>
#include <openthread/jam_detection.h>
#include <openthread/joiner.h>
#include <openthread/link.h>
#include <openthread/link_raw.h>
#include <openthread/netdata.h>
#include <openthread/netdiag.h>
#include <openthread/sntp.h>
#include <openthread/thread.h>
#include <openthread/thread_ftd.h>
#include <openthread/udp.h>
#include <openthread/openthread-freertos.h>


#define SuccessOrExit(aStatus) \
    do                         \
    {                          \
        if ((aStatus) != 0)    \
        {                      \
            goto exit;         \
        }                      \
    } while (false)

#define VerifyOrExit(aCondition, ...) \
    do                                \
    {                                 \
        if (!(aCondition))            \
        {                             \
            __VA_ARGS__;              \
            goto exit;                \
        }                             \
    } while (false)


namespace OTCMD {

#if OPENTHREAD_CONFIG_COAP_API_ENABLE
class OTCoap {
  otCoapResource mResource;
  uint8_t mResourceContent[16];
  uint16_t mResourceContentLength;
  bool mUseDefaultRequestTxParameters;
  bool mUseDefaultResponseTxParameters;
  otCoapTxParameters mRequestTxParameters;
  otCoapTxParameters mResponseTxParameters;

  otError _request(otCoapCode, const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);

  const otCoapTxParameters *GetRequestTxParameters(void) const
  {
    return mUseDefaultRequestTxParameters ? nullptr : &mRequestTxParameters;
  }

  const otCoapTxParameters *GetResponseTxParameters(void) const
  {
    return mUseDefaultResponseTxParameters ? nullptr : &mResponseTxParameters;
  }

  static void HandleRequest(void*, otMessage*, const otMessageInfo*);
  void HandleRequest(otMessage*, const otMessageInfo*);

public:
#if OPENTHREAD_CONFIG_COAP_OBSERVE_API_ENABLE
  otError cancel();
  otError observe(const otIp6Addresss*, const char*, otCoapType);
#endif
  otError resource(char const*);
  void set(uint8_t*, size_t);
  otError start();
  otError stop();
  void parameters_request_default();
  void parameters_request(uint32_t acktimeout, uint8_t numerator, uint8_t denominator, uint8_t retransmit);
  void parameters_response_default();
  void parameters_response(uint32_t acktimeout, uint8_t numerator, uint8_t denominator, uint8_t retransmit);
  otError request_delete(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_get(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_post(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_put(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
};
#endif // OPENTHREAD_CONFIG_COAP_API_ENABLE

#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE
class OTCoapSecure {
  otCoapResource mResource;
  otError _request(otCoapCode, const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
public:
  otError resource(char const*, otCoapRequestHandler, void*);
  otError start();
  void stop();
  otError connect(const otIp6Address*, uint16_t, otHandleCoapSecureClientConnect, void*);
  void disconnect();
  otError request_delete(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_get(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_post(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
  otError request_put(const otIp6Address*, char const*, otCoapType, uint8_t*, uint16_t, otCoapResponseHandler, void*);
#ifdef MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
  otError psk(const char*, const char*);
#endif // MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#ifdef MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
  void x509();
#endif
};
#endif // OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE


otError dataset_active(otOperationalDataset& dataset);
otError dataset_pending(otOperationalDataset& dataset);
otError dataset_commit_active(otOperationalDataset& dataset);
otError dataset_commit_pending(otOperationalDataset& dataset);
otError dataset_init_active(otOperationalDataset& dataset);
otError dataset_init_pending(otOperationalDataset& dataset);
#if OPENTHREAD_FTD
otError dataset_init_new(otOperationalDataset& dataset);
#endif

#if (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
//otError bbr(otBackboneRouterConfig&);
#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
void _bbr(bool);
void bbr_disable();
void bbr_enable();
uint8_t bbr_jitter();
void bbr_jitter(uint8_t);
otError bbr_register();
otBackboneRouterState bbr_state();
otError bbr_config(uint8_t, uint16_t, uint32_t);
#endif
#endif
void bufferinfo(otBufferInfo*);
uint8_t channel();
otError channel(uint8_t);
uint32_t channel_supported();
uint32_t channel_preferred();
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
otError channel_monitor();
otError channel_monitor_start();
otError channel_monitor_stop();
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD
otError channel_manager();
otError channel_manager_change(uint8_t);
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
otError channel_manager_select(bool);
#endif
otError channel_manager_auto(bool);
otError channel_manager_delay(uint8_t);
otError channel_manager_interval(uint32_t);
otError channel_manager_supported(uint32_t);
otError channel_manager_favored(uint32_t);
#endif

#if OPENTHREAD_FTD
OTChildIterator child();
OTChildIpIterator childip(uint16_t);
uint8_t childip_max();
#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
void childip_max(uint8_t);
#endif
uint8_t childmax();
otError childmax(uint8_t);
#endif // OPENTHREAD_FTD
uint32_t childtimeout();
void childtimeout(uint32_t);
#if OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
bool coex();
void coex_enable();
void coex_disable();
otError coex_metrics(otRadioCoexMetrics&);
#endif // OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE && OPENTHREAD_FTD
otCommissionerState commissioner();
otError commissioner_announce(uint32_t, uint8_t, uint16_t, const otIp6Address*);
otError commissioner_energy(uint32_t, uint8_t, uint16_t, uint16_t, const otIp6Address*, otCommissionerEnergyReportCallback, void*);
otError commissioner_joiner_add(const otExtAddress*, const char*, uint32_t);
otError commissioner_joiner_add(const char*, uint32_t);
otError commissioner_joiner_remove(const otExtAddress*);
otError commissioner_joiner_remove();
otError commissioner_mgmtget(otMeshcopTlvType* tlvs, uint8_t count);
otError commissioner_mgmtset(uint32_t, uint32_t, uint32_t, uint8_t*, uint8_t, otMeshcopTlvType*, uint8_t);

otError commissioner_panid(uint16_t, uint32_t, const otIp6Address*, otCommissionerPanIdConflictCallback, void*);
otError commissioner_provisioningurl(const char*);
uint16_t commissioner_sessionid();
otError commissioner_start(otCommissionerStateCallback, otCommissionerJoinerCallback, void*);
otError commissioner_stop();
#endif

#if OPENTHREAD_FTD
void contextreusedelay(uint32_t);
uint32_t contextreusedelay();
#endif

OTMacCounters counters_mac();
void  counters_mac_reset();
OTMleCounters counters_mle();
void  counters_mle_reset();



void dataset_clear(otOperationalDataset& dataset);
void dataset_delay(otOperationalDataset& dataset, uint32_t delay);
void dataset_extpanid(otOperationalDataset& dataset, const otExtendedPanId* panid);
void dataset_masterkey(otOperationalDataset& dataset, const otMasterKey* masterkey);
void dataset_meshlocalprefix(otOperationalDataset& dataset, const otMeshLocalPrefix* prefix);
void dataset_networkname(otOperationalDataset& dataset, const char* netname);
void dataset_panid(otOperationalDataset& dataset, otPanId panid);
void dataset_activetimestamp(otOperationalDataset& dataset, uint64_t timestamp);
void dataset_pendingtimestamp(otOperationalDataset& dataset, uint64_t timestamp);
void dataset_channel(otOperationalDataset& dataset, uint16_t channel);
void dataset_channelmask(otOperationalDataset& dataset, uint32_t channelmask);
void dataset_securitypolicy(otOperationalDataset& dataset, uint16_t rotationtime, uint32_t flags);

inline uint32_t dataset_channelmask(otOperationalDataset& dataset) { return dataset.mChannelMask; }
inline uint16_t dataset_channel(otOperationalDataset& dataset) { return dataset.mChannel; }
inline uint32_t dataset_delay(otOperationalDataset& dataset) { return dataset.mDelay; }
inline OTExtendedPanId dataset_extpanid(otOperationalDataset& dataset) { return &dataset.mExtendedPanId; }
inline OTMasterKey dataset_masterkey(otOperationalDataset& dataset) { return &dataset.mMasterKey; }
inline OTMeshLocalPrefix dataset_meshlocalprefix(otOperationalDataset& dataset) { return &dataset.mMeshLocalPrefix; }
inline OTNetworkName dataset_networkname(otOperationalDataset& dataset) { return &dataset.mNetworkName; }
inline otPanId dataset_panid(otOperationalDataset& dataset) { return dataset.mPanId; }
inline uint64_t dataset_pendingtimestamp(otOperationalDataset& dataset) { return dataset.mPendingTimestamp; }
inline uint64_t dataset_activetimestamp(otOperationalDataset& dataset) { return dataset.mActiveTimestamp; }
inline uint16_t dataset_securitypolicy_rotationtime(otOperationalDataset& dataset) { return dataset.mSecurityPolicy.mRotationTime; }
inline uint32_t dataset_securitypolicy_flags(otOperationalDataset& dataset) { return dataset.mSecurityPolicy.mFlags; }
inline OTSecurityPolicy dataset_securitypolicy(otOperationalDataset& dataset) { return &dataset.mSecurityPolicy; }

inline void dataset_channelmask_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsChannelMaskPresent = p; }
inline void dataset_channel_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsChannelPresent = p; }
inline void dataset_delay_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsDelayPresent = p; }
inline void dataset_extpanid_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsExtendedPanIdPresent = p; }
inline void dataset_masterkey_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsMasterKeyPresent = p; }
inline void dataset_meshlocalprefix_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsMeshLocalPrefixPresent = p; }
inline void dataset_networkname_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsNetworkNamePresent = p; }
inline void dataset_panid_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsPanIdPresent = p; }
inline void dataset_pendingtimestamp_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsPendingTimestampPresent = p; }
inline void dataset_activetimestamp_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsActiveTimestampPresent = p; }
inline void dataset_securitypolicy_present(otOperationalDataset& dataset, bool p) { dataset.mComponents.mIsSecurityPolicyPresent = p; }

inline bool dataset_channelmask_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsChannelMaskPresent; }
inline bool dataset_channel_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsChannelPresent; }
inline bool dataset_delay_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsDelayPresent; }
inline bool dataset_extpanid_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsExtendedPanIdPresent; }
inline bool dataset_masterkey_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsMasterKeyPresent; }
inline bool dataset_meshlocalprefix_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsMeshLocalPrefixPresent; }
inline bool dataset_networkname_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsNetworkNamePresent; }
inline bool dataset_panid_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsPanIdPresent; }
inline bool dataset_pendingtimestamp_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsPendingTimestampPresent; }
inline bool dataset_activetimestamp_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsActiveTimestampPresent; }
inline bool dataset_securitypolicy_present(otOperationalDataset& dataset) { return  dataset.mComponents.mIsSecurityPolicyPresent; }

#if OPENTHREAD_FTD
void dataset_pskc(otOperationalDataset& dataset, const otPskc& pskc_);
inline OTPskc dataset_pskc(otOperationalDataset& dataset) { return &dataset.mPskc; }
otError dataset_pskc_passphrase(char const&);
#endif

otError dataset_mgmtgetcommand_active(otOperationalDatasetComponents& components, const uint8_t* tlvs, const size_t len);
otError dataset_mgmtgetcommand_active(otOperationalDatasetComponents& components, const otIp6Address& addr, const uint8_t* tlvs, const size_t len);
otError dataset_mgmtgetcommand_pending(otOperationalDatasetComponents& components, const uint8_t* tlvs, const size_t len);
otError dataset_mgmtgetcommand_pending(otOperationalDatasetComponents& components, const otIp6Address& addr, const uint8_t* tlvs, const size_t len);
otError dataset_mgmtsetcommand_active(otOperationalDataset& dataset, const uint8_t* tlvs, const size_t len);
otError dataset_mgmtsetcommand_pending(otOperationalDataset& dataset, const uint8_t* tlvs, const size_t len);





#if OPENTHREAD_FTD
uint32_t delaytimermin();
otError delaytimermin(uint32_t);
#endif
#if OPENTHREAD_CONFIG_DIAG_ENABLE
otError diag(uint8_t, char**, char*, size_t);
#endif
otError discover(uint32_t, otHandleActiveScanResult, void*);
otError discover(uint32_t, OTActiveScanResultSyncIterator*);
#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
otError dns_resolve(const char*, const otIp6Address*, uint16_t, otDnsResponseHandler, void*);
otError dns_resolve(const char*, otDnsResponseHandler, void*);
#endif

#if OPENTHREAD_FTD
OTEidCacheIterator eidcache();
#endif // OPENTHREAD_FTD
void eui64(otExtAddress*);
OTExtAddress eui64();
#if OPENTHREAD_POSIX
otError exit(uint8_t, char**);
#endif
#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_DEBUG_UART) && OPENTHREAD_POSIX
void logfilename(const char*);
#endif
OTExtAddress extaddr();
otError extaddr(const otExtAddress*);
OTExtendedPanId extpanid();
otError extpanid(const otExtendedPanId*);
void factoryreset();
otError _ifconfig(bool);
bool ifconfig();
otError ifconfig_up();
otError ifconfig_down();
OTIpaddrIterator ipaddr();
otError _ipaddr_add(const struct otNetifAddress*);
otError ipaddr_add(const otIp6Address*, uint8_t, bool, bool, bool, uint32_t, bool);
otError ipaddr_del(const otIp6Address*);
IPAddress ipaddr_linklocal();
IPAddress ipaddr_mleid();
IPAddress ipaddr_rloc();
OTIpmaddrIterator ipmaddr();
otError ipmaddr_add(const otIp6Address*);
otError ipmaddr_del(const otIp6Address*);
bool ipmaddr_promiscuous();
void ipmaddr_promiscuous(bool);

#if OPENTHREAD_CONFIG_JOINER_ENABLE
void joiner_id(otExtAddress*);
OTExtAddress joiner_id();
otError joiner_start(const char*, const char*, otJoinerCallback, void*);
otError joiner_start(const char*, const char*, OTJoinerContext*);
void joiner_stop();
#endif // OPENTHREAD_CONFIG_JOINER_ENABLE
#if OPENTHREAD_FTD
uint16_t joinerport();
#endif
uint32_t keysequence_counter();
void keysequence_counter(uint32_t);
uint32_t keysequence_guardtime();
void  keysequence_guardtime(uint32_t);
otError leaderdata(otLeaderData*);
#if OPENTHREAD_FTD
uint32_t leaderpartitionid();
void leaderpartitionid(uint32_t);
uint8_t leaderweight();
void leaderweight(uint8_t);
#endif

uint8_t mac_retries_direct();
void mac_retries_direct(uint8_t);
#if OPENTHREAD_FTD
uint8_t mac_retries_indirect();
void mac_retries_indirect(uint8_t);
#endif
#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE
OTMacFilterIterator macfilter();
void _macfilter_addr_mode(otMacFilterAddressMode);
otMacFilterAddressMode _macfilter_addr_mode();
OTMacFilterAddrIterator macfilter_addr();
void macfilter_addr_disable();
void macfilter_addr_whitelist();
void macfilter_addr_blacklist();
otError macfilter_addr_add(const otExtAddress*, uint8_t);
otError macfilter_addr_add(uint8_t);
otError macfilter_addr_remove(const otExtAddress*);
void macfilter_addr_clear();
OTMacFilterRssIterator macfilter_rss();
otError macfilter_rss_add(const otExtAddress*, uint8_t);
otError macfilter_rss_add(uint8_t);
otError macfilter_rss_add_lqi(const otExtAddress*, uint8_t);
otError macfilter_rss_add_lqi(uint8_t);
otError macfilter_rss_remove(const otExtAddress* a=nullptr);
void macfilter_rss_clear();
#endif


OTMasterKey masterkey();
otError masterkey(const otMasterKey*);
OTLinkModeConfig mode();
otError mode(otLinkModeConfig);
#if OPENTHREAD_FTD
OTNeighborIterator neighbor();


#endif
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE || OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
otError netdataregister();
#endif
otError netdatashow(uint8_t*, uint8_t&);

#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
otError networkdiagnostic_get(const otIp6Address*, uint8_t*, uint8_t);
otError networkdiagnostic_reset(const otIp6Address*, uint8_t*, uint8_t);
#endif
#if OPENTHREAD_FTD
void networkidtimeout(uint8_t);
uint8_t networkidtimeout();
#endif
const char* networkname();
otError networkname(const char*);
#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
otNetworkTimeStatus networktime();
otError networktime(uint16_t, uint16_t);
#endif
uint16_t panid();
otError panid(uint16_t);
otError parent(otRouterInfo*);
#if OPENTHREAD_FTD
uint8_t parentpriority();
otError parentpriority(uint8_t);
#endif
otError ping(const otIp6Address*, const uint8_t*, uint16_t);
otError ping_stop();
uint32_t pollperiod();
otError pollperiod(uint32_t);
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
OTPrefixIterator prefix();
otError _prefix_add(const otBorderRouterConfig*);
otError prefix_add(const otIp6Prefix*, otRoutePreference, bool, bool, bool, bool, bool, bool, bool);
otError prefix_remove(const otIp6Prefix*);
#endif
bool promiscuous();
otError promiscuous_enable(otLinkPcapCallback, void* ctx);
otError promiscuous_disable();
#if OPENTHREAD_FTD
otError preferrouterid(uint8_t);
OTPskc pskc();
otError pskc(const otPskc*);
otError pskc_passphrase(const char*);
otError releaserouterid(uint8_t);
#endif
void reset();
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
OTRouteIterator route();
otError route_add(const otExternalRouteConfig*);
otError route_add(otRoutePreference, bool);
otError route_remove(const otIp6Prefix*);
#endif
#if OPENTHREAD_FTD
OTRouterIterator router();
otError router(int, otRouterInfo*);
uint8_t routerdowngradethreshold();
void routerdowngradethreshold(uint8_t);
void _routereligible(uint8_t);
bool routereligible();
void routereligible_enable();
void routereligible_disable();
uint8_t routerselectionjitter();
void routerselectionjitter(uint8_t);
uint8_t routerupgradethreshold();
void routerupgradethreshold(uint8_t);
#endif
uint16_t rloc16();

#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
otError service_add(uint32_t, uint8_t*, uint8_t, uint8_t*, uint8_t);
otError service_add(uint32_t, const char*, const char*);
otError service_remove(uint32_t, uint8_t*, uint8_t);
otError service_remove(uint32_t, const char*);
#endif

otError scan(uint32_t, uint16_t, otHandleActiveScanResult, void*);
otError scan(uint32_t, uint16_t, OTActiveScanResultSyncIterator* ctx);
otError scan_energy(uint32_t, uint16_t, otHandleEnergyScanResult, void*);
otError scan_energy(uint32_t, uint16_t, OTEnergyScanResultSyncIterator* ctx);
bool singleton();
#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
otError sntp_query(const otIp6Address*, uint16_t, otSntpResponseHandler, void*);
#endif
otDeviceRole state();
otError state(otDeviceRole);
otError state_child();
otError state_detached();
#if OPENTHREAD_FTD
otError state_router();
otError state_leader();
#endif
otError _thread(bool);
otError thread_start();
otError thread_stop();
uint16_t thread_version();
int8_t txpower();
otError txpower(int8_t);
otError _txpower(int8_t*);
const char* version();

}
