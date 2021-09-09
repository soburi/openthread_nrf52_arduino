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

#include "otapi.hpp"

#include <openthread-core-config.h>
#include <openthread/config.h>
#include <openthread/backbone_router.h>
#include <openthread/backbone_router_ftd.h>
#include <openthread/border_agent.h>
#include <openthread/border_router.h>
#include <openthread/channel_manager.h>
#include <openthread/channel_monitor.h>
#include <openthread/child_supervision.h>
#include <openthread/cli.h>
#include <openthread/coap.h>
#include <openthread/coap_secure.h>
#include <openthread/commissioner.h>
#include <openthread/crypto.h>
#include <openthread/dataset.h>
#include <openthread/dataset_ftd.h>
#include <openthread/diag.h>
#include <openthread/entropy.h>
#include <openthread/dns.h>
#include <openthread/heap.h>
#include <openthread/icmp6.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/jam_detection.h>
#include <openthread/joiner.h>
#include <openthread/link.h>
#include <openthread/link_raw.h>
#include <openthread/logging.h>
#include <openthread/message.h>
#include <openthread/ncp.h>
#include <openthread/netdata.h>
#include <openthread/netdiag.h>
#include <openthread/network_time.h>
#include <openthread/platform/diag.h>
#include <openthread/platform/entropy.h>
#include <openthread/platform/radio.h>
#include <openthread/platform/udp.h>
#include <openthread/random_crypto.h>
#include <openthread/random_noncrypto.h>
#include <openthread/server.h>
#include <openthread/sntp.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/thread_ftd.h>
#include <openthread/udp.h>
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

