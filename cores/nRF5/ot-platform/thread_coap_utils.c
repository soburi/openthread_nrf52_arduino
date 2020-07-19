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

#include "thread_coap_utils.h"

#include "app_timer.h"
#include "nrf_assert.h"
#include "nrf_log.h"
#include "sdk_config.h"
#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/thread.h>
#include <openthread/platform/alarm-milli.h>

#define RESPONSE_POLL_PERIOD     100
#define PROVISIONING_EXPIRY_TIME 5000
#define LED_INTERVAL             100

#define LEDS_OFF(leds_mask) do {  ASSERT(sizeof(leds_mask) == 4);                     \
                        NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
                        NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); } while (0)

#define LEDS_ON(leds_mask) do {  ASSERT(sizeof(leds_mask) == 4);                     \
                       NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
                       NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); } while (0)

#define LED_IS_ON(leds_mask) ((leds_mask) & (NRF_GPIO->OUT ^ LEDS_INV_MASK) )

#define LEDS_INVERT(leds_mask) do { uint32_t gpio_state = NRF_GPIO->OUT;      \
                              ASSERT(sizeof(leds_mask) == 4);                 \
                              NRF_GPIO->OUTSET = ((leds_mask) & ~gpio_state); \
                              NRF_GPIO->OUTCLR = ((leds_mask) & gpio_state); } while (0)

#define LEDS_CONFIGURE(leds_mask) do { uint32_t pin;                  \
                                  ASSERT(sizeof(leds_mask) == 4);     \
                                  for (pin = 0; pin < 32; pin++)      \
                                      if ( (leds_mask) & (1 << pin) ) \
                                          nrf_gpio_cfg_output(pin); } while (0)

extern otInstance* thread_ot_instance_get();

APP_TIMER_DEF(m_provisioning_timer);
APP_TIMER_DEF(m_led_timer);

/**@brief Structure holding CoAP status information. */
typedef struct
{
    bool         provisioning_enabled; /**< Information if provisioning is enabled. */
    uint32_t     provisioning_expiry;  /**< Provisioning timeout time. */
    bool         led_blinking_is_on;   /**< Information if leds are blinking */
    otIp6Address peer_address;         /**< An address of a related server node. */
} state_t;

static uint32_t                                  m_poll_period;
static state_t                                   m_state;
static thread_coap_utils_configuration_t         m_config;
static thread_coap_utils_light_command_handler_t m_light_command_handler;

/**@brief Forward declarations of CoAP resources handlers. */
static void provisioning_request_handler(void *, otMessage *, const otMessageInfo *);
static void light_request_handler(void *, otMessage *, const otMessageInfo *);

/**@brief Definition of CoAP resources for provisioning. */
static otCoapResource m_provisioning_resource =
{
    .mUriPath = "provisioning",
    .mHandler = provisioning_request_handler,
    .mContext = NULL,
    .mNext    = NULL
};

/**@brief Definition of CoAP resources for light. */
static otCoapResource m_light_resource =
{
    .mUriPath = "light",
    .mHandler = light_request_handler,
    .mContext = NULL,
    .mNext    = NULL
};

static void coap_default_handler(void                * p_context,
                                 otMessage           * p_message,
                                 const otMessageInfo * p_message_info)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_message);
    UNUSED_PARAMETER(p_message_info);

    NRF_LOG_INFO("Received CoAP message that does not match any request or resource\r\n");
}

static uint32_t poll_period_response_set(void)
{
    uint32_t     error;
    otError      ot_error;
    otInstance * p_instance = thread_ot_instance_get();

    do
    {
        if (otThreadGetLinkMode(p_instance).mRxOnWhenIdle)
        {
            error = NRF_ERROR_INVALID_STATE;
            break;
        }

        if (!m_poll_period)
        {
            m_poll_period = otLinkGetPollPeriod(p_instance);

            NRF_LOG_INFO("Poll Period: %dms set\r\n", RESPONSE_POLL_PERIOD);

            ot_error = otLinkSetPollPeriod(p_instance, RESPONSE_POLL_PERIOD);
            ASSERT(ot_error == OT_ERROR_NONE);

            error =  NRF_SUCCESS;
        }
        else
        {
            error = NRF_ERROR_BUSY;
        }
    } while (false);

    return error;
}

