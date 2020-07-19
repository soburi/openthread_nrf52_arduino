/**
 * Copyright (c) 2017-2019 - 2020, Nordic Semiconductor ASA
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

#ifndef THREAD_COAP_UTILS_H__
#define THREAD_COAP_UTILS_H__

#include <stdbool.h>
#include <openthread/coap.h>

//#include "thread_utils.h"

/**@brief Thread CoAP utils configuration structure. */
typedef struct
{
    bool coap_server_enabled;                /**< Indicates if CoAP Server should be enabled. */
    bool coap_client_enabled;                /**< Indicates if CoAP Client should be enabled. */
    bool configurable_led_blinking_enabled;  /**< Indicates if LED blinking on Light commands should be configurable. */
} thread_coap_utils_configuration_t;


/**@brief Enumeration describing light commands. */
typedef enum
{
    THREAD_COAP_UTILS_LIGHT_CMD_OFF = '0',
    THREAD_COAP_UTILS_LIGHT_CMD_ON,
    THREAD_COAP_UTILS_LIGHT_CMD_TOGGLE
} thread_coap_utils_light_command_t;


/**@brief Type definition of the function used to handle light resource change.
 */
typedef void (*thread_coap_utils_light_command_handler_t)(thread_coap_utils_light_command_t light_state);


/**@brief Scope of multicast request. */
typedef enum
{
    THREAD_COAP_UTILS_MULTICAST_LINK_LOCAL,  /**< The Link Local multicast scope allows to send message to all neighbors. */
    THREAD_COAP_UTILS_MULTICAST_REALM_LOCAL, /**< The Realm Local multicast scope allows to send message to all non-sleepy
                                                  devices in the network. */
} thread_coap_utils_multicast_scope_t;

/***************************************************************************************************
 * @section CoAP utils core functions.
 **************************************************************************************************/

/**@brief Function for initializing the CoAP service with specified resources.
 *
 * @details The @p thread_init function needs to be executed before calling this function.
 *
 * @param[in] p_config A pointer to the Thread CoAP configuration structure.
 *
 */
void thread_coap_utils_init(const thread_coap_utils_configuration_t * p_config);


/**@brief Function for stopping the CoAP service. */
void thread_coap_utils_deinit(void);

/***************************************************************************************************
 * @section CoAP Server function proptypes.
 **************************************************************************************************/

/**@brief Function for enabling or disabling the provisioning.
 *
 * @param enabled Indicates if provisioning should be enabled.
 *
 */
void thread_coap_utils_provisioning_enable_set(bool enabled);


/**@brief Function for specifying custom light command handler.
 *
 * @param[in] handler Pointer to the function invoked when light resource is changed.
 *
 */
void thread_coap_utils_light_command_handler_set(thread_coap_utils_light_command_handler_t handler);


/**@brief Function for indicating if LED associated with light resource is blinking.
*
* @return  @true if LED associated with light resources is blinking.
*
*/
bool thread_coap_utils_light_is_led_blinking(void);

/***************************************************************************************************
 * @section CoAP client function proptypes.
 **************************************************************************************************/

/**@brief Function for sending the provisioning request to the CoAP server. */
void thread_coap_utils_provisioning_request_send(void);


/**@brief Function for removing stored peer address after provisioning process. */
void thread_coap_utils_peer_addr_clear(void);


/**@brief Function for sending the light request comamnd to the CoAP peer.
 *
 * @param[in] command  Light command.
 *
 */
void thread_coap_utils_unicast_light_request_send(uint8_t command);


/**@brief Function for sending the light request comamnd to the multicast IPv6 address.
 *
 * @param[in] command  Light command.
 * @param[in] scope    IPv6 multicast address scope.
 *
 */
void thread_coap_utils_multicast_light_request_send(uint8_t                             command,
                                                    thread_coap_utils_multicast_scope_t scope);

#endif /* THREAD_COAP_UTILS_H__ */
