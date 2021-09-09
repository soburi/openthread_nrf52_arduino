#include <stdbool.h>
#include <stdint.h>

void nrf_log_frontend_std_0(uint32_t severity_mid, char const * const p_str)
{
}


void nrf_log_frontend_std_1(uint32_t            severity_mid,
                            char const * const p_str,
                            uint32_t           val0)
{
}

bool nrf_log_frontend_dequeue() { return false; }

void nrf_log_default_backends_init(void) { }

int m_nrf_log_app_logs_data_const;
typedef uint32_t (*nrf_log_timestamp_func_t)(void);
typedef uint32_t ret_code_t;
ret_code_t nrf_log_init(nrf_log_timestamp_func_t timestamp_func, uint32_t timestamp_freq) { return 0; }
