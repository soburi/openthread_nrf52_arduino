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

#include "OpenThread_types.hpp"

extern "C" {
#include <openthread/openthread-freertos.h>
}

#include <openthread/border_router.h>


template<>
otExtAddress OTm8Buffer<otExtAddress>::default_value = { .m8 = { 0,0,0,0,0,0,0,0 } };
template<>
otExtendedPanId OTm8Buffer<otExtendedPanId>::default_value = { .m8 = { 0,0,0,0,0,0,0,0 } };
template<>
otMasterKey OTm8Buffer<otMasterKey>::default_value = { .m8 = { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 } };
template<>
otNetworkName OTm8Buffer<otNetworkName>::default_value = { .m8 = { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0 } };
template<>
otPskc OTm8Buffer<otPskc>::default_value = { .m8 = { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 } };

#if OPENTHREAD_FTD
otChildInfo OTChildInfo::default_value = { 0 };
#endif
otExternalRouteConfig OTExternalRouteConfig::default_value = { 0 };
otRouterInfo OTRouterInfo::default_value = { 0 };
#if OPENTHREAD_FTD
otCacheEntryInfo OTCacheEntryInfo::default_value = { 0 };
#endif
otNeighborInfo OTNeighborInfo::default_value = { 0 };
otBorderRouterConfig OTBorderRouterConfig::default_value = { 0 };
otMacFilterEntry OTMacFilterEntry::default_value = { 0 };
otNetifAddress OTNetifAddress::default_value = { 0 };
otNetifMulticastAddress OTNetifMulticastAddress::default_value = { 0 };

size_t OTMacCounters::printTo(Print& p) const {
  size_t n = 0; 
  n += p.print("{ TxTotal: "); n += p.println(value->mTxTotal);
  n += p.print(", TxUnicast: "); n += p.println(value->mTxUnicast);
  n += p.print(", TxBroadcast: "); n += p.println(value->mTxBroadcast);
  n += p.print(", TxAckRequested: "); n += p.println(value->mTxAckRequested);
  n += p.print(", TxAcked: "); n += p.println(value->mTxAcked);
  n += p.print(", TxNoAckRequested: "); n += p.println(value->mTxNoAckRequested);
  n += p.print(", TxData: "); n += p.println(value->mTxData);
  n += p.print(", TxDataPoll: "); n += p.println(value->mTxDataPoll);
  n += p.print(", TxBeacon: "); n += p.println(value->mTxBeacon);
  n += p.print(", TxBeaconRequest: "); n += p.println(value->mTxBeaconRequest);
  n += p.print(", TxOther: "); n += p.println(value->mTxOther);
  n += p.print(", TxRetry: "); n += p.println(value->mTxRetry);
  n += p.print(", TxErrCca: "); n += p.println(value->mTxErrCca);
  n += p.print(", TxErrAbort: "); n += p.println(value->mTxErrAbort);
  n += p.print(", TxErrBusyChannel: "); n += p.println(value->mTxErrBusyChannel);
  n += p.print(", RxTotal: "); n += p.println(value->mRxTotal);
  n += p.print(", RxUnicast: "); n += p.println(value->mRxUnicast);
  n += p.print(", RxBroadcast: "); n += p.println(value->mRxBroadcast);
  n += p.print(", RxData: "); n += p.println(value->mRxData);
  n += p.print(", RxDataPoll: "); n += p.println(value->mRxDataPoll);
  n += p.print(", RxBeacon: "); n += p.println(value->mRxBeacon);
  n += p.print(", RxBeaconRequest: "); n += p.println(value->mRxBeaconRequest);
  n += p.print(", RxOther: "); n += p.println(value->mRxOther);
  n += p.print(", RxAddressFiltered: "); n += p.println(value->mRxAddressFiltered);
  n += p.print(", RxDestAddrFiltered: "); n += p.println(value->mRxDestAddrFiltered);
  n += p.print(", RxDuplicated: "); n += p.println(value->mRxDuplicated);
  n += p.print(", RxErrNoFrame: "); n += p.println(value->mRxErrNoFrame);
  n += p.print(", RxErrUnknownNeighbor: "); n += p.println(value->mRxErrUnknownNeighbor);
  n += p.print(", RxErrInvalidSrcAddr: "); n += p.println(value->mRxErrInvalidSrcAddr);
  n += p.print(", RxErrSec: "); n += p.println(value->mRxErrSec);
  n += p.print(", RxErrFcs: "); n += p.println(value->mRxErrFcs);
  n += p.print(", RxErrOther: "); n += p.println(value->mRxErrOther);
  n += p.print("}");
  return n;
}

