#include <stdbool.h>
#include <stdint.h>
#include <openthread/error.h>

void nrf5TransportInit(bool aPseudoReset) { (void)aPseudoReset; }
void nrf5TransportDeinit(bool aPseudoReset) { (void)aPseudoReset; }
void nrf5TransportProcess(void) { }
bool nrf5TransportPseudoResetRequired(void) { return false; }
otError otPlatUartFlush(void) { return OT_ERROR_NONE; }
otError otPlatUartEnable(void) { return OT_ERROR_NONE; }
otError otPlatUartDisable(void) { return OT_ERROR_NONE; }
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength) { (void)aBuf; (void)aBufLength; return OT_ERROR_NONE; }
