

#include <FreeRTOS.h>
#include <task.h>
#include <openthread/openthread-freertos.h>

#include <openthread-core-config.h>
#include <openthread-system.h>
#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/tasklet.h>

//#include <mbedtls/platform.h>


#include <openthread/heap.h>
#include <openthread/link.h>
#include <openthread/thread.h>
#include <openthread/joiner.h>
#include <platform-softdevice.h>

#include "thread_coap_utils.h"

#include "sdk_config.h"

#include "multiprotocol_802154_config.h"
#include "sdk_config.h"

#if !OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS
#include "nrf_cc310_platform_abort.h"
#include "nrf_cc310_platform_mutex.h"
#endif /* OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS */

#include <openthread/heap.h>
#include <openthread/link.h>
#include <openthread/thread.h>
#include <openthread/joiner.h>
#include <platform-softdevice.h>
#include "nrf_log.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh.h"
#include "app_error.h"
#include "app_util_platform.h"

#include "thread_coap_utils.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_freertos.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh.h"
#include "nrf_drv_clock.h"

#include <platform-softdevice.h>

#include <openthread/instance.h>
#include <openthread/thread.h>
#include <openthread/platform/platform-softdevice.h>

#include "tusb.h"
#include "nrf_drv_clock.h"
#include "nrfx_power.h"
#include "nrf_sdh.h"

#define LOG_TASK_STACK_SIZE              ( 1024 / sizeof(StackType_t))          /**< FreeRTOS task stack size is determined in multiples of StackType_t. */

#if NRF_LOG_ENABLED
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */
#endif

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
void vApplicationIdleHook( void )
{
#if NRF_LOG_ENABLED
    if (m_logger_thread)
    {
        vTaskResume(m_logger_thread);
    }
#endif
}

#if NRF_LOG_ENABLED
/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);

    while (1)
    {
        NRF_LOG_FLUSH();

        vTaskSuspend(NULL); // Suspend myself
    }
}
#endif // NRF_LOG_ENABLED


void thread_state_changed_callback(uint32_t flags, void * p_context)
{
    if (flags & OT_CHANGED_THREAD_ROLE)
    {
        switch(otThreadGetDeviceRole((otInstance*)p_context))
        {
            case OT_DEVICE_ROLE_CHILD:
            case OT_DEVICE_ROLE_ROUTER:
            case OT_DEVICE_ROLE_LEADER:
                break;

            case OT_DEVICE_ROLE_DISABLED:
            case OT_DEVICE_ROLE_DETACHED:
            default:
                thread_coap_utils_provisioning_enable_set(false);
                break;
        }
    }

    NRF_LOG_INFO("State changed! Flags: 0x%08x Current role: %d\r\n", flags, otThreadGetDeviceRole((otInstance*)p_context));
}


/**@brief Enumeration describing Thread radio receiver mode.
 *
 * @details THREAD_RADIO_MODE_RX_ON_WHEN_IDLE causes the device to keep its receiver on, when it is in idle state.
 *          Selecting THREAD_RADIO_MODE_RX_OFF_WHEN_IDLE results in creating the Thread Sleepy End Device.
 */
typedef enum
{
    THREAD_RADIO_MODE_RX_ON_WHEN_IDLE = 0, /**< Powered device */
    THREAD_RADIO_MODE_RX_OFF_WHEN_IDLE,    /**< Sleepy End Device */
} thread_radio_receiver_mode_t;


/**@brief Structure holding Thread configuration parameters. */
typedef struct
{
    thread_radio_receiver_mode_t radio_mode;              /**< Selected Thread radio receiver mode. */
    bool                         autostart_disable;       /**< Determines if node should attempt to join the
                                                               Thread network automatically. */
    bool                         autocommissioning;       /**< Determines if node should use pre-commissioned
                                                               data to join the Thread network. */
    uint32_t                     poll_period;             /**< Default SED poll period in miliseconds. */
    uint32_t                     default_child_timeout;   /**< Thread child timeout value in seconds. */
} thread_configuration_t;


#ifdef SOFTDEVICE_PRESENT
/**@brief Function for handling SOC events.
 *
 * @param[in]   sys_evt     SoC stack event.
 * @param[in]   p_context   Unused.
 */
void soc_evt_handler(uint32_t sys_evt, void * p_context)
{
    UNUSED_PARAMETER(p_context);

    otSysSoftdeviceSocEvtHandler(sys_evt);
}
#endif



void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
#if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", LOG_TASK_STACK_SIZE, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif
}

/**@brief Function for initializing the clock.
 */
void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}



void preproc() {
    log_init();
    clock_init();

    nrf_sdh_freertos_init(NULL, NULL);

#ifdef MBEDTLS_THREADING
    freertos_mbedtls_mutex_init();
#endif

    // Do not start any interrupt that uses system functions before system initialisation.
    // The best solution is to start the OS before any other initalisation.

    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Register a handler for SOC events.
    NRF_SDH_SOC_OBSERVER(m_soc_observer, NRF_SDH_SOC_STACK_OBSERVER_PRIO, soc_evt_handler, NULL);
}

void on_light_change_stub(thread_coap_utils_light_command_t command)
{
    switch (command)
    {
        case THREAD_COAP_UTILS_LIGHT_CMD_ON:
	    NRF_LOG_INFO("light_change:THREAD_COAP_UTILS_LIGHT_CMD_ON\n");
            break;

        case THREAD_COAP_UTILS_LIGHT_CMD_OFF:
	    NRF_LOG_INFO("light_change:THREAD_COAP_UTILS_LIGHT_CMD_OFF\n");
            break;

        case THREAD_COAP_UTILS_LIGHT_CMD_TOGGLE:
	    NRF_LOG_INFO("light_change:THREAD_COAP_UTILS_LIGHT_CMD_TOGGLE\n");
            break;

        default:
            ASSERT(false);
            break;
    }
}

