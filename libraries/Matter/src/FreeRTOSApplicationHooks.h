#include <FreeRTOS.h>

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationIdleHook( void );

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

#ifdef __cplusplus
} // extern "C"
#endif