size_t OTMleCounters::printTo(Print& p) const {
  size_t n = 0; 
  n += p.print("{ mDisabledRole: "); n += p.println(value->mDisabledRole);
  n += p.print(", mDetachedRole: "); n += p.println(value->mDetachedRole);
  n += p.print(", mChildRole: "); n += p.println(value->mChildRole);
  n += p.print(", mRouterRole: "); n += p.println(value->mRouterRole);
  n += p.print(", mLeaderRole: "); n += p.println(value->mLeaderRole);
  n += p.print(", mAttachAttempts: "); n += p.println(value->mAttachAttempts);
  n += p.print(", mPartitionIdChanges: "); n += p.println(value->mPartitionIdChanges);
  n += p.print(", mBetterPartitionAttachAttempts: "); n += p.println(value->mBetterPartitionAttachAttempts);
  n += p.print("}");

  return n;
}

#if OPENTHREAD_FTD
size_t OTChildInfo::printTo(Print& p) const
{
    size_t n = 0;

    OTExtAddress exta(&value->mExtAddress);

	n += p.print("{ mExtAddress: \"");
	n += p.print(exta);
	n += p.print("\", mTimeout: ");
	n += p.print(value->mTimeout);
	n += p.print(", mAge: ");
	n += p.print(value->mAge);
	n += p.print(", mRloc16: ");
	n += p.print(value->mRloc16);
	n += p.print(", mChildId: ");
	n += p.print(value->mChildId);
	n += p.print(", mNetworkDataVersion: ");
	n += p.print(value->mNetworkDataVersion);
	n += p.print(", mLinkQualityIn: ");
	n += p.print(value->mLinkQualityIn);
	n += p.print(", mAverageRssi: ");
	n += p.print(value->mAverageRssi);
	n += p.print(", mLastRssi: ");
	n += p.print(value->mLastRssi);
	n += p.print(", mFrameErrorRate: ");
	n += p.print(value->mFrameErrorRate);
	n += p.print(", mMessageErrorRate: ");
	n += p.print(value->mMessageErrorRate);
	n += p.print(", mRxOnWhenIdle: ");
	n += p.print(value->mRxOnWhenIdle);
	n += p.print(", mFullThreadDevice: ");
	n += p.print(value->mFullThreadDevice);
	n += p.print(", mFullNetworkData: ");
	n += p.print(value->mFullNetworkData);
	n += p.print(", mIsStateRestoring: ");
	n += p.print(value->mIsStateRestoring);
	n += p.print("}");

	return n;
}
#endif

size_t OTExternalRouteConfig::printTo(Print& p) const
{
    size_t n = 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
	IPAddress prefix(value->mPrefix.mPrefix.mFields.m16);
#pragma GCC diagnostic pop
	n += p.print("{ mPrefix: { mPrefix: \"");
	n += p.print(prefix);
	n += p.print("\", mLength: ");
	n += p.print(value->mPrefix.mLength);
	n += p.print("}");
	n += p.print(", mRloc16: ");
	n += p.print(value->mRloc16);
	n += p.print(", mPreference: ");
	n += p.print(value->mPreference);
	n += p.print(", mStable: ");
	n += p.print(value->mStable);
	n += p.print(", mNextHopIsThisDevice: ");
	n += p.print(value->mNextHopIsThisDevice);
	n += p.print("}");

	return n;
}

size_t OTRouterInfo::printTo(Print& p) const
{
    size_t n = 0;

	OTExtAddress exta(&value->mExtAddress);
	n += p.print("{ mExtAddress: \"");
	n += p.print(exta);
	n += p.print("\", mRloc16: ");
	n += p.print(value->mRloc16);
	n += p.print(", mRouterId: ");
	n += p.print(value->mRouterId);
	n += p.print(", mNextHop: ");
	n += p.print(value->mNextHop);
	n += p.print(", mPathCost: ");
	n += p.print(value->mPathCost);
	n += p.print(", mLinkQualityIn: ");
	n += p.print(value->mLinkQualityIn);
	n += p.print(", mLinkQualityOut: ");
	n += p.print(value->mLinkQualityOut);
	n += p.print(", mAge: ");
	n += p.print(value->mAge);
	n += p.print(", mAllocated: ");
	n += p.print(value->mAllocated);
	n += p.print(", mLinkEstablished: ");
	n += p.print(value->mLinkEstablished);
	n += p.print("}");
	return n;
}

