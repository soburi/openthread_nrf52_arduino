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

#include "otcmd.hpp"

#define CLS_PUB(n, x) class cmd_##n { public: x } n;
#define CLS_PUB_PRIV(n, x, y) class cmd_##n { public: x private: y } n;
#define CLS_DECL(x) x

#define CLSDS_PUB(n, x) class cmd_##n { otOperationalDataset& mDataset; public: x cmd_##n(otOperationalDataset& ds) : mDataset(ds) { } } n;
#define CLSDS_PUB_SUB(n, x, y) class cmd_##n { otOperationalDataset& mDataset; public: x cmd_##n(otOperationalDataset& ds) : mDataset(ds) y { } } n;
#define CLSDS_PUB_PRIV(n, x, y) class cmd_##n { otOperationalDataset& mDataset; public: x cmd_##n(otOperationalDataset& ds) : mDataset(ds) { } private: y } n;
#define CLSDS_INIT(n) ,n(ds)

class OpenThread_cmds {
public:
#if (OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2)
  CLS_PUB(bbr,
    otError operator()(otBackboneRouterConfig& a0) { return OTCMD::bbr(a0); }
#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
    CLS_PUB(enable, void operator()() const { return OTCMD::bbr_enable(); } );
    CLS_PUB(disable, void operator()() const { return OTCMD::bbr_disable(); } );
    CLS_PUB(config, otError operator()(uint8_t a0, uint16_t a1, uint32_t a2) { return OTCMD::bbr_config(a0, a1, a2); } );
    CLS_PUB(jitter,
      uint8_t operator()() const { return OTCMD::bbr_jitter(); }
      void operator()(uint8_t a0) { return OTCMD::bbr_jitter(a0); }
    );
    CLS_PUB(reg, otError operator()() const { return OTCMD::bbr_register(); } );
    CLS_PUB(state, otBackboneRouterState operator()() const { return OTCMD::bbr_state(); } );
#endif
  );
#endif

  /**
   * bufferinfo command
   */
  CLS_PUB(bufferinfo, void operator()(otBufferInfo* a0) { return OTCMD::bufferinfo(a0); });

  CLS_PUB(channel,
    /**
     * channel get channel
     */
    uint8_t operator()() const { return OTCMD::channel(); }
    otError operator()(uint8_t a0) { return OTCMD::channel(a0); }
    CLS_PUB(supported,       uint32_t operator()() const { return OTCMD::channel_supported(); });
    CLS_PUB(preferred,       uint32_t operator()() const { return OTCMD::channel_preferred(); });
#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE
    CLS_PUB(monitor,       otError operator()() const { return OTCMD::channel_monitor(); });
#endif
#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD
    CLS_PUB(manager,
      otError operator()() const { return OTCMD::channel_manager(); }
      CLS_PUB(favored,         otError operator()(uint32_t a0) { return OTCMD::channel_manager_favored(a0); });
    );
#endif
  );

#if OPENTHREAD_FTD
  CLS_PUB(child,     OTChildIterator operator()() { return OTCMD::child(); });
  CLS_PUB(childip,
    OTChildIpIterator operator()(uint16_t a0) const { return OTCMD::childip(a0); }
    CLS_PUB(max,
      uint8_t operator()() const { return OTCMD::childip_max(); }
#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
      void operator()(uint8_t a0) { return OTCMD::childip_max(a0); }
#endif
    );
  );
  CLS_PUB(childmax,
    uint8_t operator()() const { return OTCMD::childmax(); }
    otError operator()(uint8_t a0) { return OTCMD::childmax(a0); }
  );
  CLS_PUB(childtimeout,
    uint32_t operator()() const { return OTCMD::childtimeout(); }
    void operator()(uint32_t a0) { return OTCMD::childtimeout(a0); }
  );
#endif

#if OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
  CLS_PUB(coex,
    bool operator()() const { return OTCMD::coex(); }
    void operator()(bool a0) { return OTCMD::coex(a0); }
    CLS_PUB(metrics,       otError operator()(otRadioCoexMetrics& a0) { return OTCMD::coex_metrics(a0); });
  );
#endif

#if OPENTHREAD_CONFIG_COMMISSIONER_ENABLE && OPENTHREAD_FTD
  CLS_PUB(commissioner,
    otCommissionerState operator()() const { return OTCMD::commissioner(); }
    CLS_PUB(announce,       otError operator()(uint32_t a0, uint8_t a1, uint16_t a2, IPAddress& a3) { return OTCMD::commissioner_announce(a0, a1, a2, a3); });
    CLS_PUB(energy,       otError operator()(uint32_t a0, uint8_t a1, uint16_t a2, uint16_t a3, IPAddress& a4, otCommissionerEnergyReportCallback a5, void* a6) { return OTCMD::commissioner_energy(a0, a1, a2, a3, a4, a5, a6); });
    CLS_PUB(mgmtget,       otError operator()(otMeshcopTlvType* a0, uint8_t a1) { return OTCMD::commissioner_mgmtget(a0, a1); });
    CLS_PUB(mgmtset,       otError operator()(uint32_t a0, uint32_t a1, uint32_t a2, uint8_t* a3, uint8_t a4, otMeshcopTlvType* a5, uint8_t a6) { return OTCMD::commissioner_mgmtset(a0, a1, a2, a3, a4, a5, a6); });
    CLS_PUB(panid,       otError operator()(uint16_t a0, uint32_t a1, IPAddress& a2, otCommissionerPanIdConflictCallback a3, void* a4) { return OTCMD::commissioner_panid(a0, a1, a2, a3, a4); });
    CLS_PUB(provisioningurl,       otError operator()(const char* a0) { return OTCMD::commissioner_provisioningurl(a0); });
    CLS_PUB(sessionid,       uint16_t operator()() const { return OTCMD::commissioner_sessionid(); });
    CLS_PUB(start,       otError operator()(otCommissionerStateCallback a0, otCommissionerJoinerCallback a1, void* a2) { return OTCMD::commissioner_start(a0, a1, a2); });
    CLS_PUB(stop,       otError operator()() const { return OTCMD::commissioner_stop(); });
  );
#endif

#if OPENTHREAD_FTD
  CLS_PUB(contextreusedelay,
    void operator()(uint32_t a0) { return OTCMD::contextreusedelay(a0); }
    uint32_t operator()() const { return OTCMD::contextreusedelay(); }
  );
#endif

