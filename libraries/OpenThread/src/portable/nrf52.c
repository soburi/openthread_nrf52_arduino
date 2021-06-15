/*
  Copyright (c) 2020-2021 Tokita, Hiroshi  All right reserved.

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

#include <openthread/platform/platform-softdevice.h>
#include <openthread/openthread-freertos.h>

#include <stdlib.h>
#include <string.h>

#include "nrf_assert.h"
#include "nrf_error.h"
#include "nrf_soc.h"
#include "nrf_sdh_soc.h"

#include "nrf_802154.h"
#include "nrf_802154_types.h"
#include "nrf_raal_softdevice.h"

void nrf5TransportInit(bool aPseudoReset) { (void)aPseudoReset; }
void nrf5TransportDeinit(bool aPseudoReset) { (void)aPseudoReset; }
void nrf5TransportProcess(void) { }
bool nrf5TransportPseudoResetRequired(void) { return false; }
otError otPlatUartFlush(void) { return OT_ERROR_NONE; }
otError otPlatUartEnable(void) { return OT_ERROR_NONE; }
otError otPlatUartDisable(void) { return OT_ERROR_NONE; }
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength) { (void)aBuf; (void)aBufLength; return OT_ERROR_NONE; }

/**@brief Multiprotocol configuration modes for the nRF IEEE 802.15.4 radio driver.
 */
typedef enum
{
    MULTIPROTOCOL_802154_MODE_FAST_SWITCHING_TIMES, /**< This mode allows for the fastest switching times between 802.15.4 and BLE. */
    MULTIPROTOCOL_802154_MODE_LOW_CPU_UTILIZATION   /**< This mode allows for the lower CPU utilization during the BLE timeslot. */
} multiprotocol_802154_mode_t;


uint32_t multiprotocol_802154_mode_set(multiprotocol_802154_mode_t mode)
{
    nrf_raal_softdevice_cfg_t cfg;

    // Check if we are in correct state.
    if (nrf_802154_state_get() != NRF_802154_STATE_SLEEP)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    memset(&cfg, 0, sizeof(cfg));

    cfg.timeslot_length      = NRF_RAAL_TIMESLOT_DEFAULT_LENGTH;
    cfg.timeslot_max_length  = NRF_RAAL_TIMESLOT_DEFAULT_MAX_LENGTH;
    cfg.timeslot_alloc_iters = NRF_RAAL_TIMESLOT_DEFAULT_ALLOC_ITERS;
    cfg.timeslot_safe_margin = NRF_RAAL_TIMESLOT_DEFAULT_SAFE_MARGIN;
    cfg.lf_clk_accuracy_ppm  = NRF_RAAL_DEFAULT_LF_CLK_ACCURACY_PPM;

    switch (mode)
    {
        case MULTIPROTOCOL_802154_MODE_FAST_SWITCHING_TIMES:
            cfg.timeslot_timeout = NRF_RAAL_TIMESLOT_DEFAULT_TIMEOUT;
            break;
        
        case MULTIPROTOCOL_802154_MODE_LOW_CPU_UTILIZATION:
            cfg.timeslot_timeout = NRF_RADIO_EARLIEST_TIMEOUT_MAX_US;
            break;

        default:
            ASSERT(false);
            break;
    }

    nrf_raal_softdevice_config(&cfg);

    return NRF_SUCCESS;
}

/**@brief Function for handling SOC events.
 *
 * @param[in]   sys_evt     SoC stack event.
 * @param[in]   p_context   Unused.
 */
static void soc_evt_handler(uint32_t sys_evt, void * p_context)
{
    UNUSED_PARAMETER(p_context);

    otSysSoftdeviceSocEvtHandler(sys_evt);
}

void otrUserInit()
{
#if defined(MULTIPROTOCOL_802154_CONFIG_PRESENT) && defined(MULTIPROTOCOL_802154_MODE)
    uint32_t retval = multiprotocol_802154_mode_set((multiprotocol_802154_mode_t)MULTIPROTOCOL_802154_MODE);
    ASSERT(retval == NRF_SUCCESS);
#endif

  // Register a handler for SOC events.
  NRF_SDH_SOC_OBSERVER(m_soc_observer, NRF_SDH_SOC_STACK_OBSERVER_PRIO, soc_evt_handler, NULL);
}

