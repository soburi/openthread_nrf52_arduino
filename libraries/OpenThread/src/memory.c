#include <stddef.h>
#include <string.h>
#include <openthread/platform/memory.h>
#include <openthread/openthread-freertos.h>

void* otPlatCAlloc(size_t n, size_t size)
{
  return memset(pvPortMalloc(n * size), 0, n * size);
}

void otPlatFree(void *p_ptr)
{
  vPortFree(p_ptr);
}

