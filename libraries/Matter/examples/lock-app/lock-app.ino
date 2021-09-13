
#include <Matter.h>

#include "AppTask.h"

#include "AppEvent.h"

#include <FreeRTOS.h>

extern QueueHandle_t sAppEventQueue;



void setup() {
  // put your setup code here, to run once:
  Matter.begin();
}

void loop() {
    AppEvent event;

  // put your main code here, to run repeatedly:
  BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, pdMS_TO_TICKS(10));

  while (eventReceived == pdTRUE)
  {
      AppTask::sAppTask.DispatchEvent(&event);
      eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
  }

  AppTask::sAppTask.Process();

}