#if OPENTHREAD_FTD
size_t OTCacheEntryInfo::printTo(Print& p) const
{
    size_t n = 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
	IPAddress target(value->mTarget.mFields.m16);
	IPAddress meshlocaleid(value->mMeshLocalEid.mFields.m16);
#pragma GCC diagnostic pop

	n += p.print("{ mTarget: \"");
	n += p.print(target);
	n += p.print("\", mRloc16: ");
	n += p.print(value->mRloc16);
	n += p.print(", mState: ");
	n += p.print(value->mState);
	n += p.print(", mCanEvict: ");
	n += p.print(value->mCanEvict);
	n += p.print(", mValidLastTrans: ");
	n += p.print(value->mValidLastTrans);
	n += p.print(", mLastTransTime: ");
	n += p.print(value->mLastTransTime);
	n += p.print(", mMeshLocalEid: \"");
	n += p.print(meshlocaleid);
	n += p.print("\", mTimeout: ");
	n += p.print(value->mTimeout);
	n += p.print(", mRetryDelay: ");
	n += p.print(value->mRetryDelay);
	n += p.print("}");
	return n;
}
#endif

size_t OTNeighborInfo::printTo(Print& p) const
{
    size_t n = 0;

    OTExtAddress exta(&value->mExtAddress);

	n += p.print("{ mExtAddress: \"");
	n += p.print(exta);
	n += p.print("\", mAge: ");
	n += p.print(value->mAge);
	n += p.print(", mRloc16: ");
	n += p.print(value->mRloc16);
	n += p.print(", mLinkFrameCounter: ");
	n += p.print(value->mLinkFrameCounter);
	n += p.print(", mMleFrameCounter: ");
	n += p.print(value->mMleFrameCounter);
	n += p.print(", mLinkQualityIn: ");
	n += p.print(value->mLinkQualityIn);
	n += p.print(", mAverageRssi: ");
	n += p.print(value->mAverageRssi);
	n += p.print(", mLastRssi: ");
	n += p.print(value->mLastRssi);
	n += p.print(", mFrameErrorRate: ");
	n += p.print(value->mFrameErrorRate);
	n += p.print(", mMessageErrorRate: ");
	n += p.print(value->mMessageErrorRate);
	n += p.print(", mRxOnWhenIdle: ");
	n += p.print(value->mRxOnWhenIdle);
	n += p.print(", mFullThreadDevice: ");
	n += p.print(value->mFullThreadDevice);
	n += p.print(", mFullNetworkData: ");
	n += p.print(value->mFullNetworkData);
	n += p.print(", mIsChild: ");
	n += p.print(value->mIsChild);
	n += p.print("}");

	return n;
}


size_t OTBorderRouterConfig::printTo(Print& p) const
{
    size_t n = 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
	IPAddress prefix(value->mPrefix.mPrefix.mFields.m16);
#pragma GCC diagnostic pop
	n += p.print("{ mPrefix: { mPrefix: \"");
	n += p.print(prefix);
	n += p.print("\", mLength: ");
	n += p.print(value->mPrefix.mLength);
	n += p.print("}");
	n += p.print(", mPreference: ");
	n += p.print(value->mPreference);
	n += p.print(", mPreferred: ");
	n += p.print(value->mPreferred);
	n += p.print(", mSlaac: ");
	n += p.print(value->mSlaac);
	n += p.print(", mDhcp: ");
	n += p.print(value->mDhcp);
	n += p.print(", mConfigure: ");
	n += p.print(value->mConfigure);
	n += p.print(", mDefaultRoute: ");
	n += p.print(value->mDefaultRoute);
	n += p.print(", mOnMesh: ");
	n += p.print(value->mOnMesh);
	n += p.print("}");

	return n;
}


size_t OTMacFilterEntry::printTo(Print& p) const
{
    size_t n = 0;
	OTExtAddress exta(&value->mExtAddress);

	n += p.print("{ mExtAddress: \"");
	n += p.print(exta);
	n += p.print("\", mRssIn: ");
	n += p.print(value->mRssIn);
	n += p.print("}");

	return n;
}


size_t OTNetifAddress::printTo(Print& p) const
{
    size_t n = 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
	IPAddress addr(value->mAddress.mFields.m16);
#pragma GCC diagnostic pop
	n += p.print("{ mAddress: \"");
	n += p.print(addr);
	n += p.print("\", mPrefixLength: ");
	n += p.print(value->mPrefixLength);
	n += p.print(", mPreferred: ");
	n += p.print(value->mPreferred);
	n += p.print(", mValid: ");
	n += p.print(value->mValid);
	n += p.print(", mScopeOverrideValid: ");
	n += p.print(value->mScopeOverrideValid);
	n += p.print(", mScopeOverride: ");
	n += p.print(value->mScopeOverride);
	n += p.print(", mRloc: ");
	n += p.print(value->mRloc);
	n += p.print("}");
	return n;
}

