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
#ifndef MULTIPROTOCOL_CONFIG_H__
#define MULTIPROTOCOL_CONFIG_H__

#include <stdint.h>

/**@brief Multiprotocol configuration modes for the nRF IEEE 802.15.4 radio driver.
 */
typedef enum
{
    MULTIPROTOCOL_802154_MODE_FAST_SWITCHING_TIMES, /**< This mode allows for the fastest switching times between 802.15.4 and BLE. */
    MULTIPROTOCOL_802154_MODE_LOW_CPU_UTILIZATION   /**< This mode allows for the lower CPU utilization during the BLE timeslot. */
} multiprotocol_802154_mode_t;

/**@brief Function for configuring multiprotocol mode of the nRF IEEE 802.15.4 radio driver.
 *
 * @note This function should be called only if the following requirements are met:
 *         - nRF IEEE 802.15.4 radio driver is initialized by @p nrf_802154_init().
 *         - nRF IEEE 802.15.4 radio driver is in the sleep mode.
 *  
 * @param[in] mode Multiprotocol mode.
 *
 * @retval NRF_ERROR_INVALID_STATE  nRF IEEE 802.15.4 radio driver is in an incorrect state.
 * @retval NRF_ERROR_NONE           Multiprotocol configuration has been applied.
 *
 */
uint32_t multiprotocol_802154_mode_set(multiprotocol_802154_mode_t mode);

#endif /* MULTIPROTOCOL_CONFIG_H__ */