  CLS_PUB(counters,
    CLS_PUB(mac,
      OTMacCounters operator()() const { return OTCMD::counters_mac(); }
      CLS_PUB(reset,         void operator()() const { return OTCMD:: counters_mac_reset(); });
    );
    CLS_PUB(mle,
      OTMleCounters operator()() const { return OTCMD::counters_mle(); }
      CLS_PUB(reset,         void operator()() const { return OTCMD:: counters_mle_reset(); });
    );
  );

  class _dataset {
    otOperationalDataset mDataset;
  public:
	  CLSDS_PUB(active, otError operator()() { return OTCMD::dataset_active(mDataset); });

    CLSDS_PUB_SUB(activetimestamp,
      CLS_DECL(
        void operator()(uint64_t a0) { OTCMD::dataset_activetimestamp(mDataset, a0); }
        uint64_t operator()() { return OTCMD::dataset_activetimestamp(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_activetimestamp_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_activetimestamp_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB_SUB(channel,
      CLS_DECL(
        void operator()(uint16_t a0) { OTCMD::dataset_channel(mDataset, a0); }
        uint16_t operator()() { return OTCMD::dataset_channel(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_channel_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_channel_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB_SUB(channelmask,
      CLS_DECL(
        void operator()(uint32_t a0) { OTCMD::dataset_channelmask(mDataset, a0); }
        uint32_t operator()() { return OTCMD::dataset_channelmask(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_channelmask_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_channelmask_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB(clear,
      void operator()() { OTCMD::dataset_clear(mDataset); }
    );

    class cmd_commit {
    public:
      CLSDS_PUB(active, otError operator()() { return OTCMD::dataset_commit_active(mDataset); } );
      CLSDS_PUB(pending, otError operator()() { return OTCMD::dataset_commit_pending(mDataset); } );
      cmd_commit(otOperationalDataset& ds) : active(ds), pending(ds) {};
    } commit;

    CLSDS_PUB_SUB(delay,
      CLS_DECL(
        void operator()(uint32_t a0) { OTCMD::dataset_delay(mDataset, a0); }
        uint32_t operator()() { return OTCMD::dataset_delay(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_delay_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_delay_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB_SUB(extpanid,
      CLS_DECL(
        void operator()(const otExtendedPanId* a0) { OTCMD::dataset_extpanid(mDataset, a0); }
        OTExtendedPanId operator()() { return OTCMD::dataset_extpanid(mDataset); } 
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_extpanid_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_extpanid_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );

    class cmd_init {
    public:
        CLSDS_PUB(active, otError operator()() { return OTCMD::dataset_init_active(mDataset); } );
        CLSDS_PUB(pending, otError operator()() { return OTCMD::dataset_init_pending(mDataset); } );
#if OPENTHREAD_FTD
        CLSDS_PUB(create, otError operator()() { return OTCMD::dataset_init_new(mDataset); } );
        CLSDS_PUB(new_, otError operator()() { return OTCMD::dataset_init_new(mDataset); } );
#endif
      cmd_init(otOperationalDataset& ds) : active(ds)
                                         , pending(ds)
#if OPENTHREAD_FTD
                                         , create(ds)
                                         , new_(ds)
#endif
      {};
    } init;

    CLSDS_PUB_SUB(masterkey,
      CLS_DECL(
        void operator()(const otMasterKey* a0) { OTCMD::dataset_masterkey(mDataset, a0); }
        OTMasterKey operator()() { return OTCMD::dataset_masterkey(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_masterkey_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_masterkey_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB_SUB(meshlocalprefix,
      CLS_DECL(
        void operator()(const otMeshLocalPrefix* a0) { OTCMD::dataset_meshlocalprefix(mDataset, a0); }
        OTMeshLocalPrefix operator()() { return OTCMD::dataset_meshlocalprefix(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_meshlocalprefix_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_meshlocalprefix_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );

    CLSDS_PUB_SUB(networkname,
      CLS_DECL(
        void operator()(const char* a0) { OTCMD::dataset_networkname(mDataset, a0); }
        OTNetworkName operator()() { return OTCMD::dataset_networkname(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_networkname_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_networkname_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );

    CLSDS_PUB_SUB(panid,
      CLS_DECL(
        void operator()(const otPanId& a0) { OTCMD::dataset_panid(mDataset, a0); }
        otPanId operator()() { return OTCMD::dataset_panid(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_panid_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_panid_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );

	  CLSDS_PUB(pending, otError operator()() { return OTCMD::dataset_pending(mDataset); });

    CLSDS_PUB_SUB(pendingtimestamp,
      CLS_DECL(
        void operator()(uint64_t a0) { OTCMD::dataset_pendingtimestamp(mDataset, a0); }
        uint64_t operator()() { return OTCMD::dataset_pendingtimestamp(mDataset); }
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_pendingtimestamp_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_pendingtimestamp_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );
    CLSDS_PUB_SUB(securitypolicy,
      CLS_DECL(
        void operator()(uint16_t a0, uint32_t a1) { OTCMD::dataset_securitypolicy(mDataset, a0, a1); };
        OTSecurityPolicy operator()() { return OTCMD::dataset_securitypolicy(mDataset); };
        CLSDS_PUB(present,
          void operator()(bool a0) { OTCMD::dataset_securitypolicy_present(mDataset, a0); }
          bool operator()() { return OTCMD::dataset_securitypolicy_present(mDataset); }
        );
      ),
      CLS_DECL( CLSDS_INIT(present) )
    );

#if OPENTHREAD_FTD
    CLSDS_PUB(pskc,
      void operator()(const otPskc& a0) { OTCMD::dataset_pskc(mDataset, a0); }
      OTPskc operator()() { return OTCMD::dataset_pskc(mDataset); }
    );
#endif

    _dataset() : active(mDataset)
               , activetimestamp(mDataset)
               , channel(mDataset)
               , channelmask(mDataset)
               , clear(mDataset)
               , commit(mDataset)
               , delay(mDataset)
               , extpanid(mDataset)
               , init(mDataset)
               , masterkey(mDataset)
               , meshlocalprefix(mDataset)
               , networkname(mDataset)
               , panid(mDataset)
               , pending(mDataset)
               , pendingtimestamp(mDataset)
               , securitypolicy(mDataset)
#if OPENTHREAD_FTD
               , pskc(mDataset)
#endif
    {
    }

  } dataset;

#if OPENTHREAD_FTD
  CLS_PUB(delaytimermin,
    uint32_t operator()() const { return OTCMD::delaytimermin(); }
    otError operator()(uint32_t a0) { return OTCMD::delaytimermin(a0); }
  );
#endif

#if OPENTHREAD_CONFIG_DIAG_ENABLE
  CLS_PUB(diag,     otError operator()(uint8_t a0, char** a1, char* a2, size_t a3) { return OTCMD::diag(a0, a1, a2, a3); });
#endif

  CLS_PUB_PRIV(discover,
    CLS_DECL( OTActiveScanResultSyncIterator& operator()(uint32_t a0) { OTCMD::discover(a0, &context); return context; }; ),
  CLS_DECL( OTActiveScanResultSyncIterator context;)
  );

#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
  CLS_PUB(dns,
    CLS_PUB(resolve,
      otError operator()(const char* a0, const otIp6Address* a1, uint16_t a2, otDnsResponseHandler a3, void* a4) { return OTCMD::dns_resolve(a0, a1, a2, a3, a4); }
      otError operator()(const char* a0, otDnsResponseHandler a1, void* a2) { return OTCMD::dns_resolve(a0, a1, a2); }
    );
  );
#endif

#if OPENTHREAD_FTD
  CLS_PUB(eidcache,     OTEidCacheIterator operator()() { return OTCMD::eidcache(); });
#endif
  CLS_PUB(eui64,
    OTExtAddress operator()() const { return OTCMD::eui64(); }
  );

#if 0
  CLS_PUB(exit,     otError operator()(uint8_t a0, char** a1) { return OTCMD::exit(a0, a1); });
  CLS_PUB(logfilename,     void operator()(const char* a0) { return OTCMD::logfilename(a0); });
#endif

  CLS_PUB(extaddr,
    OTExtAddress operator()() const { return OTCMD::extaddr(); }
    otError operator()(OTExtAddress a0) { return OTCMD::extaddr(a0); }
  );

  CLS_PUB(extpanid,
    OTExtendedPanId operator()() const { return OTCMD::extpanid(); }
    otError operator()(OTExtendedPanId a0) { return OTCMD::extpanid(a0); }
  );

  CLS_PUB(factoryreset,     void operator()() const { return OTCMD::factoryreset(); });

  CLS_PUB(ifconfig,
    bool operator()() const { return OTCMD::ifconfig(); }
    CLS_PUB(up,       otError operator()() const { return OTCMD::ifconfig_up(); });
    CLS_PUB(down,       otError operator()() const { return OTCMD::ifconfig_down(); });
  );

  CLS_PUB(ipaddr,
    const OTIpaddrIterator operator()() const { return OTCMD::ipaddr(); }
    CLS_PUB(add,       otError operator()(const otIp6Address* a0, uint8_t a1, bool a2, bool a3, bool a4, uint32_t a5, bool a6) { return OTCMD::ipaddr_add(a0, a1, a2, a3, a4, a5, a6); });
    CLS_PUB(del,       otError operator()(const otIp6Address* a0) { return OTCMD::ipaddr_del(a0); });
    CLS_PUB(linklocal,       IPAddress operator()() const { return OTCMD::ipaddr_linklocal(); });
    CLS_PUB(mleid,       IPAddress operator()() const { return OTCMD::ipaddr_mleid(); });
    CLS_PUB(rloc,       IPAddress operator()() const { return OTCMD::ipaddr_rloc(); });
  );

  CLS_PUB(ipmaddr,
    const OTIpmaddrIterator operator()() const { return OTCMD::ipmaddr(); }
    CLS_PUB(add,       otError operator()(const otIp6Address* a0) { return OTCMD::ipmaddr_add(a0); });
    CLS_PUB(del,       otError operator()(const otIp6Address* a0) { return OTCMD::ipmaddr_del(a0); });
    CLS_PUB(promiscuous,
      bool operator()() const { return OTCMD::ipmaddr_promiscuous(); }
      void operator()(bool a0) { return OTCMD::ipmaddr_promiscuous(a0); }
    );
  );

#if OPENTHREAD_CONFIG_JOINER_ENABLE
  CLS_PUB(joiner,
    CLS_PUB(id,       OTExtAddress operator()() const { return OTCMD::joiner_id(); });
    CLS_PUB_PRIV(start,
    CLS_DECL(
      otError operator()(const char* a0, const char* a1=NULL) {
        OTCMD::joiner_start(a0, a1, &context);
        return context.error;
      }
    ),
    CLS_DECL( OTJoinerContext context; )
    );
    CLS_PUB(stop,       void operator()() const { return OTCMD::joiner_stop(); });
  );
#endif

#if OPENTHREAD_FTD
  CLS_PUB(joinerport,     uint16_t operator()() const { return OTCMD::joinerport(); });
#endif

  CLS_PUB(keysequence,
    CLS_PUB(counter,
      uint32_t operator()() const { return OTCMD::keysequence_counter(); }
      void operator()(uint32_t a0) { return OTCMD::keysequence_counter(a0); }
    );
    CLS_PUB(guardtime,
      uint32_t operator()() const { return OTCMD::keysequence_guardtime(); }
      void operator()(uint32_t a0) { return OTCMD:: keysequence_guardtime(a0); }
    );
  );
  CLS_PUB(leaderdata,     otError operator()(otLeaderData* a0) { return OTCMD::leaderdata(a0); });

#if OPENTHREAD_FTD
  CLS_PUB(leaderpartitionid,
    uint32_t operator()() const { return OTCMD::leaderpartitionid(); }
    void operator()(uint32_t a0) { return OTCMD::leaderpartitionid(a0); }
  );
  CLS_PUB(leaderweight,
    uint8_t operator()() const { return OTCMD::leaderweight(); }
    void operator()(uint8_t a0) { return OTCMD::leaderweight(a0); }
  );
#endif

  CLS_PUB(mac,
    CLS_PUB(retries,
      CLS_PUB(direct,
        uint8_t operator()() { return OTCMD::mac_retries_direct(); }
        void operator()(uint8_t a0) { return OTCMD::mac_retries_direct(a0); }
      );
#if OPENTHREAD_FTD
      CLS_PUB(indirect,
        uint8_t operator()() { return OTCMD::mac_retries_indirect(); }
        void operator()(uint8_t a0) { return OTCMD::mac_retries_indirect(a0); }
      );
#endif
    );
  );

  CLS_PUB(macfilter,
    const OTMacFilterIterator operator()() const { return OTCMD::macfilter(); }
    CLS_PUB(addr,
      const OTMacFilterAddrIterator operator()() const { return OTCMD::macfilter_addr(); }
      CLS_PUB(whitelist, void operator()() const { return OTCMD::macfilter_addr_whitelist(); });
      CLS_PUB(blacklist, void operator()() const { return OTCMD::macfilter_addr_blacklist(); });
      CLS_PUB(add,
        void operator()(const otExtAddress* a0, uint8_t a1) const { OTCMD::macfilter_addr_add(a0, a1); }
        void operator()(uint8_t a0) const { OTCMD::macfilter_addr_add(a0); }
      );
      CLS_PUB(remove, void operator()(const otExtAddress* a0) const { OTCMD::macfilter_addr_remove(a0); });
      CLS_PUB(clear, void operator()() const { OTCMD::macfilter_addr_clear(); });
    );
    CLS_PUB(rss,
      const OTMacFilterRssIterator operator()() const { return OTCMD::macfilter_rss(); }
      CLS_PUB(add,
        void operator()(const otExtAddress* a0, uint8_t a1) const { OTCMD::macfilter_rss_add(a0, a1); }
        void operator()(uint8_t a0) const { OTCMD::macfilter_rss_add(a0); }
        CLS_PUB(lqi,
          void operator()(const otExtAddress* a0, uint8_t a1) const { OTCMD::macfilter_rss_add_lqi(a0, a1); }
          void operator()(uint8_t a0) const { OTCMD::macfilter_rss_add_lqi(a0); }
        );
      );
      CLS_PUB(remove, otError operator()(const otExtAddress* a0=nullptr) { return OTCMD::macfilter_rss_remove(a0); });
      CLS_PUB(clear, void operator()() const { OTCMD::macfilter_rss_clear(); });
    );
  );

  CLS_PUB(masterkey,
    OTMasterKey operator()() const { return OTCMD::masterkey(); }
    otError operator()(OTMasterKey a0) { return OTCMD::masterkey(a0); }
  );

  CLS_PUB(mode,
    otLinkModeConfig operator()() const { return OTCMD::mode(); }
    otError operator()(otLinkModeConfig a0) { return OTCMD::mode(a0); }
  );

#if OPENTHREAD_FTD
  CLS_PUB(neighbor,     OTNeighborIterator operator()() { return OTCMD::neighbor(); });
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE || OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
  CLS_PUB(netdataregister,     otError operator()() const { return OTCMD::netdataregister(); });
#endif
#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
  CLS_PUB(netdatashow,     otError operator()(uint8_t* a0, uint8_t& a1) { return OTCMD::netdatashow(a0, a1); });
#endif

#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
  CLS_PUB(networkdiagnostic,
    CLS_PUB(get,       otError operator()(const otIp6Address* a0, uint8_t* a1, uint8_t a2) { return OTCMD::networkdiagnostic_get(a0, a1, a2); });
    CLS_PUB(reset,       otError operator()(const otIp6Address* a0, uint8_t* a1, uint8_t a2) { return OTCMD::networkdiagnostic_reset(a0, a1, a2); });
  );
#endif

#if OPENTHREAD_FTD
  CLS_PUB(networkidtimeout,
    void operator()(uint8_t a0) { return OTCMD::networkidtimeout(a0); }
    uint8_t operator()() const { return OTCMD::networkidtimeout(); }
  );
#endif

  CLS_PUB(networkname,
    const char* operator()() const { return OTCMD::networkname(); }
    otError operator()(const char* a0) { return OTCMD::networkname(a0); }
  );

#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
  CLS_PUB(networktime,
    otNetworkTimeStatus operator()() const { return OTCMD::networktime(); }
    otError operator()(uint16_t a0, uint16_t a1) { return OTCMD::networktime(a0, a1); }
  );
#endif

  CLS_PUB(panid,
    uint16_t operator()() const { return OTCMD::panid(); }
    otError operator()(uint16_t a0) { return OTCMD::panid(a0); }
  );
  CLS_PUB(parent,     otError operator()(otRouterInfo* a0) { return OTCMD::parent(a0); });
#if OPENTHREAD_FTD
  CLS_PUB(parentpriority,
    uint8_t operator()() const { return OTCMD::parentpriority(); }
    otError operator()(uint8_t a0) { return OTCMD::parentpriority(a0); }
  );
#endif

  CLS_PUB(ping,
    otError operator()(const otIp6Address* a0, const uint8_t* a1, uint16_t a2) { return OTCMD::ping(a0, a1, a2); }
    CLS_PUB(stop,       otError operator()() const { return OTCMD::ping_stop(); });
  );
  CLS_PUB(pollperiod,
    uint32_t operator()() const { return OTCMD::pollperiod(); }
    otError operator()(uint32_t a0) { return OTCMD::pollperiod(a0); }
  );

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
  CLS_PUB(prefix,
    const OTPrefixIterator operator()() const { return OTCMD::prefix(); }
    CLS_PUB(add,       otError operator()(const otIp6Prefix* a0, otRoutePreference a1, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7, bool a8) { return OTCMD::prefix_add(a0, a1, a2, a3, a4, a5, a6, a7, a8); });
    CLS_PUB(remove,       otError operator()(const otIp6Prefix* a0) { return OTCMD::prefix_remove(a0); });
  );
#endif

  CLS_PUB(promiscuous,
    bool operator()() const { return OTCMD::promiscuous(); }
    CLS_PUB(enable,       otError operator()(otLinkPcapCallback a0, void* a1) { return OTCMD::promiscuous_enable(a0, a1); });
    CLS_PUB(disable,       otError operator()() const { return OTCMD::promiscuous_disable(); });
  );

#if OPENTHREAD_FTD
  CLS_PUB(preferrouterid,     otError operator()(uint8_t a0) { return OTCMD::preferrouterid(a0); });
  CLS_PUB(pskc,
    OTPskc operator()() const { return OTCMD::pskc(); }
    otError operator()(const otPskc* a0) { return OTCMD::pskc(a0); }
    CLS_PUB(passphrase,       otError operator()(const char* a0) { return OTCMD::pskc_passphrase(a0); });
  );
  CLS_PUB(releaserouterid,     otError operator()(uint8_t a0) { return OTCMD::releaserouterid(a0); });
#endif

  CLS_PUB(reset, void operator()() const { return OTCMD::reset(); });
  CLS_PUB(rloc16, uint16_t operator()() const { return OTCMD::rloc16(); } );

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
  CLS_PUB(route,
    OTRouteIterator operator()() const { return OTCMD::route(); }
    CLS_PUB(add,
      otError operator()(const otExternalRouteConfig* a0) { return OTCMD::route_add(a0); }
      otError operator()(otRoutePreference a0, bool a1) { return OTCMD::route_add(a0, a1); }
    );
    CLS_PUB(remove,       otError operator()(const otIp6Prefix* a0) { return OTCMD::route_remove(a0); });
  );
#endif

#if OPENTHREAD_FTD
  CLS_PUB(router,     OTRouterIterator operator()() { return OTCMD::router(); });
  CLS_PUB(routerdowngradethreshold,
    uint8_t operator()() const { return OTCMD::routerdowngradethreshold(); }
    void operator()(uint8_t a0) { return OTCMD::routerdowngradethreshold(a0); }
  );
  CLS_PUB(routereligible,
    bool operator()() const { return OTCMD::routereligible(); }
    CLS_PUB(enable, void operator()() const { OTCMD::routereligible_enable(); });
    CLS_PUB(disable, void operator()() const { OTCMD::routereligible_disable(); });
  );
  CLS_PUB(routerselectionjitter,
    uint8_t operator()() const { return OTCMD::routerselectionjitter(); }
    void operator()(uint8_t a0) { return OTCMD::routerselectionjitter(a0); }
  );
  CLS_PUB(routerupgradethreshold,
    uint8_t operator()() const { return OTCMD::routerupgradethreshold(); }
    void operator()(uint8_t a0) { return OTCMD::routerupgradethreshold(a0); }
  );
#endif

  CLS_PUB_PRIV(scan,
    CLS_DECL(
      OTActiveScanResultSyncIterator& operator()(uint32_t a0, uint16_t a1) {
        OTCMD::scan(a0, a1, &context);
        return context;
      }
      CLS_PUB_PRIV(energy,
        CLS_DECL(
          OTEnergyScanResultSyncIterator& operator()(uint32_t a0, uint16_t a1) {
            OTCMD::scan_energy(a0, a1, &context);
            return context;
          }
        ),
        CLS_DECL(
          OTEnergyScanResultSyncIterator context;
        )
      );
    ),
    CLS_DECL(
      OTActiveScanResultSyncIterator context;
    )
  );

#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE
  CLS_PUB(service,
    CLS_PUB(add,
      otError operator()(uint32_t a0, uint8_t* a1, uint8_t a2, uint8_t* a3, uint8_t a4) { return OTCMD::service_add(a0, a1, a2, a3, a4); }
      otError operator()(uint32_t a0, const char* a1, const char* a2) { return OTCMD::service_add(a0, a1, a2); }
    );
    CLS_PUB(remove,
      otError operator()(uint32_t a0, uint8_t* a1, uint8_t a2) { return OTCMD::service_remove(a0, a1, a2); }
      otError operator()(uint32_t a0, const char* a1) { return OTCMD::service_remove(a0, a1); }
    );
  );
#endif

  CLS_PUB(singleton, bool operator()() const { return OTCMD::singleton(); });

#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE
  CLS_PUB(sntp,
    CLS_PUB(query, otError operator()(const otIp6Address* a0, uint16_t a1, otSntpResponseHandler a2, void* a3) { return OTCMD::sntp_query(a0, a1, a2, a3); });
  );
#endif

  CLS_PUB(state,
    otError operator()(otDeviceRole a0) { return OTCMD::state(a0); }
    otDeviceRole operator()() const { return OTCMD::state(); }
    CLS_PUB(child,       otError operator()() const { return OTCMD::state_child(); });
    CLS_PUB(detached,       otError operator()() const { return OTCMD::state_detached(); });
#if OPENTHREAD_FTD
    CLS_PUB(router,       otError operator()() const { return OTCMD::state_router(); });
    CLS_PUB(leader,       otError operator()() const { return OTCMD::state_leader(); });
#endif
  );

  CLS_PUB(thread,
    //bool operator()() const { return OTCMD::thread(); }
    CLS_PUB(start,       otError operator()() const { return OTCMD::thread_start(); });
    CLS_PUB(stop,       otError operator()() const { return OTCMD::thread_stop(); });
    CLS_PUB(version,       uint16_t operator()() const { return OTCMD::thread_version(); });
  );

  CLS_PUB(txpower,
    int8_t operator()() const { return OTCMD::txpower(); }
    otError operator()(int8_t a0) { return OTCMD::txpower(a0); }
  );
  CLS_PUB(version, const char* operator()() const { return OTCMD::version(); });

};


#undef CLS_PUB
#undef CLS_PUB_PRIV
#undef CLS_DECL

#undef CLSDS_PUB
#undef CLSDS_PUB_SUB
#undef CLSDS_PUB_PRIV
#undef CLSDS_INIT
