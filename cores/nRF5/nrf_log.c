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
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
}

bool ada_callback_invoke(const void* malloc_data, uint32_t malloc_len, const void* func, uint32_t arguments[], uint8_t argcount)
{
	return true;
}