void proc() {
///////////////////////////////////////////////////
    otError error;
    thread_configuration_t thread_configuration =
    {
        .radio_mode        = THREAD_RADIO_MODE_RX_ON_WHEN_IDLE,
	.autostart_disable = false,
        .autocommissioning = true,
    };
    thread_configuration_t* p_config = &thread_configuration;
    otInstance * mp_ot_instance = otrGetInstance();

    ASSERT(mp_ot_instance != NULL);

    NRF_LOG_INFO("Thread version   : %s", (uint32_t)otGetVersionString());
    NRF_LOG_INFO("Network name     : %s",
                 (uint32_t)otThreadGetNetworkName(mp_ot_instance));

    if (!otDatasetIsCommissioned(mp_ot_instance) && p_config->autocommissioning)
    {
        error = otLinkSetChannel(mp_ot_instance, THREAD_CHANNEL);
        ASSERT(error == OT_ERROR_NONE);

        error = otLinkSetPanId(mp_ot_instance, THREAD_PANID);
        ASSERT(error == OT_ERROR_NONE);
    }

    if (!p_config->autostart_disable)
    {
        otLinkModeConfig mode;
        memset(&mode, 0, sizeof(mode));

        if (p_config->radio_mode == THREAD_RADIO_MODE_RX_OFF_WHEN_IDLE)
        {
            mode.mRxOnWhenIdle       = false; // Join network as SED.
            mode.mSecureDataRequests = true;

            error = otLinkSetPollPeriod(mp_ot_instance, p_config->poll_period);
            ASSERT(error == OT_ERROR_NONE);
        }
        else
        {
            mode.mRxOnWhenIdle       = true;
            mode.mSecureDataRequests = true;
#ifdef OPENTHREAD_FTD
            mode.mDeviceType         = true;
            mode.mNetworkData        = true;
#endif
        }

        error = otThreadSetLinkMode(mp_ot_instance, mode);
        ASSERT(error == OT_ERROR_NONE);

        if (p_config->default_child_timeout != 0)
        {
            otThreadSetChildTimeout(mp_ot_instance, p_config->default_child_timeout);
        }

//error = otJoinerStart(mp_ot_instance, "J01NME", NULL, PACKAGE_NAME, "OPENTHREAD_ENABLE_PLATFORM_INFO",
//                      PACKAGE_VERSION, NULL, joiner_start_sync_callback, NULL);
//assert(error == OT_ERROR_NONE);

        error = otIp6SetEnabled(mp_ot_instance, true);
        ASSERT(error == OT_ERROR_NONE);

        if (otDatasetIsCommissioned(mp_ot_instance) || p_config->autocommissioning)
        {
            error = otThreadSetEnabled(mp_ot_instance, true);
            ASSERT(error == OT_ERROR_NONE);

            NRF_LOG_INFO("Thread interface has been enabled.");
            NRF_LOG_INFO("802.15.4 Channel : %d", otLinkGetChannel(mp_ot_instance));
            NRF_LOG_INFO("802.15.4 PAN ID  : 0x%04x", otLinkGetPanId(mp_ot_instance));
            NRF_LOG_INFO("Radio mode       : %s", otThreadGetLinkMode(mp_ot_instance).mRxOnWhenIdle ?
                                            "rx-on-when-idle" : "rx-off-when-idle");
        }
    }
///////////////////////////////////////////////////
  ASSERT(otrGetInstance() != NULL);

  /*otError*/ error = otSetStateChangedCallback(otrGetInstance(), thread_state_changed_callback, otrGetInstance());
  ASSERT(error == OT_ERROR_NONE);

  thread_coap_utils_configuration_t thread_coap_configuration =
  {
    .coap_server_enabled               = true,
    .coap_client_enabled               = false,
    .configurable_led_blinking_enabled = true,
  };

  thread_coap_utils_init(&thread_coap_configuration);
  thread_coap_utils_light_command_handler_set(on_light_change_stub);
}

static void *mbedtlsCAlloc(size_t n, size_t size)
{
    void *p_ptr = NULL;

    p_ptr = pvPortMalloc(n * size);

    memset(p_ptr, 0, n * size);

    return p_ptr;
}

static void mbedtlsFree(void *p_ptr)
{
    vPortFree(p_ptr);
}


void otrWrapSysInit()
{
  //preproc();
  //hrsInit();
#if !OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS
    int ret;

    ret = mbedtls_platform_set_calloc_free(mbedtlsCAlloc, mbedtlsFree);
    ASSERT(ret == 0);

#ifdef FREERTOS
    nrf_cc310_platform_abort_init();
    nrf_cc310_platform_mutex_init();
#endif

    ret = mbedtls_platform_setup(NULL);
    ASSERT(ret == 0);

#endif /* OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS */

#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
    otHeapSetCAllocFree(mbedtlsCAlloc, mbedtlsFree);
#endif /* OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE */

    //otrUartLockInit();
    otSysInit(0, NULL);

#if defined(MULTIPROTOCOL_802154_CONFIG_PRESENT) && defined(MULTIPROTOCOL_802154_MODE)
    uint32_t retval = multiprotocol_802154_mode_set((multiprotocol_802154_mode_t)MULTIPROTOCOL_802154_MODE);
    ASSERT(retval == NRF_SUCCESS);
#endif

}


otInstance * thread_ot_instance_get(void) { return otrGetInstance(); }