#define OTCLS(cls) cls
#define OT_DECL__FUNC(n, cls, rettype, fn, ...) \
rettype OTAPI:: OTCLS(cls)  ::  fn (OT_DECL_ARGS ## n(__VA_ARGS__)) \
{ \
  rettype ret = OT_CALL_FUNC ## n(cls, fn); \
  return ret; \
}

#define OT_DECL_VFUNC(n, cls, rettype, fn, ...) \
rettype OTAPI:: OTCLS(cls)  ::  fn (OT_DECL_ARGS ## n(__VA_ARGS__)) \
{ \
  OT_CALL_FUNC ## n(cls, fn); \
}

#if (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
OT_DECL__FUNC(1, BackboneRouter, otError, GetPrimary, otBackboneRouterConfig*)
OT_DECL__FUNC(0, BackboneRouter, otBackboneRouterState, GetState)
OT_DECL__FUNC(0, BackboneRouter, otError, Register)
OT_DECL__FUNC(0, BackboneRouter, uint8_t, GetRegistrationJitter)
OT_DECL_VFUNC(1, BackboneRouter, void, GetConfig, otBackboneRouterConfig*)
OT_DECL_VFUNC(1, BackboneRouter, void, SetConfig, const otBackboneRouterConfig*)
OT_DECL_VFUNC(1, BackboneRouter, void, SetEnabled, bool)
OT_DECL_VFUNC(1, BackboneRouter, void, SetRegistrationJitter, uint8_t)
#endif
#if OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
OT_DECL__FUNC(0, BorderAgent, otBorderAgentState, GetState)
#endif
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
OT_DECL__FUNC(1, BorderRouter, otError, AddOnMeshPrefix, const otBorderRouterConfig*)
OT_DECL__FUNC(1, BorderRouter, otError, AddRoute, const otExternalRouteConfig*)
OT_DECL__FUNC(3, BorderRouter, otError, GetNetData, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, BorderRouter, otError, GetNextOnMeshPrefix, otNetworkDataIterator*, otBorderRouterConfig*)
OT_DECL__FUNC(2, BorderRouter, otError, GetNextRoute, otNetworkDataIterator*, otExternalRouteConfig*)
OT_DECL__FUNC(0, BorderRouter, otError, Register)
OT_DECL__FUNC(1, BorderRouter, otError, RemoveOnMeshPrefix, const otIp6Prefix*)
OT_DECL__FUNC(1, BorderRouter, otError, RemoveRoute, const otIp6Prefix*)
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD
OT_DECL__FUNC(0, ChannelManager, bool, GetAutoChannelSelectionEnabled)
OT_DECL__FUNC(1, ChannelManager, otError, RequestChannelSelect, bool)
OT_DECL__FUNC(1, ChannelManager, otError, SetAutoChannelSelectionInterval, uint32_t)
OT_DECL__FUNC(1, ChannelManager, otError, SetDelay, uint16_t)
OT_DECL__FUNC(0, ChannelManager, uint16_t, GetDelay)
OT_DECL__FUNC(0, ChannelManager, uint32_t, GetAutoChannelSelectionInterval)
OT_DECL__FUNC(0, ChannelManager, uint32_t, GetFavoredChannels)
OT_DECL__FUNC(0, ChannelManager, uint32_t, GetSupportedChannels)
OT_DECL__FUNC(0, ChannelManager, uint8_t, GetRequestedChannel)
OT_DECL_VFUNC(1, ChannelManager, void, RequestChannelChange, uint8_t)
OT_DECL_VFUNC(1, ChannelManager, void, SetAutoChannelSelectionEnabled, bool)
OT_DECL_VFUNC(1, ChannelManager, void, SetFavoredChannels, uint32_t)
OT_DECL_VFUNC(1, ChannelManager, void, SetSupportedChannels, uint32_t)
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
OT_DECL__FUNC(0, ChannelMonitor, bool, IsEnabled)
OT_DECL__FUNC(0, ChannelMonitor, int8_t, GetRssiThreshold)
OT_DECL__FUNC(1, ChannelMonitor, otError, SetEnabled, bool)
OT_DECL__FUNC(1, ChannelMonitor, uint16_t, GetChannelOccupancy, uint8_t)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleCount)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleInterval)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleWindow)
#endif
#if OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE
OT_DECL__FUNC(0, ChildSupervision, uint16_t, GetCheckTimeout)
OT_DECL__FUNC(0, ChildSupervision, uint16_t, GetInterval)
OT_DECL_VFUNC(1, ChildSupervision, void, SetCheckTimeout, uint16_t)
OT_DECL_VFUNC(1, ChildSupervision, void, SetInterval, uint16_t)
#endif
#if OPENTHREAD_CONFIG_COAP_API_ENABLE
const char* OTAPI::Coap::MessageCodeToString(const otMessage* a) { return otCoapMessageCodeToString(a); }
const otCoapOption* OTAPI::Coap::OptionIteratorGetFirstOption(otCoapOptionIterator* a) { return otCoapOptionIteratorGetFirstOption(a); }
const otCoapOption* OTAPI::Coap::OptionIteratorGetFirstOptionMatching(otCoapOptionIterator* a, uint16_t b) { return otCoapOptionIteratorGetFirstOptionMatching(a,b); }
const otCoapOption* OTAPI::Coap::OptionIteratorGetNextOption(otCoapOptionIterator* a) { return otCoapOptionIteratorGetNextOption(a); }
const otCoapOption* OTAPI::Coap::OptionIteratorGetNextOptionMatching(otCoapOptionIterator* a, uint16_t b) { return otCoapOptionIteratorGetNextOptionMatching(a, b); }
const uint8_t* OTAPI::Coap::MessageGetToken(const otMessage* a) { return otCoapMessageGetToken(a); }
otCoapCode OTAPI::Coap::MessageGetCode(const otMessage* a) { return otCoapMessageGetCode(a); }
otCoapType OTAPI::Coap::MessageGetType(const otMessage* a) { return otCoapMessageGetType(a); }
OT_DECL_VFUNC(1, Coap, void, AddResource, otCoapResource*)
otError OTAPI::Coap::MessageAppendBlock1Option(otMessage* a, uint32_t b, bool c, otCoapBlockSzx d) { return otCoapMessageAppendBlock1Option(a,b,c,d); }
otError OTAPI::Coap::MessageAppendBlock2Option(otMessage* a, uint32_t b, bool c, otCoapBlockSzx d) { return otCoapMessageAppendBlock2Option(a,b,c,d); }
otError OTAPI::Coap::MessageAppendContentFormatOption(otMessage* a, otCoapOptionContentFormat b) { return otCoapMessageAppendContentFormatOption(a,b); }
otError OTAPI::Coap::MessageAppendMaxAgeOption(otMessage* a, uint32_t b) { return otCoapMessageAppendMaxAgeOption(a,b); }
otError OTAPI::Coap::MessageAppendObserveOption(otMessage* a, uint32_t b) { return otCoapMessageAppendObserveOption(a,b); }
otError OTAPI::Coap::MessageAppendOption(otMessage* a, uint16_t b, uint16_t c, const void* d) { return otCoapMessageAppendOption(a,b,c,d); }
otError OTAPI::Coap::MessageAppendProxyUriOption(otMessage* a, const char* b) { return otCoapMessageAppendProxyUriOption(a,b); }
otError OTAPI::Coap::MessageAppendUintOption(otMessage* a, uint16_t b, uint32_t c) { return otCoapMessageAppendUintOption(a,b,c); }
otError OTAPI::Coap::MessageAppendUriPathOptions(otMessage* a, const char* b) { return otCoapMessageAppendUriPathOptions(a,b); }
otError OTAPI::Coap::MessageAppendUriQueryOption(otMessage* a, const char* b) { return otCoapMessageAppendUriQueryOption(a,b); }
otError OTAPI::Coap::MessageInitResponse(otMessage* a, const otMessage* b, otCoapType c, otCoapCode d) { return otCoapMessageInitResponse(a,b,c,d); }
otError OTAPI::Coap::MessageSetPayloadMarker(otMessage* a) { return otCoapMessageSetPayloadMarker(a); }
otError OTAPI::Coap::MessageSetToken(otMessage* a, const uint8_t* b, uint8_t c) { return otCoapMessageSetToken(a,b,c); }
otError OTAPI::Coap::OptionIteratorGetOptionUintValue(otCoapOptionIterator* a, uint64_t* b) { return otCoapOptionIteratorGetOptionUintValue(a,b); }
otError OTAPI::Coap::OptionIteratorGetOptionValue(otCoapOptionIterator* a, void* b) { return otCoapOptionIteratorGetOptionValue(a,b); }
otError OTAPI::Coap::OptionIteratorInit(otCoapOptionIterator* a, const otMessage* b) { return otCoapOptionIteratorInit(a,b); }
OT_DECL__FUNC(5, Coap, otError, SendRequestWithParameters, otMessage*, const otMessageInfo*, otCoapResponseHandler, void*, const otCoapTxParameters*)
OT_DECL__FUNC(3, Coap, otError, SendResponseWithParameters, otMessage*, const otMessageInfo*, const otCoapTxParameters*)
OT_DECL__FUNC(1, Coap, otError, Start, uint16_t)
OT_DECL__FUNC(0, Coap, otError, Stop)
OT_DECL__FUNC(1, Coap, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(4, Coap, otError, SendRequest, otMessage*, const otMessageInfo*, otCoapResponseHandler, void*)
OT_DECL__FUNC(2, Coap, otError, SendResponse, otMessage*, const otMessageInfo*)
uint16_t OTAPI::Coap::BlockSizeFromExponent(otCoapBlockSzx a) { return otCoapBlockSizeFromExponent(a); }
uint16_t OTAPI::Coap::MessageGetMessageId(const otMessage* a) { return otCoapMessageGetMessageId(a); }
uint8_t OTAPI::Coap::MessageGetTokenLength(const otMessage* a) { return otCoapMessageGetTokenLength(a); }
void OTAPI::Coap::MessageGenerateToken(otMessage* a, uint8_t b) { return otCoapMessageGenerateToken(a,b); }
void OTAPI::Coap::MessageInit(otMessage* a, otCoapType b, otCoapCode c) { return otCoapMessageInit(a,b,c); }
OT_DECL_VFUNC(1, Coap, void, RemoveResource, otCoapResource*)
OT_DECL_VFUNC(2, Coap, void, SetDefaultHandler, otCoapRequestHandler, void*)
#endif
#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE
OT_DECL__FUNC(0, CoapSecure, bool, IsConnected)
OT_DECL__FUNC(0, CoapSecure, bool, IsConnectionActive)
OT_DECL_VFUNC(1, CoapSecure, void, AddResource, otCoapResource*)
OT_DECL__FUNC(3, CoapSecure, otError, Connect, const otSockAddr*, otHandleCoapSecureClientConnect, void*)
OT_DECL__FUNC(3, CoapSecure, otError, GetPeerCertificateBase64, unsigned char*, size_t*, size_t)
OT_DECL__FUNC(3, CoapSecure, otError, SendRequest, otMessage*, otCoapResponseHandler, void*)
OT_DECL__FUNC(2, CoapSecure, otError, SendResponse, otMessage*, const otMessageInfo*)
OT_DECL__FUNC(1, CoapSecure, otError, Start, uint16_t)
OT_DECL_VFUNC(0, CoapSecure, void, Disconnect)
OT_DECL_VFUNC(1, CoapSecure, void, RemoveResource, otCoapResource*)
OT_DECL_VFUNC(2, CoapSecure, void, SetCaCertificateChain, const uint8_t*, uint32_t)
OT_DECL_VFUNC(4, CoapSecure, void, SetCertificate, const uint8_t*, uint32_t, const uint8_t*, uint32_t)
OT_DECL_VFUNC(2, CoapSecure, void, SetClientConnectedCallback, otHandleCoapSecureClientConnect, void*)
OT_DECL_VFUNC(2, CoapSecure, void, SetDefaultHandler, otCoapRequestHandler, void*)
OT_DECL_VFUNC(4, CoapSecure, void, SetPsk, const uint8_t*, uint16_t, const uint8_t*, uint16_t)
OT_DECL_VFUNC(1, CoapSecure, void, SetSslAuthMode, bool)
OT_DECL_VFUNC(0, CoapSecure, void, Stop)
#endif
#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE && OPENTHREAD_FTD
OT_DECL__FUNC(0, Commissioner, const char*, GetProvisioningUrl)
OT_DECL__FUNC(0, Commissioner, otCommissionerState, GetState)
OT_DECL__FUNC(3, Commissioner, otError, AddJoiner, const otExtAddress*, const char*, uint32_t)
OT_DECL__FUNC(4, Commissioner, otError, AnnounceBegin, uint32_t, uint8_t, uint16_t, const otIp6Address*)
OT_DECL__FUNC(7, Commissioner, otError, EnergyScan, uint32_t, uint8_t, uint16_t, uint16_t, const otIp6Address*, otCommissionerEnergyReportCallback, void*)
OT_DECL__FUNC(2, Commissioner, otError, GetNextJoinerInfo, uint16_t*, otJoinerInfo*)
OT_DECL__FUNC(5, Commissioner, otError, PanIdQuery, uint16_t, uint32_t, const otIp6Address*, otCommissionerPanIdConflictCallback, void*)
OT_DECL__FUNC(1, Commissioner, otError, RemoveJoiner, const otExtAddress*)
OT_DECL__FUNC(2, Commissioner, otError, SendMgmtGet, const uint8_t*, uint8_t)
OT_DECL__FUNC(3, Commissioner, otError, SendMgmtSet, const otCommissioningDataset*, const uint8_t*, uint8_t)
OT_DECL__FUNC(1, Commissioner, otError, SetProvisioningUrl, const char*)
OT_DECL__FUNC(3, Commissioner, otError, Start, otCommissionerStateCallback, otCommissionerJoinerCallback, void*)
OT_DECL__FUNC(0, Commissioner, otError, Stop)
OT_DECL__FUNC(0, Commissioner, uint16_t, GetSessionId)
#endif
otError OTAPI::Crypto::EcdsaSign(uint8_t* a, uint16_t* b, const uint8_t* c, uint16_t d, const uint8_t* e, uint16_t f) { return otCryptoEcdsaSign(a,b,c,d,e,f); }
void OTAPI::Crypto::AesCcm(const uint8_t* a, uint16_t b, uint8_t c, const void* d, uint8_t e, const void* f, uint32_t g, void* h, void* i, uint32_t j, bool k, void* l) { otCryptoAesCcm(a,b,c,d,e,f,g,h,i,j,k,l); }
void OTAPI::Crypto::HmacSha256(const uint8_t* a, uint16_t b, const uint8_t* c, uint16_t d, otCryptoSha256Hash* e) { return otCryptoHmacSha256(a,b,c,d,e); }
OT_DECL__FUNC(0, Dataset, bool, IsCommissioned)
otError OTAPI::Dataset::GeneratePskc(const char* a, const otNetworkName* b, const otExtendedPanId* c, otPskc* d) { return otDatasetGeneratePskc(a,b,c,d); }
OT_DECL__FUNC(1, Dataset, otError, GetActive, otOperationalDataset*)
OT_DECL__FUNC(1, Dataset, otError, GetPending, otOperationalDataset*)
OT_DECL__FUNC(4, Dataset, otError, SendMgmtActiveGet, const otOperationalDatasetComponents*, const uint8_t*, uint8_t, const otIp6Address*)
OT_DECL__FUNC(3, Dataset, otError, SendMgmtActiveSet, const otOperationalDataset*, const uint8_t*, uint8_t)
OT_DECL__FUNC(4, Dataset, otError, SendMgmtPendingGet, const otOperationalDatasetComponents*, const uint8_t*, uint8_t, const otIp6Address*)
OT_DECL__FUNC(3, Dataset, otError, SendMgmtPendingSet, const otOperationalDataset*, const uint8_t*, uint8_t)
OT_DECL__FUNC(1, Dataset, otError, SetActive, const otOperationalDataset*)
OT_DECL__FUNC(1, Dataset, otError, SetPending, const otOperationalDataset*)
OT_DECL__FUNC(1, Dataset, otError, CreateNewNetwork, otOperationalDataset*)
OT_DECL__FUNC(1, Dataset, otError, SetDelayTimerMinimal, uint32_t)
OT_DECL__FUNC(0, Dataset, uint32_t, GetDelayTimerMinimal)
#if OPENTHREAD_CONFIG_DIAG_ENABLE
OT_DECL__FUNC(0, Diag, bool, IsEnabled)
OT_DECL__FUNC(4, Diag, otError, ProcessCmd, uint8_t, char**, char*, size_t)
OT_DECL_VFUNC(3, Diag, void, ProcessCmdLine, const char*, char*, size_t)
#endif
#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
//OT_DECL__FUNC(3, DnsClient, otError, Query, const otDnsQuery*, otDnsResponseHandler, void*)
#endif
mbedtls_entropy_context* OTAPI::Entropy::MbedTlsContextGet() { return otEntropyMbedTlsContextGet(); }
const char* OTAPI::Thread::ErrorToString(otError a) { return otThreadErrorToString(a); }
void OTAPI::Heap::Free(void* a) { return otHeapFree(a); }
void* OTAPI::Heap::CAlloc(size_t a, size_t b) { return otHeapCAlloc(a,b); }
OT_DECL__FUNC(1, Icmp6, otError, RegisterHandler, otIcmp6Handler*)
OT_DECL__FUNC(3, Icmp6, otError, SendEchoRequest, otMessage*, const otMessageInfo*, uint16_t)
OT_DECL__FUNC(0, Icmp6, otIcmp6EchoMode, GetEchoMode)
OT_DECL_VFUNC(1, Icmp6, void, SetEchoMode, otIcmp6EchoMode)
OT_DECL__FUNC(0, Instance, bool, IsInitialized)
OT_DECL__FUNC(0, Instance, otError, ErasePersistentInfo)
otInstance* OTAPI::Instance::Init(void* a, size_t* b) { return otInstanceInit(a,b); }
otInstance* OTAPI::Instance::InitSingle() { return otInstanceInitSingle(); }
OT_DECL_VFUNC(0, Instance, void, FactoryReset)
OT_DECL_VFUNC(0, Instance, void, Finalize)
OT_DECL_VFUNC(0, Instance, void, Reset)
bool OTAPI::Ip6::IsAddressEqual(const otIp6Address* a, const otIp6Address* b) { return otIp6IsAddressEqual(a,b); }
bool OTAPI::Ip6::IsAddressUnspecified(const otIp6Address* a) { return otIp6IsAddressUnspecified(a); }
OT_DECL__FUNC(0, Ip6, bool, IsEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsMulticastPromiscuousEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsReceiveFilterEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsSlaacEnabled)
OT_DECL__FUNC(0, Ip6, const otNetifAddress*, GetUnicastAddresses)
OT_DECL__FUNC(0, Ip6, const otNetifMulticastAddress*, GetMulticastAddresses)
OT_DECL__FUNC(1, Ip6, const uint16_t*, GetUnsecurePorts, uint8_t*)
OT_DECL__FUNC(1, Ip6, otError, AddUnicastAddress, const otNetifAddress*)
OT_DECL__FUNC(1, Ip6, otError, AddUnsecurePort, uint16_t)
otError OTAPI::Ip6::AddressFromString(const char* a, otIp6Address* b) { return otIp6AddressFromString(a,b); }
OT_DECL__FUNC(1, Ip6, otError, RemoveUnicastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otError, RemoveUnsecurePort, uint16_t)
OT_DECL__FUNC(1, Ip6, otError, SelectSourceAddress, otMessageInfo*)
OT_DECL__FUNC(1, Ip6, otError, Send, otMessage*)
OT_DECL__FUNC(1, Ip6, otError, SetEnabled, bool)
OT_DECL__FUNC(1, Ip6, otError, SubscribeMulticastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otError, UnsubscribeMulticastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(3, Ip6, otMessage*, NewMessageFromBuffer, const uint8_t*, uint16_t, const otMessageSettings*)
uint8_t OTAPI::Ip6::PrefixMatch(const otIp6Address* a, const otIp6Address* b) { return otIp6PrefixMatch(a,b); }
OT_DECL_VFUNC(0, Ip6, void, RemoveAllUnsecurePorts)
OT_DECL_VFUNC(2, Ip6, void, SetAddressCallback, otIp6AddressCallback, void*)
OT_DECL_VFUNC(1, Ip6, void, SetMulticastPromiscuousEnabled, bool)
OT_DECL_VFUNC(2, Ip6, void, SetReceiveCallback, otIp6ReceiveCallback, void*)
OT_DECL_VFUNC(1, Ip6, void, SetReceiveFilterEnabled, bool)
OT_DECL_VFUNC(1, Ip6, void, SetSlaacEnabled, bool)
OT_DECL_VFUNC(1, Ip6, void, SetSlaacPrefixFilter, otIp6SlaacPrefixFilter)
#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
OT_DECL__FUNC(0, JamDetection, bool, GetState)
OT_DECL__FUNC(0, JamDetection, bool, IsEnabled)
OT_DECL__FUNC(0, JamDetection, int8_t, GetRssiThreshold)
OT_DECL__FUNC(1, JamDetection, otError, SetBusyPeriod, uint8_t)
OT_DECL__FUNC(1, JamDetection, otError, SetRssiThreshold, int8_t)
OT_DECL__FUNC(1, JamDetection, otError, SetWindow, uint8_t)
OT_DECL__FUNC(2, JamDetection, otError, Start, otJamDetectionCallback, void*)
OT_DECL__FUNC(0, JamDetection, otError, Stop)
OT_DECL__FUNC(0, JamDetection, uint64_t, GetHistoryBitmap)
OT_DECL__FUNC(0, JamDetection, uint8_t, GetBusyPeriod)
OT_DECL__FUNC(0, JamDetection, uint8_t, GetWindow)
#endif
#if OPENTHREAD_CONFIG_JOINER_ENABLE
OT_DECL__FUNC(8, Joiner, otError, Start, const char*, const char*, const char*, const char*, const char*, const char*, otJoinerCallback, void*)
OT_DECL__FUNC(0, Joiner, otJoinerState, GetState)
OT_DECL__FUNC(0, Joiner, const otExtAddress*, GetId)
OT_DECL_VFUNC(0, Joiner, void, Stop)
#endif
OT_DECL__FUNC(0, Link, bool, IsActiveScanInProgress)
OT_DECL__FUNC(0, Link, bool, IsEnabled)
OT_DECL__FUNC(0, Link, bool, IsEnergyScanInProgress)
OT_DECL__FUNC(0, Link, bool, IsInTransmitState)
OT_DECL__FUNC(0, Link, bool, IsPromiscuous)
OT_DECL__FUNC(0, Link, const otExtAddress*, GetExtendedAddress)
OT_DECL__FUNC(0, Link, const otMacCounters*, GetCounters)
OT_DECL__FUNC(1, Link, const uint32_t*, GetTxDirectRetrySuccessHistogram, uint8_t*)
OT_DECL__FUNC(1, Link, const uint32_t*, GetTxIndirectRetrySuccessHistogram, uint8_t*)
OT_DECL__FUNC(1, Link, int8_t, ConvertLinkQualityToRss, uint8_t)
OT_DECL__FUNC(4, Link, otError, ActiveScan, uint32_t, uint16_t, otHandleActiveScanResult, void*)
OT_DECL__FUNC(4, Link, otError, EnergyScan, uint32_t, uint16_t, otHandleEnergyScanResult, void*)
OT_DECL__FUNC(1, Link, otError, FilterAddAddress, const otExtAddress*)
OT_DECL__FUNC(2, Link, otError, FilterAddRssIn, const otExtAddress*, int8_t)
OT_DECL__FUNC(2, Link, otError, FilterGetNextAddress, otMacFilterIterator*, otMacFilterEntry*)
OT_DECL__FUNC(2, Link, otError, FilterGetNextRssIn, otMacFilterIterator*, otMacFilterEntry*)
OT_DECL_VFUNC(1, Link, void, FilterRemoveAddress, const otExtAddress*)
OT_DECL_VFUNC(1, Link, void, FilterRemoveRssIn, const otExtAddress*)
OT_DECL_VFUNC(1, Link, void, FilterSetAddressMode, otMacFilterAddressMode)
OT_DECL__FUNC(1, Link, otError, OutOfBandTransmitRequest, otRadioFrame*)
OT_DECL__FUNC(0, Link, otError, SendDataRequest)
OT_DECL__FUNC(1, Link, otError, SetChannel, uint8_t)
//OT_DECL__FUNC(1, Link, otError, SetEnabled, bool)
OT_DECL__FUNC(1, Link, otError, SetExtendedAddress, const otExtAddress*)
OT_DECL__FUNC(1, Link, otError, SetPanId, otPanId)
OT_DECL__FUNC(1, Link, otError, SetPollPeriod, uint32_t)
OT_DECL__FUNC(1, Link, otError, SetPromiscuous, bool)
OT_DECL__FUNC(1, Link, otError, SetSupportedChannelMask, uint32_t)
OT_DECL__FUNC(0, Link, otMacFilterAddressMode, FilterGetAddressMode)
OT_DECL__FUNC(0, Link, otPanId, GetPanId)
OT_DECL__FUNC(0, Link, otShortAddress, GetShortAddress)
OT_DECL__FUNC(0, Link, uint16_t, GetCcaFailureRate)
OT_DECL__FUNC(0, Link, uint32_t, GetPollPeriod)
OT_DECL__FUNC(0, Link, uint32_t, GetSupportedChannelMask)
OT_DECL__FUNC(1, Link, uint8_t, ConvertRssToLinkQuality, int8_t)
OT_DECL__FUNC(0, Link, uint8_t, GetChannel)
OT_DECL__FUNC(0, Link, uint8_t, GetMaxFrameRetriesDirect)
OT_DECL__FUNC(0, Link, uint8_t, GetMaxFrameRetriesIndirect)
OT_DECL_VFUNC(0, Link, void, FilterClearAddresses)
OT_DECL_VFUNC(0, Link, void, FilterClearAllRssIn)
OT_DECL_VFUNC(1, Link, void, GetFactoryAssignedIeeeEui64, otExtAddress*)
OT_DECL_VFUNC(0, Link, void, ResetCounters)
OT_DECL_VFUNC(0, Link, void, ResetTxRetrySuccessHistogram)
OT_DECL_VFUNC(1, Link, void, SetMaxFrameRetriesDirect, uint8_t)
OT_DECL_VFUNC(1, Link, void, SetMaxFrameRetriesIndirect, uint8_t)
OT_DECL_VFUNC(2, Link, void, SetPcapCallback, otLinkPcapCallback, void*)
#if OPENTHREAD_CONFIG_LINK_RAW_ENABLE || OPENTHREAD_RADIO 
OT_DECL__FUNC(0, LinkRaw, bool, GetPromiscuous)
OT_DECL__FUNC(0, LinkRaw, bool, IsEnabled)
OT_DECL__FUNC(0, LinkRaw, int8_t, GetRssi)
OT_DECL__FUNC(3, LinkRaw, otError, EnergyScan, uint8_t, uint16_t, otLinkRawEnergyScanDone)
OT_DECL__FUNC(0, LinkRaw, otError, Receive)
//OT_DECL__FUNC(1, LinkRaw, otError, SetEnable, bool)
OT_DECL__FUNC(1, LinkRaw, otError, SetPromiscuous, bool)
OT_DECL__FUNC(1, LinkRaw, otError, SetShortAddress, uint16_t)
OT_DECL__FUNC(0, LinkRaw, otError, Sleep)
OT_DECL__FUNC(1, LinkRaw, otError, SrcMatchAddExtEntry, const otExtAddress*)
OT_DECL__FUNC(1, LinkRaw, otError, SrcMatchAddShortEntry, uint16_t)
OT_DECL__FUNC(0, LinkRaw, otError, SrcMatchClearExtEntries)
OT_DECL__FUNC(1, LinkRaw, otError, SrcMatchClearExtEntry, const otExtAddress*)
OT_DECL__FUNC(0, LinkRaw, otError, SrcMatchClearShortEntries)
OT_DECL__FUNC(1, LinkRaw, otError, SrcMatchClearShortEntry, uint16_t)
OT_DECL__FUNC(1, LinkRaw, otError, SrcMatchEnable, bool)
OT_DECL__FUNC(1, LinkRaw, otError, Transmit, otLinkRawTransmitDone)
OT_DECL__FUNC(0, LinkRaw, otRadioCaps, GetCaps)
OT_DECL__FUNC(0, LinkRaw, otRadioFrame*, GetTransmitBuffer)
#endif
otLogLevel OTAPI::Logging::GetLevel() { return otLoggingGetLevel(); }
void OTAPI::Logging::SetLevel(otLogLevel a) { otLoggingSetLevel(a); }
bool OTAPI::Message::IsLinkSecurityEnabled(const otMessage* a) { return otMessageIsLinkSecurityEnabled(a); }
int OTAPI::Message::Write(otMessage* a, uint16_t b, const void* c, uint16_t d) { return otMessageWrite(a,b,c,d); }
int8_t OTAPI::Message::GetRss(const otMessage* a) { return otMessageGetRss(a); }
otError OTAPI::Message::Append(otMessage* a, const void* b, uint16_t c) { return otMessageAppend(a,b,c); }
void OTAPI::Message::QueueDequeue(otMessageQueue* a, otMessage* b) { otMessageQueueDequeue(a,b); }
void OTAPI::Message::QueueEnqueue(otMessageQueue* a, otMessage* b) { otMessageQueueEnqueue(a,b); }
void OTAPI::Message::QueueEnqueueAtHead(otMessageQueue* a, otMessage* b) { otMessageQueueEnqueueAtHead(a,b); }
otError OTAPI::Message::SetLength(otMessage* a, uint16_t b) { return otMessageSetLength(a,b); }
void OTAPI::Message::SetOffset(otMessage* a, uint16_t b) { otMessageSetOffset(a,b); }
otMessage* OTAPI::Message::QueueGetHead(otMessageQueue* a) { return otMessageQueueGetHead(a); }
otMessage* OTAPI::Message::QueueGetNext(otMessageQueue* a, const otMessage* b) { return otMessageQueueGetNext(a,b); }
uint16_t OTAPI::Message::GetLength(const otMessage* a) { return otMessageGetLength(a); }
uint16_t OTAPI::Message::GetOffset(const otMessage* a) { return otMessageGetOffset(a); }
uint16_t OTAPI::Message::Read(const otMessage* a, uint16_t b, void* c, uint16_t d) { return otMessageRead(a,b,c,d); }
void OTAPI::Message::Free(otMessage* a) { return otMessageFree(a); }
OT_DECL_VFUNC(1, Message, void, GetBufferInfo, otBufferInfo*)
void OTAPI::Message::QueueInit(otMessageQueue* a) { return otMessageQueueInit(a); }
void OTAPI::Message::SetDirectTransmission(otMessage* a, bool b) { return otMessageSetDirectTransmission(a,b); }
OT_DECL__FUNC(3, NetData, otError, Get, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, NetData, otError, GetNextOnMeshPrefix, otNetworkDataIterator*, otBorderRouterConfig*)
OT_DECL__FUNC(2, NetData, otError, GetNextRoute, otNetworkDataIterator*, otExternalRouteConfig*)
OT_DECL__FUNC(2, NetData, otError, GetNextService, otNetworkDataIterator*, otServiceConfig*)
OT_DECL__FUNC(0, NetData, uint8_t, GetStableVersion)
OT_DECL__FUNC(0, NetData, uint8_t, GetVersion)
otError OTAPI::Thread::GetNextDiagnosticTlv(const otMessage* a, otNetworkDiagIterator* b, otNetworkDiagTlv* c) { return otThreadGetNextDiagnosticTlv(a,b,c); }
OT_DECL__FUNC(5, Thread, otError, SendDiagnosticGet, const otIp6Address*, const uint8_t*, uint8_t, otReceiveDiagnosticGetCallback, void*)
OT_DECL__FUNC(3, Thread, otError, SendDiagnosticReset, const otIp6Address*, const uint8_t*, uint8_t)
#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
OT_DECL__FUNC(1, NetworkTime, otError, SetSyncPeriod, uint16_t)
OT_DECL__FUNC(1, NetworkTime, otError, SetXtalThreshold, uint16_t)
OT_DECL__FUNC(1, NetworkTime, otNetworkTimeStatus, Get, uint64_t*)
OT_DECL__FUNC(0, NetworkTime, uint16_t, GetSyncPeriod)
OT_DECL__FUNC(0, NetworkTime, uint16_t, GetXtalThreshold)
OT_DECL_VFUNC(2, NetworkTime, void, SyncSetCallback, otNetworkTimeSyncCallbackFn, void*)
#endif
bool OTAPI::Plat::DiagModeGet() { return otPlatDiagModeGet(); }
OT_DECL__FUNC(4, Plat, otError, DiagProcess, uint8_t, char**, char*, size_t)
OT_DECL_VFUNC(0, Plat, void, DiagAlarmCallback)
void OTAPI::Plat::DiagChannelSet(uint8_t a) { return otPlatDiagChannelSet(a); }
void OTAPI::Plat::DiagModeSet(bool a) { return otPlatDiagModeSet(a); }
OT_DECL_VFUNC(2, Plat, void, DiagRadioReceived, otRadioFrame*, otError)
void OTAPI::Plat::DiagTxPowerSet(int8_t a) { return otPlatDiagTxPowerSet(a); }
otError OTAPI::Plat::EntropyGet(uint8_t* a, uint16_t b) { return otPlatEntropyGet(a,b); }
//void OTAPI::Plat::Log(otLogLevel, otLogRegion, const char*, ...) { return otPlatLog(); }
otError OTAPI::Plat::UdpBind(otUdpSocket* a) { OT_API_CALL_RET(otError, otPlatUdpBind(a);); }
otError OTAPI::Plat::UdpClose(otUdpSocket* a) { OT_API_CALL_RET(otError, otPlatUdpClose(a);); }
otError OTAPI::Plat::UdpConnect(otUdpSocket* a) { OT_API_CALL_RET(otError, otPlatUdpConnect(a);); }
otError OTAPI::Plat::UdpSend(otUdpSocket* a, otMessage* b, const otMessageInfo* c) { OT_API_CALL_RET(otError, otPlatUdpSend(a,b,c);); }
otError OTAPI::Plat::UdpSocket(otUdpSocket* a) { OT_API_CALL_RET(otError, otPlatUdpSocket(a);); }
mbedtls_ctr_drbg_context* OTAPI::RandomCrypto::MbedTlsContextGet() { return otRandomCryptoMbedTlsContextGet(); }
otError OTAPI::RandomCrypto::FillBuffer(uint8_t* a, uint16_t b) { return otRandomCryptoFillBuffer(a,b); }
uint16_t OTAPI::RandomNonCrypto::GetUint16() { return otRandomNonCryptoGetUint16(); }
uint16_t OTAPI::RandomNonCrypto::GetUint16InRange(uint16_t a, uint16_t b) { return otRandomNonCryptoGetUint16InRange(a,b); }
uint32_t OTAPI::RandomNonCrypto::AddJitter(uint32_t a, uint16_t b) { return otRandomNonCryptoAddJitter(a,b); }
uint32_t OTAPI::RandomNonCrypto::GetUint32() { return otRandomNonCryptoGetUint32(); }
uint32_t OTAPI::RandomNonCrypto::GetUint32InRange(uint32_t a, uint32_t b) { return otRandomNonCryptoGetUint32InRange(a,b); }
uint8_t OTAPI::RandomNonCrypto::GetUint8() { return otRandomNonCryptoGetUint8(); }
uint8_t OTAPI::RandomNonCrypto::GetUint8InRange(uint8_t a, uint8_t b) { return otRandomNonCryptoGetUint8InRange(a,b); }
void OTAPI::RandomNonCrypto::FillBuffer(uint8_t* a, uint16_t b) { return otRandomNonCryptoFillBuffer(a,b); }
#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
OT_DECL__FUNC(1, Server, otError, AddService, const otServiceConfig*)
OT_DECL__FUNC(3, Server, otError, GetNetDataLocal, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, Server, otError, GetNextService, otNetworkDataIterator*, otServiceConfig*)
OT_DECL__FUNC(0, Server, otError, Register)
OT_DECL__FUNC(3, Server, otError, RemoveService, uint32_t, const uint8_t*, uint8_t)
#endif
#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
OT_DECL__FUNC(3, SntpClient, otError, Query, const otSntpQuery*, otSntpResponseHandler, void*)
OT_DECL_VFUNC(1, SntpClient, void, SetUnixEra, uint32_t)
#endif
OT_DECL__FUNC(0, Tasklets, bool, ArePending)
OT_DECL_VFUNC(0, Tasklets, void, Process)
OT_DECL_VFUNC(0, Tasklets, void, SignalPending)
OT_DECL__FUNC(0, Thread, bool, IsDiscoverInProgress)
OT_DECL__FUNC(0, Thread, bool, IsSingleton)
OT_DECL__FUNC(0, Thread, const char*, GetNetworkName)
OT_DECL__FUNC(0, Thread, const otExtendedPanId*, GetExtendedPanId)
OT_DECL__FUNC(0, Thread, const otIp6Address*, GetLinkLocalIp6Address)
OT_DECL__FUNC(0, Thread, const otIp6Address*, GetMeshLocalEid)
OT_DECL__FUNC(0, Thread, const otIp6Address*, GetRloc)
OT_DECL__FUNC(0, Thread, const otIpCounters*, GetIp6Counters)
OT_DECL__FUNC(0, Thread, const otMasterKey*, GetMasterKey)
OT_DECL__FUNC(0, Thread, const otMeshLocalPrefix*, GetMeshLocalPrefix)
OT_DECL__FUNC(0, Thread, const otMleCounters*, GetMleCounters)
OT_DECL__FUNC(0, Thread, otDeviceRole, GetDeviceRole)
OT_DECL__FUNC(0, Thread, otError, BecomeChild)
OT_DECL__FUNC(0, Thread, otError, BecomeDetached)
OT_DECL__FUNC(6, Thread, otError, Discover, uint32_t, uint16_t, bool, bool, otHandleActiveScanResult, void*)
OT_DECL__FUNC(1, Thread, otError, GetLeaderData, otLeaderData*)
OT_DECL__FUNC(1, Thread, otError, GetLeaderRloc, otIp6Address*)
OT_DECL__FUNC(2, Thread, otError, GetNextNeighborInfo, otNeighborInfoIterator*, otNeighborInfo*)
OT_DECL__FUNC(1, Thread, otError, GetParentAverageRssi, int8_t*)
OT_DECL__FUNC(1, Thread, otError, GetParentInfo, otRouterInfo*)
OT_DECL__FUNC(1, Thread, otError, GetParentLastRssi, int8_t*)
OT_DECL__FUNC(1, Thread, otError, SetEnabled, bool)
OT_DECL__FUNC(1, Thread, otError, SetExtendedPanId, const otExtendedPanId*)
OT_DECL__FUNC(1, Thread, otError, SetLinkMode, otLinkModeConfig)
OT_DECL__FUNC(1, Thread, otError, SetMasterKey, const otMasterKey*)
OT_DECL__FUNC(1, Thread, otError, SetMeshLocalPrefix, const otMeshLocalPrefix*)
OT_DECL__FUNC(1, Thread, otError, SetNetworkName, const char*)
OT_DECL__FUNC(0, Thread, otLinkModeConfig, GetLinkMode)
OT_DECL__FUNC(0, Thread, uint16_t, GetRloc16)
uint16_t OTAPI::Thread::GetVersion() { return otThreadGetVersion(); }
OT_DECL__FUNC(0, Thread, uint32_t, GetChildTimeout)
OT_DECL__FUNC(0, Thread, uint32_t, GetKeySequenceCounter)
OT_DECL__FUNC(0, Thread, uint32_t, GetKeySwitchGuardTime)
OT_DECL__FUNC(0, Thread, uint32_t, GetPartitionId)
OT_DECL__FUNC(0, Thread, uint8_t, GetLeaderRouterId)
OT_DECL__FUNC(0, Thread, uint8_t, GetLeaderWeight)
OT_DECL_VFUNC(2, Thread, void, RegisterParentResponseCallback, otThreadParentResponseCallback, void*)
OT_DECL_VFUNC(0, Thread, void, ResetIp6Counters)
OT_DECL_VFUNC(0, Thread, void, ResetMleCounters)
OT_DECL_VFUNC(1, Thread, void, SetChildTimeout, uint32_t)
OT_DECL_VFUNC(1, Thread, void, SetKeySequenceCounter, uint32_t)
OT_DECL_VFUNC(1, Thread, void, SetKeySwitchGuardTime, uint32_t)
OT_DECL__FUNC(0, Thread, bool, IsRouterEligible)
OT_DECL__FUNC(0, Thread, const otPskc*, GetPskc)
OT_DECL__FUNC(0, Thread, int8_t, GetParentPriority)
OT_DECL__FUNC(0, Thread, otError, BecomeLeader)
OT_DECL__FUNC(0, Thread, otError, BecomeRouter)
OT_DECL__FUNC(2, Thread, otError, GetChildInfoById, uint16_t, otChildInfo*)
OT_DECL__FUNC(2, Thread, otError, GetChildInfoByIndex, uint16_t, otChildInfo*)
OT_DECL__FUNC(3, Thread, otError, GetChildNextIp6Address, uint16_t, otChildIp6AddressIterator*, otIp6Address*)
OT_DECL__FUNC(2, Thread, otError, GetNextCacheEntry, otCacheEntryInfo*, otCacheEntryIterator*)
OT_DECL__FUNC(2, Thread, otError, GetRouterInfo, uint16_t, otRouterInfo*)
OT_DECL__FUNC(1, Thread, otError, ReleaseRouterId, uint8_t)
OT_DECL__FUNC(1, Thread, otError, SetJoinerUdpPort, uint16_t)
OT_DECL__FUNC(1, Thread, otError, SetMaxAllowedChildren, uint16_t)
OT_DECL__FUNC(1, Thread, otError, SetMaxChildIpAddresses, uint8_t)
OT_DECL__FUNC(1, Thread, otError, SetParentPriority, int8_t)
OT_DECL__FUNC(1, Thread, otError, SetPreferredRouterId, uint8_t)
OT_DECL__FUNC(1, Thread, otError, SetPskc, const otPskc*)
OT_DECL__FUNC(1, Thread, otError, SetRouterEligible, bool)
OT_DECL__FUNC(0, Thread, uint16_t, GetJoinerUdpPort)
OT_DECL__FUNC(0, Thread, uint16_t, GetMaxAllowedChildren)
OT_DECL__FUNC(0, Thread, uint32_t, GetContextIdReuseDelay)
OT_DECL__FUNC(0, Thread, uint32_t, GetPreferredLeaderPartitionId)
OT_DECL__FUNC(0, Thread, uint8_t, GetLocalLeaderWeight)
OT_DECL__FUNC(0, Thread, uint8_t, GetMaxChildIpAddresses)
OT_DECL__FUNC(0, Thread, uint8_t, GetMaxRouterId)
OT_DECL__FUNC(0, Thread, uint8_t, GetNetworkIdTimeout)
OT_DECL__FUNC(0, Thread, uint8_t, GetRouterDowngradeThreshold)
OT_DECL__FUNC(0, Thread, uint8_t, GetRouterIdSequence)
OT_DECL__FUNC(0, Thread, uint8_t, GetRouterSelectionJitter)
OT_DECL__FUNC(0, Thread, uint8_t, GetRouterUpgradeThreshold)
OT_DECL_VFUNC(1, Thread, void, RegisterNeighborTableCallback, otNeighborTableCallback)
OT_DECL_VFUNC(1, Thread, void, SetContextIdReuseDelay, uint32_t)
OT_DECL_VFUNC(1, Thread, void, SetPreferredLeaderPartitionId, uint32_t)
OT_DECL_VFUNC(1, Thread, void, SetLocalLeaderWeight, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetNetworkIdTimeout, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterDowngradeThreshold, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterSelectionJitter, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterUpgradeThreshold, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetSteeringData, const otExtAddress*)
OT_DECL__FUNC(1, Udp, otError, AddReceiver, otUdpReceiver*)
otError OTAPI::Udp::Bind(otUdpSocket* a, otSockAddr* b) { OT_API_CALL_RET(otError, otUdpBind(otrGetInstance(),a,b); ); }
otError OTAPI::Udp::Close(otUdpSocket* a) { OT_API_CALL_RET(otError, otUdpClose(otrGetInstance(),a);); }
otError OTAPI::Udp::Connect(otUdpSocket* a, otSockAddr* b) { OT_API_CALL_RET(otError, otUdpConnect(otrGetInstance(),a,b);); }
OT_DECL__FUNC(3, Udp, otError, Open, otUdpSocket*, otUdpReceive, void*)
OT_DECL__FUNC(1, Udp, otError, RemoveReceiver, otUdpReceiver*)
otError OTAPI::Udp::Send(otUdpSocket* a, otMessage* b, const otMessageInfo* c) { OT_API_CALL_RET(otError, otUdpSend(otrGetInstance(),a,b,c); ); }
OT_DECL__FUNC(2, Udp, otError, SendDatagram, otMessage*, otMessageInfo*)
OT_DECL__FUNC(1, Udp, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(0, Udp, otUdpSocket*, GetSockets)
OT_DECL_VFUNC(4, Udp, void, ForwardReceive, otMessage*, uint16_t, const otIp6Address*, uint16_t)
OT_DECL_VFUNC(2, Udp, void, ForwardSetForwarder, otUdpForwarder, void*)