size_t OTNetifMulticastAddress::printTo(Print& p) const
{
    size_t n = 0;

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
	IPAddress addr(value->mAddress.mFields.m16);
#pragma GCC diagnostic pop
	n += p.print("{ mAddress: \"");
	n += p.print(addr);
	n += p.print("\" }");

	return n;
}

size_t OTSecurityPolicy::printTo(Print& p) const
{
    size_t n = 0;

	n += p.print("{ mRotationTime: \"");
	n += p.print(value->mRotationTime);
	n += p.print("\", mObtainMasterKeyEnabled: \"");
	n += p.print(value->mObtainMasterKeyEnabled);
	n += p.print("\", mNativeCommissioningEnabled: \"");
	n += p.print(value->mNativeCommissioningEnabled);
	n += p.print("\", mRoutersEnabled: \"");
	n += p.print(value->mRoutersEnabled);
	n += p.print("\", mExternalCommissioningEnabled: \"");
	n += p.print(value->mExternalCommissioningEnabled);
	n += p.print("\", mBeaconsEnabled: \"");
	n += p.print(value->mBeaconsEnabled);
	n += p.print("\", mCommercialCommissioningEnabled: \"");
	n += p.print(value->mCommercialCommissioningEnabled);
	n += p.print("\", mAutonomousEnrollmentEnabled: \"");
	n += p.print(value->mAutonomousEnrollmentEnabled);
	n += p.print("\", mMasterKeyProvisioningEnabled: \"");
	n += p.print(value->mMasterKeyProvisioningEnabled);
	n += p.print("\", mTobleLinkEnabled: \"");
	n += p.print(value->mTobleLinkEnabled);
	n += p.print("\", mNonCcmRoutersEnabled: \"");
	n += p.print(value->mNonCcmRoutersEnabled);
	n += p.print("\", mVersionThresholdForRouting: \"");
	n += p.print(value->mVersionThresholdForRouting);
	n += p.print("}");

	return n;
}

size_t OTActiveScanResult::printTo(Print& p) const
{
    size_t n = 0;
	return n;
}

size_t OTEnergyScanResult::printTo(Print& p) const
{
    size_t n = 0;
	return n;
}

#if OPENTHREAD_FTD
OTRouterInfo OTRouterIterator::next() {
  uint16_t max_routerid;
  index++;

  OT_API_CALL(
    max_routerid = otThreadGetMaxRouterId(otrGetInstance());
  );

  if(index >= max_routerid) {
    error = OT_ERROR_FAILED;
    return nullptr;
  }

  OT_API_CALL(
    error = otThreadGetRouterInfo(otrGetInstance(), index, &entry);
  );
  return get();
}

OTChildInfo OTChildIterator::next() {
  uint16_t max_children;
  index++;

  OT_API_CALL(
    max_children = otThreadGetMaxAllowedChildren(otrGetInstance());
  );

  if(index >= max_children) {
    error = OT_ERROR_FAILED;
    return nullptr;
  }

  OT_API_CALL(
    error = otThreadGetChildInfoByIndex(otrGetInstance(), index, &entry);
  );
  return get();
}



IPAddress OTChildIpIterator::next() {
  OT_API_CALL(
    error = otThreadGetChildNextIp6Address(otrGetInstance(), index, &iter, &entry);
  );
  return get();
}

OTCacheEntryInfo OTEidCacheIterator::next() {
  OT_API_CALL(
    error = otThreadGetNextCacheEntry(otrGetInstance(), &entry, &iter);
  );
  return get();
}

OTNeighborInfo OTNeighborIterator::next() {
  OT_API_CALL(
    error = otThreadGetNextNeighborInfo(otrGetInstance(), &iter, &entry);
  );
  return get();
}
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE

OTExternalRouteConfig OTRouteIterator::next() {
  OT_API_CALL(
    error = otBorderRouterGetNextRoute(otrGetInstance(), &iter, &entry);
  );
  return get();
}

OTBorderRouterConfig OTPrefixIterator::next() {
  OT_API_CALL(
    error = otBorderRouterGetNextOnMeshPrefix(otrGetInstance(), &iter, &entry);
  );
  return get();
}
#endif //OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE

OTMacFilterEntry OTMacFilterAddrIterator::next() {
  OT_API_CALL(
    error = otLinkFilterGetNextAddress(otrGetInstance(), &iter, &entry);
  );
  return get();
}