static void poll_period_restore(void)
{
    otError      error;
    otInstance * p_instance = thread_ot_instance_get();

    do
    {
        if (otThreadGetLinkMode(p_instance).mRxOnWhenIdle)
        {
            break;
        }

        if (m_poll_period)
        {
            error = otLinkSetPollPeriod(p_instance, m_poll_period);
            ASSERT(error == OT_ERROR_NONE);

            NRF_LOG_INFO("Poll Period: %dms restored\r\n", m_poll_period);
            m_poll_period = 0;
        }
    } while (false);
}

static void light_changed_default(thread_coap_utils_light_command_t light_command)
{
    switch (light_command)
    {
        case THREAD_COAP_UTILS_LIGHT_CMD_ON:
//            LEDS_ON(BSP_LED_3_MASK);
            break;

        case THREAD_COAP_UTILS_LIGHT_CMD_OFF:
//            LEDS_OFF(BSP_LED_3_MASK);
            break;

        case THREAD_COAP_UTILS_LIGHT_CMD_TOGGLE:
//            LEDS_INVERT(BSP_LED_3_MASK);
            break;

        default:
            break;
    }
}

static bool provisioning_is_enabled(void)
{
    return m_state.provisioning_enabled;
}

static void provisioning_enable(bool enable)
{
    uint32_t error;

    m_state.provisioning_enabled = enable;

    if (enable)
    {
        m_state.provisioning_expiry = otPlatAlarmMilliGetNow() + PROVISIONING_EXPIRY_TIME;
        error = app_timer_start(m_provisioning_timer,
                                APP_TIMER_TICKS(PROVISIONING_EXPIRY_TIME),
                                NULL);
        ASSERT(error == NRF_SUCCESS);

        error = app_timer_start(m_led_timer, APP_TIMER_TICKS(LED_INTERVAL), NULL);
        ASSERT(error == NRF_SUCCESS);

        if (m_config.configurable_led_blinking_enabled)
        {
            m_light_command_handler(THREAD_COAP_UTILS_LIGHT_CMD_ON);
        }
    }
    else
    {
        m_state.provisioning_expiry = 0;

        error = app_timer_stop(m_provisioning_timer);
        ASSERT(error == NRF_SUCCESS);

        error = app_timer_stop(m_led_timer);
        ASSERT(error == NRF_SUCCESS);

//        LEDS_OFF(BSP_LED_2_MASK);

        if (m_config.configurable_led_blinking_enabled)
        {
            error = app_timer_stop(m_led_timer);
            ASSERT(error == NRF_SUCCESS);

            if (m_state.led_blinking_is_on)
            {
                m_light_command_handler(THREAD_COAP_UTILS_LIGHT_CMD_ON);
            }
            else
            {
                m_light_command_handler(THREAD_COAP_UTILS_LIGHT_CMD_OFF);
            }
        }
    }
}

static void provisioning_timer_handler(void * p_context)
{
    provisioning_enable(false);
}

