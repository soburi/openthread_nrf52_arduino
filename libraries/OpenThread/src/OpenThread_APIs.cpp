#include "OpenThread.h"
#include <openthread/openthread-freertos.h>

OT_DECL__FUNC(1, BackboneRouter, otError, GetPrimary, otBackboneRouterConfig*)
OT_DECL__FUNC(0, BackboneRouter, otBackboneRouterState, GetState)
OT_DECL__FUNC(0, BackboneRouter, otError, Register)
OT_DECL__FUNC(0, BackboneRouter, uint8_t, GetRegistrationJitter)
OT_DECL_VFUNC(1, BackboneRouter, void, GetConfig, otBackboneRouterConfig*)
OT_DECL_VFUNC(1, BackboneRouter, void, SetConfig, const otBackboneRouterConfig*)
OT_DECL_VFUNC(1, BackboneRouter, void, SetEnabled, bool)
OT_DECL_VFUNC(1, BackboneRouter, void, SetRegistrationJitter, uint8_t)
OT_DECL__FUNC(0, BorderAgent, otBorderAgentState, GetState)
OT_DECL__FUNC(1, BorderRouter, otError, AddOnMeshPrefix, const otBorderRouterConfig*)
OT_DECL__FUNC(1, BorderRouter, otError, AddRoute, const otExternalRouteConfig*)
OT_DECL__FUNC(3, BorderRouter, otError, GetNetData, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, BorderRouter, otError, GetNextOnMeshPrefix, otNetworkDataIterator*, otBorderRouterConfig*)
OT_DECL__FUNC(2, BorderRouter, otError, GetNextRoute, otNetworkDataIterator*, otExternalRouteConfig*)
OT_DECL__FUNC(0, BorderRouter, otError, Register)
OT_DECL__FUNC(1, BorderRouter, otError, RemoveOnMeshPrefix, const otIp6Prefix*)
OT_DECL__FUNC(1, BorderRouter, otError, RemoveRoute, const otIp6Prefix*)
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
OT_DECL__FUNC(0, ChannelMonitor, bool, IsEnabled)
OT_DECL__FUNC(0, ChannelMonitor, int8_t, GetRssiThreshold)
OT_DECL__FUNC(1, ChannelMonitor, otError, SetEnabled, bool)
OT_DECL__FUNC(1, ChannelMonitor, uint16_t, GetChannelOccupancy, uint8_t)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleCount)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleInterval)
OT_DECL__FUNC(0, ChannelMonitor, uint32_t, GetSampleWindow)
OT_DECL__FUNC(0, ChildSupervision, uint16_t, GetCheckTimeout)
OT_DECL__FUNC(0, ChildSupervision, uint16_t, GetInterval)
OT_DECL_VFUNC(1, ChildSupervision, void, SetCheckTimeout, uint16_t)
OT_DECL_VFUNC(1, ChildSupervision, void, SetInterval, uint16_t)
/*OT_DECL__FUNC(1, Cli, AppendResult, void, otError)*/
OT_DECL_VFUNC(2, Cli, void, ConsoleInit, otCliConsoleOutputCallback, void*)
/*OT_DECL__FUNC(2, Cli, ConsoleInputLine, void, char*, uint16_t)*/
/*OT_DECL__FUNC(2, Cli, Output, void, const char*, uint16_t)*/
/*OT_DECL__FUNC(2, Cli, OutputBytes, void, const uint8_t*, uint8_t)*/
/*OT_DECL__FUNC(2, Cli, OutputFormat, void, const char*, ...)*/
/*OT_DECL__FUNC(4, Cli, PlatLogv, void, otLogLevel, otLogRegion, const char*, va_list)*/
/*OT_DECL__FUNC(2, Cli, SetUserCommands, void, const otCliCommand*, uint8_t)*/
OT_DECL_VFUNC(0, Cli, void, UartInit)
/*OT_DECL__FUNC(1, Coap, MessageCodeToString, const char*, const otMessage*)*/
/*OT_DECL__FUNC(1, Coap, OptionIteratorGetFirstOption, const otCoapOption*, otCoapOptionIterator*)*/
/*OT_DECL__FUNC(2, Coap, OptionIteratorGetFirstOptionMatching, const otCoapOption*, otCoapOptionIterator*, uint16_t)*/
/*OT_DECL__FUNC(1, Coap, OptionIteratorGetNextOption, const otCoapOption*, otCoapOptionIterator*)*/
/*OT_DECL__FUNC(2, Coap, OptionIteratorGetNextOptionMatching, const otCoapOption*, otCoapOptionIterator*, uint16_t)*/
/*OT_DECL__FUNC(1, Coap, MessageGetToken, const uint8_t*, const otMessage*)*/
/*OT_DECL__FUNC(1, Coap, MessageGetCode, otCoapCode, const otMessage*)*/
/*OT_DECL__FUNC(1, Coap, MessageGetType, otCoapType, const otMessage*)*/
OT_DECL__FUNC(1, Coap, otError, AddResource, otCoapResource*)
/*OT_DECL__FUNC(4, Coap, MessageAppendBlock1Option, otError, otMessage*, uint32_t, bool, otCoapBlockSize)*/
/*OT_DECL__FUNC(4, Coap, MessageAppendBlock2Option, otError, otMessage*, uint32_t, bool, otCoapBlockSize)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendContentFormatOption, otError, otMessage*, otCoapOptionContentFormat)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendMaxAgeOption, otError, otMessage*, uint32_t)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendObserveOption, otError, otMessage*, uint32_t)*/
/*OT_DECL__FUNC(4, Coap, MessageAppendOption, otError, otMessage*, uint16_t, uint16_t, const void*)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendProxyUriOption, otError, otMessage*, const char*)*/
/*OT_DECL__FUNC(3, Coap, MessageAppendUintOption, otError, otMessage*, uint16_t, uint32_t)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendUriPathOptions, otError, otMessage*, const char*)*/
/*OT_DECL__FUNC(2, Coap, MessageAppendUriQueryOption, otError, otMessage*, const char*)*/
/*OT_DECL__FUNC(4, Coap, MessageInitResponse, otError, otMessage*, const otMessage*, otCoapType, otCoapCode)*/
/*OT_DECL__FUNC(1, Coap, MessageSetPayloadMarker, otError, otMessage*)*/
/*OT_DECL__FUNC(3, Coap, MessageSetToken, otError, otMessage*, const uint8_t*, uint8_t)*/
/*OT_DECL__FUNC(2, Coap, OptionIteratorGetOptionUintValue, otError, otCoapOptionIterator*, uint64_t*)*/
/*OT_DECL__FUNC(2, Coap, OptionIteratorGetOptionValue, otError, otCoapOptionIterator*, void*)*/
/*OT_DECL__FUNC(2, Coap, OptionIteratorInit, otError, otCoapOptionIterator*, const otMessage*)*/
OT_DECL__FUNC(5, Coap, otError, SendRequestWithParameters, otMessage*, const otMessageInfo*, otCoapResponseHandler, void*, const otCoapTxParameters*)
OT_DECL__FUNC(3, Coap, otError, SendResponseWithParameters, otMessage*, const otMessageInfo*, const otCoapTxParameters*)
OT_DECL__FUNC(1, Coap, otError, Start, uint16_t)
OT_DECL__FUNC(0, Coap, otError, Stop)
OT_DECL__FUNC(1, Coap, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(4, Coap, otError, SendRequest, otMessage*, const otMessageInfo*, otCoapResponseHandler, void*)
OT_DECL__FUNC(2, Coap, otError, SendResponse, otMessage*, const otMessageInfo*)
/*OT_DECL__FUNC(1, Coap, BlockSizeFromExponent, uint16_t, otCoapBlockSize)*/
/*OT_DECL__FUNC(1, Coap, MessageGetMessageId, uint16_t, const otMessage*)*/
/*OT_DECL__FUNC(1, Coap, MessageGetTokenLength, uint8_t, const otMessage*)*/
/*OT_DECL__FUNC(2, Coap, MessageGenerateToken, void, otMessage*, uint8_t)*/
/*OT_DECL__FUNC(3, Coap, MessageInit, void, otMessage*, otCoapType, otCoapCode)*/
OT_DECL_VFUNC(1, Coap, void, RemoveResource, otCoapResource*)
OT_DECL_VFUNC(2, Coap, void, SetDefaultHandler, otCoapRequestHandler, void*)
OT_DECL__FUNC(0, CoapSecure, bool, IsConnected)
OT_DECL__FUNC(0, CoapSecure, bool, IsConnectionActive)
OT_DECL__FUNC(1, CoapSecure, otError, AddResource, otCoapResource*)
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
/*OT_DECL__FUNC(6, Crypto, EcdsaSign, otError, uint8_t*, uint16_t*, const uint8_t*, uint16_t, const uint8_t*, uint16_t)*/
/*OT_DECL__FUNC(12, Crypto, AesCcm, void, const uint8_t*, uint16_t, uint8_t, const void*, uint8_t, const void*, uint32_t, void*, void*, uint32_t, bool, void*)*/
/*OT_DECL__FUNC(5, Crypto, HmacSha256, void, const uint8_t*, uint16_t, const uint8_t*, uint16_t, uint8_t*)*/
OT_DECL__FUNC(0, Dataset, bool, IsCommissioned)
/*OT_DECL__FUNC(4, Dataset, GeneratePskc, otError, const char*, const otNetworkName*, const otExtendedPanId*, otPskc*)*/
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
OT_DECL__FUNC(0, Diag, bool, IsEnabled)
OT_DECL__FUNC(4, Diag, otError, ProcessCmd, uint8_t, char**, char*, size_t)
OT_DECL_VFUNC(3, Diag, void, ProcessCmdLine, const char*, char*, size_t)
OT_DECL__FUNC(3, DnsClient, otError, Query, const otDnsQuery*, otDnsResponseHandler, void*)
/*OT_DECL__FUNC(1, Entropy, MbedTlsContextGet, mbedtls_entropy_context*, void)*/
/*OT_DECL__FUNC(1, Thread, ErrorToString, const char*, otError)*/
/*OT_DECL__FUNC(1, Heap, Free, void, void*)*/
/*OT_DECL__FUNC(2, Heap, SetCAllocFree, void, otHeapCAllocFn, otHeapFreeFn)*/
/*OT_DECL__FUNC(2, Heap, CAlloc, void*, size_t, size_t)*/
OT_DECL__FUNC(1, Icmp6, otError, RegisterHandler, otIcmp6Handler*)
OT_DECL__FUNC(3, Icmp6, otError, SendEchoRequest, otMessage*, const otMessageInfo*, uint16_t)
OT_DECL__FUNC(0, Icmp6, otIcmp6EchoMode, GetEchoMode)
OT_DECL_VFUNC(1, Icmp6, void, SetEchoMode, otIcmp6EchoMode)
OT_DECL__FUNC(0, Instance, bool, IsInitialized)
OT_DECL__FUNC(0, Instance, otError, ErasePersistentInfo)
/*OT_DECL__FUNC(2, Instance, Init, otInstance*, void*, size_t*)*/
/*OT_DECL__FUNC(1, Instance, InitSingle, otInstance*, void)*/
OT_DECL_VFUNC(0, Instance, void, FactoryReset)
OT_DECL_VFUNC(0, Instance, void, Finalize)
OT_DECL_VFUNC(0, Instance, void, Reset)
/*OT_DECL__FUNC(2, Ip6, IsAddressEqual, bool, const otIp6Address*, const otIp6Address*)*/
/*OT_DECL__FUNC(1, Ip6, IsAddressUnspecified, bool, const otIp6Address*)*/
OT_DECL__FUNC(0, Ip6, bool, IsEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsMulticastPromiscuousEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsReceiveFilterEnabled)
OT_DECL__FUNC(0, Ip6, bool, IsSlaacEnabled)
OT_DECL__FUNC(0, Ip6, const otNetifAddress*, GetUnicastAddresses)
OT_DECL__FUNC(0, Ip6, const otNetifMulticastAddress*, GetMulticastAddresses)
OT_DECL__FUNC(1, Ip6, const uint16_t*, GetUnsecurePorts, uint8_t*)
OT_DECL__FUNC(1, Ip6, otError, AddUnicastAddress, const otNetifAddress*)
OT_DECL__FUNC(1, Ip6, otError, AddUnsecurePort, uint16_t)
/*OT_DECL__FUNC(2, Ip6, AddressFromString, otError, const char*, otIp6Address*)*/
OT_DECL__FUNC(1, Ip6, otError, RemoveUnicastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otError, RemoveUnsecurePort, uint16_t)
OT_DECL__FUNC(1, Ip6, otError, SelectSourceAddress, otMessageInfo*)
OT_DECL__FUNC(1, Ip6, otError, Send, otMessage*)
OT_DECL__FUNC(1, Ip6, otError, SetEnabled, bool)
OT_DECL__FUNC(1, Ip6, otError, SubscribeMulticastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otError, UnsubscribeMulticastAddress, const otIp6Address*)
OT_DECL__FUNC(1, Ip6, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(3, Ip6, otMessage*, NewMessageFromBuffer, const uint8_t*, uint16_t, const otMessageSettings*)
/*OT_DECL__FUNC(2, Ip6, PrefixMatch, uint8_t, const otIp6Address*, const otIp6Address*)*/
OT_DECL_VFUNC(0, Ip6, void, RemoveAllUnsecurePorts)
OT_DECL_VFUNC(2, Ip6, void, SetAddressCallback, otIp6AddressCallback, void*)
OT_DECL_VFUNC(1, Ip6, void, SetMulticastPromiscuousEnabled, bool)
OT_DECL_VFUNC(2, Ip6, void, SetReceiveCallback, otIp6ReceiveCallback, void*)
OT_DECL_VFUNC(1, Ip6, void, SetReceiveFilterEnabled, bool)
OT_DECL_VFUNC(1, Ip6, void, SetSlaacEnabled, bool)
OT_DECL_VFUNC(1, Ip6, void, SetSlaacPrefixFilter, otIp6SlaacPrefixFilter)
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
OT_DECL__FUNC(8, Joiner, otError, Start, const char*, const char*, const char*, const char*, const char*, const char*, otJoinerCallback, void*)
OT_DECL__FUNC(0, Joiner, otJoinerState, GetState)
OT_DECL_VFUNC(1, Joiner, void, GetId, otExtAddress*)
OT_DECL_VFUNC(0, Joiner, void, Stop)
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
OT_DECL__FUNC(1, Link, otError, FilterRemoveAddress, const otExtAddress*)
OT_DECL__FUNC(1, Link, otError, FilterRemoveRssIn, const otExtAddress*)
OT_DECL__FUNC(1, Link, otError, FilterSetAddressMode, otMacFilterAddressMode)
OT_DECL__FUNC(1, Link, otError, OutOfBandTransmitRequest, otRadioFrame*)
OT_DECL__FUNC(0, Link, otError, SendDataRequest)
OT_DECL__FUNC(1, Link, otError, SetChannel, uint8_t)
OT_DECL__FUNC(1, Link, otError, SetEnabled, bool)
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
OT_DECL_VFUNC(0, Link, void, FilterClearRssIn)
OT_DECL_VFUNC(1, Link, void, GetFactoryAssignedIeeeEui64, otExtAddress*)
OT_DECL_VFUNC(0, Link, void, ResetCounters)
OT_DECL_VFUNC(0, Link, void, ResetTxRetrySuccessHistogram)
OT_DECL_VFUNC(1, Link, void, SetMaxFrameRetriesDirect, uint8_t)
OT_DECL_VFUNC(1, Link, void, SetMaxFrameRetriesIndirect, uint8_t)
OT_DECL_VFUNC(2, Link, void, SetPcapCallback, otLinkPcapCallback, void*)
OT_DECL__FUNC(0, Link, bool, RawGetPromiscuous)
OT_DECL__FUNC(0, Link, bool, RawIsEnabled)
OT_DECL__FUNC(0, Link, int8_t, RawGetRssi)
OT_DECL__FUNC(3, Link, otError, RawEnergyScan, uint8_t, uint16_t, otLinkRawEnergyScanDone)
OT_DECL__FUNC(1, Link, otError, RawReceive, otLinkRawReceiveDone)
OT_DECL__FUNC(1, Link, otError, RawSetEnable, bool)
OT_DECL__FUNC(1, Link, otError, RawSetPromiscuous, bool)
OT_DECL__FUNC(1, Link, otError, RawSetShortAddress, uint16_t)
OT_DECL__FUNC(0, Link, otError, RawSleep)
OT_DECL__FUNC(1, Link, otError, RawSrcMatchAddExtEntry, const otExtAddress*)
OT_DECL__FUNC(1, Link, otError, RawSrcMatchAddShortEntry, uint16_t)
OT_DECL__FUNC(0, Link, otError, RawSrcMatchClearExtEntries)
OT_DECL__FUNC(1, Link, otError, RawSrcMatchClearExtEntry, const otExtAddress*)
OT_DECL__FUNC(0, Link, otError, RawSrcMatchClearShortEntries)
OT_DECL__FUNC(1, Link, otError, RawSrcMatchClearShortEntry, uint16_t)
OT_DECL__FUNC(1, Link, otError, RawSrcMatchEnable, bool)
OT_DECL__FUNC(1, Link, otError, RawTransmit, otLinkRawTransmitDone)
OT_DECL__FUNC(0, Link, otRadioCaps, RawGetCaps)
OT_DECL__FUNC(0, Link, otRadioFrame*, RawGetTransmitBuffer)
/*OT_DECL__FUNC(1, Logging, GetLevel, otLogLevel, void)*/
/*OT_DECL__FUNC(1, Logging, SetLevel, void, otLogLevel)*/
/*OT_DECL__FUNC(1, Message, IsLinkSecurityEnabled, bool, const otMessage*)*/
/*OT_DECL__FUNC(4, Message, Write, int, otMessage*, uint16_t, const void*, uint16_t)*/
/*OT_DECL__FUNC(1, Message, GetRss, int8_t, const otMessage*)*/
/*OT_DECL__FUNC(3, Message, Append, otError, otMessage*, const void*, uint16_t)*/
/*OT_DECL__FUNC(2, Message, QueueDequeue, otError, otMessageQueue*, otMessage*)*/
/*OT_DECL__FUNC(2, Message, QueueEnqueue, otError, otMessageQueue*, otMessage*)*/
/*OT_DECL__FUNC(2, Message, QueueEnqueueAtHead, otError, otMessageQueue*, otMessage*)*/
/*OT_DECL__FUNC(2, Message, SetLength, otError, otMessage*, uint16_t)*/
/*OT_DECL__FUNC(2, Message, SetOffset, otError, otMessage*, uint16_t)*/
/*OT_DECL__FUNC(1, Message, QueueGetHead, otMessage*, otMessageQueue*)*/
/*OT_DECL__FUNC(2, Message, QueueGetNext, otMessage*, otMessageQueue*, const otMessage*)*/
/*OT_DECL__FUNC(1, Message, GetLength, uint16_t, const otMessage*)*/
/*OT_DECL__FUNC(1, Message, GetOffset, uint16_t, const otMessage*)*/
/*OT_DECL__FUNC(4, Message, Read, uint16_t, const otMessage*, uint16_t, void*, uint16_t)*/
/*OT_DECL__FUNC(1, Message, Free, void, otMessage*)*/
OT_DECL_VFUNC(1, Message, void, GetBufferInfo, otBufferInfo*)
/*OT_DECL__FUNC(1, Message, QueueInit, void, otMessageQueue*)*/
/*OT_DECL__FUNC(2, Message, SetDirectTransmission, void, otMessage*, bool)*/
/*OT_DECL__FUNC(3, Ncp, StreamWrite, otError, int, const uint8_t*, int)*/
/*OT_DECL__FUNC(1, Ncp, HandleDidReceiveNewLegacyUlaPrefix, void, const uint8_t*)*/
/*OT_DECL__FUNC(1, Ncp, HandleLegacyNodeDidJoin, void, const otExtAddress*)*/
OT_DECL_VFUNC(0, Ncp, void, Init)
/*OT_DECL__FUNC(4, Ncp, PlatLogv, void, otLogLevel, otLogRegion, const char*, va_list)*/
/*OT_DECL__FUNC(1, Ncp, RegisterLegacyHandlers, void, const otNcpLegacyHandlers*)*/
/*OT_DECL__FUNC(2, Ncp, RegisterPeekPokeDelagates, void, otNcpDelegateAllowPeekPoke, otNcpDelegateAllowPeekPoke)*/
OT_DECL__FUNC(3, NetData, otError, Get, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, NetData, otError, GetNextOnMeshPrefix, otNetworkDataIterator*, otBorderRouterConfig*)
OT_DECL__FUNC(2, NetData, otError, GetNextRoute, otNetworkDataIterator*, otExternalRouteConfig*)
OT_DECL__FUNC(2, NetData, otError, GetNextService, otNetworkDataIterator*, otServiceConfig*)
OT_DECL__FUNC(0, NetData, uint8_t, GetStableVersion)
OT_DECL__FUNC(0, NetData, uint8_t, GetVersion)
/*OT_DECL__FUNC(3, Thread, GetNextDiagnosticTlv, otError, const otMessage*, otNetworkDiagIterator*, otNetworkDiagTlv*)*/
OT_DECL__FUNC(3, Thread, otError, SendDiagnosticGet, const otIp6Address*, const uint8_t*, uint8_t)
OT_DECL__FUNC(3, Thread, otError, SendDiagnosticReset, const otIp6Address*, const uint8_t*, uint8_t)
OT_DECL_VFUNC(2, Thread, void, SetReceiveDiagnosticGetCallback, otReceiveDiagnosticGetCallback, void*)
OT_DECL__FUNC(1, NetworkTime, otError, SetSyncPeriod, uint16_t)
OT_DECL__FUNC(1, NetworkTime, otError, SetXtalThreshold, uint16_t)
OT_DECL__FUNC(1, NetworkTime, otNetworkTimeStatus, Get, uint64_t*)
OT_DECL__FUNC(0, NetworkTime, uint16_t, GetSyncPeriod)
OT_DECL__FUNC(0, NetworkTime, uint16_t, GetXtalThreshold)
OT_DECL_VFUNC(2, NetworkTime, void, SyncSetCallback, otNetworkTimeSyncCallbackFn, void*)
/*OT_DECL__FUNC(1, Plat, DiagModeGet, bool, void)*/
OT_DECL__FUNC(4, Plat, otError, DiagProcess, uint8_t, char**, char*, size_t)
OT_DECL_VFUNC(0, Plat, void, DiagAlarmCallback)
/*OT_DECL__FUNC(1, Plat, DiagChannelSet, void, uint8_t)*/
/*OT_DECL__FUNC(1, Plat, DiagModeSet, void, bool)*/
OT_DECL_VFUNC(2, Plat, void, DiagRadioReceived, otRadioFrame*, otError)
/*OT_DECL__FUNC(1, Plat, DiagTxPowerSet, void, int8_t)*/
/*OT_DECL__FUNC(2, Plat, EntropyGet, otError, uint8_t*, uint16_t)*/
/*OT_DECL__FUNC(4, Plat, Log, void, otLogLevel, otLogRegion, const char*, ...)*/
/*OT_DECL__FUNC(1, Plat, UdpBind, otError, otUdpSocket*)*/
/*OT_DECL__FUNC(1, Plat, UdpClose, otError, otUdpSocket*)*/
/*OT_DECL__FUNC(1, Plat, UdpConnect, otError, otUdpSocket*)*/
/*OT_DECL__FUNC(3, Plat, UdpSend, otError, otUdpSocket*, otMessage*, const otMessageInfo*)*/
/*OT_DECL__FUNC(1, Plat, UdpSocket, otError, otUdpSocket*)*/
/*OT_DECL__FUNC(1, RandomCrypto, MbedTlsContextGet, mbedtls_ctr_drbg_context*, void)*/
/*OT_DECL__FUNC(2, RandomCrypto, FillBuffer, otError, uint8_t*, uint16_t)*/
/*OT_DECL__FUNC(1, RandomNonCrypto, GetUint16, uint16_t, void)*/
/*OT_DECL__FUNC(2, RandomNonCrypto, GetUint16InRange, uint16_t, uint16_t, uint16_t)*/
/*OT_DECL__FUNC(2, RandomNonCrypto, AddJitter, uint32_t, uint32_t, uint16_t)*/
/*OT_DECL__FUNC(1, RandomNonCrypto, GetUint32, uint32_t, void)*/
/*OT_DECL__FUNC(2, RandomNonCrypto, GetUint32InRange, uint32_t, uint32_t, uint32_t)*/
/*OT_DECL__FUNC(1, RandomNonCrypto, GetUint8, uint8_t, void)*/
/*OT_DECL__FUNC(2, RandomNonCrypto, GetUint8InRange, uint8_t, uint8_t, uint8_t)*/
/*OT_DECL__FUNC(2, RandomNonCrypto, FillBuffer, void, uint8_t*, uint16_t)*/
OT_DECL__FUNC(1, Server, otError, AddService, const otServiceConfig*)
OT_DECL__FUNC(3, Server, otError, GetNetDataLocal, bool, uint8_t*, uint8_t*)
OT_DECL__FUNC(2, Server, otError, GetNextService, otNetworkDataIterator*, otServiceConfig*)
OT_DECL__FUNC(0, Server, otError, Register)
OT_DECL__FUNC(3, Server, otError, RemoveService, uint32_t, const uint8_t*, uint8_t)
OT_DECL__FUNC(3, SntpClient, otError, Query, const otSntpQuery*, otSntpResponseHandler, void*)
OT_DECL_VFUNC(1, SntpClient, void, SetUnixEra, uint32_t)
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
/*OT_DECL__FUNC(1, Thread, GetVersion, uint16_t, void)*/
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
OT_DECL__FUNC(0, Thread, uint32_t, GetLocalLeaderPartitionId)
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
OT_DECL_VFUNC(1, Thread, void, SetLocalLeaderPartitionId, uint32_t)
OT_DECL_VFUNC(1, Thread, void, SetLocalLeaderWeight, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetNetworkIdTimeout, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterDowngradeThreshold, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterSelectionJitter, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetRouterUpgradeThreshold, uint8_t)
OT_DECL_VFUNC(1, Thread, void, SetSteeringData, const otExtAddress*)
OT_DECL__FUNC(1, Udp, otError, AddReceiver, otUdpReceiver*)
/*OT_DECL__FUNC(2, Udp, Bind, otError, otUdpSocket*, otSockAddr*)*/
/*OT_DECL__FUNC(1, Udp, Close, otError, otUdpSocket*)*/
/*OT_DECL__FUNC(2, Udp, Connect, otError, otUdpSocket*, otSockAddr*)*/
OT_DECL__FUNC(3, Udp, otError, Open, otUdpSocket*, otUdpReceive, void*)
OT_DECL__FUNC(1, Udp, otError, RemoveReceiver, otUdpReceiver*)
/*OT_DECL__FUNC(3, Udp, Send, otError, otUdpSocket*, otMessage*, const otMessageInfo*)*/
OT_DECL__FUNC(2, Udp, otError, SendDatagram, otMessage*, otMessageInfo*)
OT_DECL__FUNC(1, Udp, otMessage*, NewMessage, const otMessageSettings*)
OT_DECL__FUNC(0, Udp, otUdpSocket*, GetSockets)
OT_DECL_VFUNC(4, Udp, void, ForwardReceive, otMessage*, uint16_t, const otIp6Address*, uint16_t)
OT_DECL_VFUNC(2, Udp, void, ForwardSetForwarder, otUdpForwarder, void*)