OTMacFilterEntry OTMacFilterRssIterator::next() {
  OT_API_CALL(
    error = otLinkFilterGetNextRssIn(otrGetInstance(), &iter, &entry);
  );
  return get();
}

#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE

void OTMacFilterIterator::_next() {
  if(!addr_end) {
    error = otLinkFilterGetNextAddress(otrGetInstance(), &iter, &entry);
    if(error) {
      addr_end = true;
      error = OT_ERROR_NONE;
    }
  }
  else {
    error = otLinkFilterGetNextRssIn(otrGetInstance(), &iter, &entry);
  }
}

OTMacFilterEntry OTMacFilterIterator::next() {
  OT_API_CALL(
    _next();
  );

  return get();
}
#endif //OPENTHREAD_CONFIG_MAC_FILTER_ENABLE

OTIpaddrIterator::OTIpaddrIterator() {
  OT_API_CALL(
    addr = otIp6GetUnicastAddresses(otrGetInstance());
  );
}

OTIpmaddrIterator::OTIpmaddrIterator() {
  OT_API_CALL(
    addr = otIp6GetMulticastAddresses(otrGetInstance());
  );
}

OTActiveScanResultSyncIterator::OTActiveScanResultSyncIterator() {
  scan_sem = xSemaphoreCreateMutex();
  xSemaphoreTake(scan_sem, portMAX_DELAY);
  data[0].done = true;
  data[1].done = true;
}

void OTActiveScanResultSyncIterator::set_error(otError e)
{
  int backbuffer = 0;
  if(current == 1) backbuffer = 0;
  else             backbuffer = 1;
  data[backbuffer].error = e;

  // flip
  current = backbuffer;
}

void OTActiveScanResultSyncIterator::receive(otActiveScanResult* result)
{
  int backbuffer = 0;
  if(current == 1) backbuffer = 0;
  else             backbuffer = 1;

  if(!data[current].done) {
    //Serial.println("backbuffer.error = OT_ERROR_BUSY");
    data[backbuffer].error = OT_ERROR_BUSY;
    data[backbuffer].done = false;
  } else if(!result) {
    //Serial.println("backbuffer.error = OT_ERROR_GENERIC");
    data[backbuffer].error = OT_ERROR_GENERIC;
    data[backbuffer].done = false;
  } else {
    //Serial.println("backbuffer.error = OT_ERROR_NONE");
    data[backbuffer].error = OT_ERROR_NONE;
    data[backbuffer].result = *result;
    data[backbuffer].done = false;
  }

  // flip
  current = backbuffer;
}

bool OTActiveScanResultSyncIterator::end() {
  return (data[current].error != 0);
}

OTActiveScanResult OTActiveScanResultSyncIterator::get() {
  if(!data[current].error) {
    return &data[current].result;
  } else {
    return nullptr;
  }
}

OTActiveScanResult OTActiveScanResultSyncIterator::next() {
  data[current].done = true;
  if(!xSemaphoreTake(scan_sem, portMAX_DELAY) ) {
    data[current].error = OT_ERROR_GENERIC;
    return nullptr;
  }

  // flip in backgrounds
  return get();
}


OTEnergyScanResultSyncIterator::OTEnergyScanResultSyncIterator() {
  scan_sem = xSemaphoreCreateMutex();
  xSemaphoreTake(scan_sem, portMAX_DELAY);
}

void OTEnergyScanResultSyncIterator::set_error(otError e)
{
  data[write_current].error = e;

  write_current++;
}

void OTEnergyScanResultSyncIterator::receive(otEnergyScanResult* result)
{
  if(!result) {
    //Serial.println("backbuffer.error = OT_ERROR_GENERIC");
    data[write_current].error = OT_ERROR_GENERIC;
  } else {
    //Serial.println("backbuffer.error = OT_ERROR_NONE");
    data[write_current].error = OT_ERROR_NONE;
    data[write_current].result = *result;
  }

  write_current++;
}

bool OTEnergyScanResultSyncIterator::end() {
  return (data[current].error != 0);
}

OTEnergyScanResult OTEnergyScanResultSyncIterator::get() {
  if(!data[current].error) {
    return &data[current].result;
  } else {
    return nullptr;
  }
}

OTEnergyScanResult OTEnergyScanResultSyncIterator::next() {
  current++;
  if(current >= write_current) {
    if(!xSemaphoreTake(scan_sem, portMAX_DELAY) ) {
      data[current].error = OT_ERROR_GENERIC;
      return nullptr;
    }
  }

  return get();
}


OTJoinerContext::OTJoinerContext() {
  join_sem = xSemaphoreCreateMutex();
  xSemaphoreTake(join_sem, portMAX_DELAY);
}