static otError provisioning_response_send(otMessage           * p_request_message,
                                          const otMessageInfo * p_message_info)
{
    otError        error = OT_ERROR_NO_BUFS;
    otMessage    * p_response;
    otInstance   * p_instance = thread_ot_instance_get();

    do
    {
        p_response = otCoapNewMessage(p_instance, NULL);
        if (p_response == NULL)
        {
            break;
        }

        otCoapMessageInit(p_response, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);

        error = otCoapMessageSetToken(p_response,
                                      otCoapMessageGetToken(p_request_message),
                                      otCoapMessageGetTokenLength(p_request_message));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapMessageSetPayloadMarker(p_response);
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otMessageAppend(
            p_response, otThreadGetMeshLocalEid(p_instance), sizeof(otIp6Address));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(p_instance, p_response, p_message_info);

    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

static void provisioning_request_handler(void                * p_context,
                                         otMessage           * p_message,
                                         const otMessageInfo * p_message_info)
{
    UNUSED_PARAMETER(p_message);

    otError       error;
    otMessageInfo message_info;

    if (!provisioning_is_enabled())
    {
        return;
    }

    if ((otCoapMessageGetType(p_message) == OT_COAP_TYPE_NON_CONFIRMABLE) &&
        (otCoapMessageGetCode(p_message) == OT_COAP_CODE_GET))
    {
        message_info = *p_message_info;
        memset(&message_info.mSockAddr, 0, sizeof(message_info.mSockAddr));

        error = provisioning_response_send(p_message, &message_info);
        if (error == OT_ERROR_NONE)
        {
            provisioning_enable(false);
        }
    }
}

static void led_timer_handler(void * p_context)
{
    // This handler may be called after app_timer_stop due to app_shceduler.
    if (m_state.provisioning_enabled)
    {
//        LEDS_INVERT(BSP_LED_2_MASK);
    }
}

static void light_response_send(otMessage           * p_request_message,
                                const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otMessage  * p_response;
    otInstance * p_instance = thread_ot_instance_get();

    do
    {
        p_response = otCoapNewMessage(p_instance, NULL);
        if (p_response == NULL)
        {
            break;
        }

        error = otCoapMessageInitResponse(p_response,
                                          p_request_message,
                                          OT_COAP_TYPE_ACKNOWLEDGMENT,
                                          OT_COAP_CODE_CHANGED);

        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(p_instance, p_response, p_message_info);

    } while (false);

    if ((error != OT_ERROR_NONE) && (p_response != NULL))
    {
        otMessageFree(p_response);
    }
}

static void light_request_handler(void                * p_context,
                                  otMessage           * p_message,
                                  const otMessageInfo * p_message_info)
{
    uint8_t command;

    do
    {
        if (otCoapMessageGetType(p_message) != OT_COAP_TYPE_CONFIRMABLE &&
            otCoapMessageGetType(p_message) != OT_COAP_TYPE_NON_CONFIRMABLE)
        {
            break;
        }

        if (otCoapMessageGetCode(p_message) != OT_COAP_CODE_PUT)
        {
            break;
        }

        if (otMessageRead(p_message, otMessageGetOffset(p_message), &command, 1) != 1)
        {
            NRF_LOG_INFO("light handler - missing command\r\n");
        }

        m_light_command_handler((thread_coap_utils_light_command_t)command);

        switch (command)
        {
            case THREAD_COAP_UTILS_LIGHT_CMD_ON:
                if (m_config.configurable_led_blinking_enabled)
                {
                    m_state.led_blinking_is_on = true;
                }
                break;

            case THREAD_COAP_UTILS_LIGHT_CMD_OFF:
                if (m_config.configurable_led_blinking_enabled)
                {
                    m_state.led_blinking_is_on = false;
                }
                break;

            case THREAD_COAP_UTILS_LIGHT_CMD_TOGGLE:
                if (m_config.configurable_led_blinking_enabled)
                {
                    m_state.led_blinking_is_on = !m_state.led_blinking_is_on;
                }
                break;

            default:
                break;
        }

        if (otCoapMessageGetType(p_message) == OT_COAP_TYPE_CONFIRMABLE)
        {
            light_response_send(p_message, p_message_info);
        }

    } while (false);
}

void thread_coap_utils_init(const thread_coap_utils_configuration_t * p_config)
{
    otInstance * p_instance = thread_ot_instance_get();

    otError error = otCoapStart(p_instance, OT_DEFAULT_COAP_PORT);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(p_instance, coap_default_handler, NULL);

    m_config = *p_config;

    m_light_command_handler = light_changed_default;

    if (m_config.coap_server_enabled)
    {
//        LEDS_CONFIGURE(LEDS_MASK);
//        LEDS_OFF(LEDS_MASK);

        uint32_t retval = app_timer_create(&m_led_timer,
                                           APP_TIMER_MODE_REPEATED,
                                           led_timer_handler);
        ASSERT(retval == NRF_SUCCESS);

        retval = app_timer_create(&m_provisioning_timer,
                                  APP_TIMER_MODE_SINGLE_SHOT,
                                  provisioning_timer_handler);
        ASSERT(retval == NRF_SUCCESS);

        m_light_resource.mContext        = p_instance;
        m_provisioning_resource.mContext = p_instance;

        error = otCoapAddResource(p_instance, &m_light_resource);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapAddResource(p_instance, &m_provisioning_resource);
        ASSERT(error == OT_ERROR_NONE);
    }
}

void thread_coap_utils_deinit(void)
{
    otInstance * p_instance = thread_ot_instance_get();

    otError error = otCoapStop(p_instance);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(p_instance, NULL, NULL);

    if (m_config.coap_server_enabled)
    {
        m_light_resource.mContext        = NULL;
        m_provisioning_resource.mContext = NULL;

        otCoapRemoveResource(p_instance, &m_light_resource);
        otCoapRemoveResource(p_instance, &m_provisioning_resource);
    }
}

void thread_coap_utils_peer_addr_clear(void)
{
    memset(&m_state.peer_address, 0, sizeof(m_state.peer_address));
}

void thread_coap_utils_unicast_light_request_send(uint8_t command)
{
    otError       error = OT_ERROR_NONE;
    otMessage   * p_request;
    otMessageInfo message_info;
    otInstance  * p_instance = thread_ot_instance_get();

    do
    {
        if (otIp6IsAddressUnspecified(&m_state.peer_address))
        {
            NRF_LOG_INFO("Failed to send the CoAP Request to the unspecified IPv6 address\r\n");
            break;
        }

        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapMessageGenerateToken(p_request, 2);

        error = otCoapMessageAppendUriPathOptions(p_request, "light");
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapMessageSetPayloadMarker(p_request);
        ASSERT(error == OT_ERROR_NONE);

        otCoapMessageInit(p_request, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapMessageGenerateToken(p_request, 2);
        UNUSED_VARIABLE(otCoapMessageAppendUriPathOptions(p_request, "light"));
        UNUSED_VARIABLE(otCoapMessageSetPayloadMarker(p_request));

        error = otMessageAppend(p_request, &command, sizeof(command));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mPeerPort = OT_DEFAULT_COAP_PORT;
        memcpy(&message_info.mPeerAddr, &m_state.peer_address, sizeof(message_info.mPeerAddr));

        error = otCoapSendRequest(p_instance,
                                  p_request,
                                  &message_info,
                                  NULL,
                                  p_instance);
    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(p_request);
    }
}

void thread_coap_utils_multicast_light_request_send(uint8_t                             command,
                                                    thread_coap_utils_multicast_scope_t scope)
{
    otError       error = OT_ERROR_NONE;
    otMessage   * p_request;
    otMessageInfo message_info;
    const char  * p_scope = NULL;
    otInstance  * p_instance = thread_ot_instance_get();

    do
    {
        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);

        error = otCoapMessageAppendUriPathOptions(p_request, "light");
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapMessageSetPayloadMarker(p_request);
        ASSERT(error == OT_ERROR_NONE);

        error = otMessageAppend(p_request, &command, sizeof(command));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        switch (scope)
        {
        case THREAD_COAP_UTILS_MULTICAST_LINK_LOCAL:
            p_scope = "ff02::1";
            break;

        case THREAD_COAP_UTILS_MULTICAST_REALM_LOCAL:
            p_scope = "ff03::1";
            break;

        default:
            ASSERT(false);
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mPeerPort = OT_DEFAULT_COAP_PORT;

        error = otIp6AddressFromString(p_scope, &message_info.mPeerAddr);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapSendRequest(p_instance, p_request, &message_info, NULL, NULL);
    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(p_request);
    }
}

static void provisioning_response_handler(void                * p_context,
                                          otMessage           * p_message,
                                          const otMessageInfo * p_message_info,
                                          otError               result)
{
    UNUSED_PARAMETER(p_context);

    // Restore the polling period back to initial slow value.
    poll_period_restore();

    if (result == OT_ERROR_NONE)
    {
        UNUSED_RETURN_VALUE(otMessageRead(p_message,
                                          otMessageGetOffset(p_message),
                                          &m_state.peer_address,
                                          sizeof(m_state.peer_address)));
    }
    else
    {
        NRF_LOG_INFO("Provisioning failed: %d\r\n", result);
    }
}

void thread_coap_utils_provisioning_request_send(void)
{
    otError       error = OT_ERROR_NO_BUFS;
    otMessage   * p_request;
    otMessageInfo message_info;
    otInstance  * p_instance = thread_ot_instance_get();

    do
    {
        p_request = otCoapNewMessage(p_instance, NULL);
        if (p_request == NULL)
        {
            break;
        }

        otCoapMessageInit(p_request, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapMessageGenerateToken(p_request, 2);

        error = otCoapMessageAppendUriPathOptions(p_request, "provisioning");
        ASSERT(error == OT_ERROR_NONE);

        // decrease the polling period for higher responsiveness
        uint32_t err_code = poll_period_response_set();
        if (err_code == NRF_ERROR_BUSY)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mPeerPort = OT_DEFAULT_COAP_PORT;

        error = otIp6AddressFromString("ff03::1", &message_info.mPeerAddr);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapSendRequest(
            p_instance, p_request, &message_info, provisioning_response_handler, p_instance);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

}

void thread_coap_utils_provisioning_enable_set(bool value)
{
    provisioning_enable(value);
}

void thread_coap_utils_light_command_handler_set(thread_coap_utils_light_command_handler_t handler)
{
    ASSERT(handler != NULL);

    m_light_command_handler = handler;
}

bool thread_coap_utils_light_is_led_blinking(void)
{
    return m_state.led_blinking_is_on;
}
