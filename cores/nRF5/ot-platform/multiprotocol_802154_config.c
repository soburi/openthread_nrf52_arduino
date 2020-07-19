/**
 * Copyright (c) 2019 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "multiprotocol_802154_config.h"

#include <stdlib.h>
#include <string.h>

#include "multiprotocol_802154_config.h"

#include "nrf_assert.h"
#include "nrf_error.h"
#include "nrf_soc.h"

#include "nrf_802154.h"
#include "nrf_802154_types.h"
#include "nrf_raal_softdevice.h"

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
