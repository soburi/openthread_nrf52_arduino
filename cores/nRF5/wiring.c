/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.

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

#include "Arduino.h"
#include "nrf.h"
#include "nrf_nvic.h"

#ifdef SOFTDEVICE_PRESENT
#include "nrf_sdm.h"
#include "nrf_sdh.h"
#include "nrf_sdh_freertos.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log.h"
#include "nrf_drv_clock.h"
#include "nrf_cc310_platform_abort.h"
#include "nrf_cc310_platform_mutex.h"
#include <mbedtls/platform.h>
#else
nrf_nvic_state_t nrf_nvic_state;
#endif

#define DFU_MAGIC_SERIAL_ONLY_RESET   0x4e
#define DFU_MAGIC_UF2_RESET           0x57
#define DFU_MAGIC_OTA_RESET           0xA8

// Must match temp register in bootloader
#define BOOTLOADER_VERSION_REGISTER     NRF_TIMER2->CC[0]
uint32_t bootloaderVersion = 0;


#define LOG_TASK_STACK_SIZE              ( 1024 / sizeof(StackType_t))          /**< FreeRTOS task stack size is determined in multiples of StackType_t. */

#if NRF_LOG_ENABLED
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */

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


static void log_init(void)
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

#if !OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS
static void* mbedtlsCAlloc(size_t n, size_t size)
{
    return memset(pvPortMalloc(n * size), 0, n * size);
}

static void mbedtlsFree(void *p_ptr)
{
    vPortFree(p_ptr);
}
#endif

void init( void )
{
  // Retrieve bootloader version
  bootloaderVersion = BOOTLOADER_VERSION_REGISTER;

#ifndef SOFTDEVICE_PRESENT
  // Select Clock Source : XTAL or RC
#if defined( USE_LFXO )
  // 32Khz XTAL
  NRF_CLOCK->LFCLKSRC = (uint32_t)((CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
#elif defined(USE_LFRC)
  // Internal OSC
  NRF_CLOCK->LFCLKSRC = (uint32_t)((CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
#else
  #error Clock Source is not configured, define USE_LFXO or USE_LFRC according to your board
#endif

  NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

  // RTC1 could be enabled by bootloader. Disable it
  NVIC_DisableIRQ(RTC1_IRQn);
  NRF_RTC1->EVTENCLR    = RTC_EVTEN_COMPARE0_Msk;
  NRF_RTC1->INTENCLR    = RTC_INTENSET_COMPARE0_Msk;
  NRF_RTC1->TASKS_STOP  = 1;
  NRF_RTC1->TASKS_CLEAR = 1;

  // Make sure all pin is set HIGH when pinmode() is called
  NRF_P0->OUTSET = UINT32_MAX;
#ifdef NRF_P1
  NRF_P1->OUTSET = UINT32_MAX;
#endif
#else /* SOFTDEVICE_PRESENT */

  ret_code_t err_code;

  log_init();

  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_sdh_freertos_init(NULL, NULL);

#ifdef MBEDTLS_THREADING
  freertos_mbedtls_mutex_init();
#endif

  // Do not start any interrupt that uses system functions before system initialisation.
  // The best solution is to start the OS before any other initalisation.

  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);


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

#endif /* SOFTDEVICE_PRESENT */
}

void enterUf2Dfu(void)
{
  NRF_POWER->GPREGRET = DFU_MAGIC_UF2_RESET;
  NVIC_SystemReset();
}

void enterSerialDfu(void)
{
  NRF_POWER->GPREGRET = DFU_MAGIC_SERIAL_ONLY_RESET;
  NVIC_SystemReset();
}

void enterOTADfu(void)
{
  NRF_POWER->GPREGRET = DFU_MAGIC_OTA_RESET;
  NVIC_SystemReset();
}

void waitForEvent(void)
{
#if 0
  // Set bit 7 and bits 4..0 in the mask to one (0x ...00 1001 1111)
  enum { FPU_EXCEPTION_MASK = 0x0000009F };

  /* Clear exceptions and PendingIRQ from the FPU unit */
  __set_FPSCR(__get_FPSCR()  & ~(FPU_EXCEPTION_MASK));
  (void) __get_FPSCR();
  NVIC_ClearPendingIRQ(FPU_IRQn);
#endif

  uint8_t sd_en = 0;
  (void) sd_softdevice_is_enabled(&sd_en);

  if ( sd_en )
  {
    (void) sd_app_evt_wait();
  }else
  {
    // SoftDevice is not enabled.
    __WFE();
    __SEV(); // Clear Event Register.
    __WFE();
  }
}


void systemOff(uint32_t pin, uint8_t wake_logic)
{
//  for(int i=0; i<8; i++)
//  {
//    NRF_POWER->RAM[i].POWERCLR = 0x03UL;
//  }

  pin = g_ADigitalPinMap[pin];

  if ( wake_logic )
  {
    nrf_gpio_cfg_sense_input(pin, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
  }else
  {
    nrf_gpio_cfg_sense_input(pin, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
  }

  uint8_t sd_en;
  (void) sd_softdevice_is_enabled(&sd_en);

  // Enter System OFF state
  if ( sd_en )
  {
    sd_power_system_off();
  }else
  {
    NRF_POWER->SYSTEMOFF = 1;
  }
}
