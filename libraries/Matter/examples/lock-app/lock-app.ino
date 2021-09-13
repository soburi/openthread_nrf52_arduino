
#include <Matter.h>

#include "AppTask.h"

#include "AppEvent.h"

#include <FreeRTOS.h>

#include "chipinit.h"

extern QueueHandle_t sAppEventQueue;



void setup() {
  // put your setup code here, to run once:
  Matter.init();
  ChipInit();
  AppTask::sAppTask.Init();
  Matter.begin();
}

void loop() {
    AppEvent event;

  // put your main code here, to run repeatedly:
  BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, pdMS_TO_TICKS(10));

  while (eventReceived == pdTRUE)
  {
      //AppTask::sAppTask.DispatchEvent(&event);
      event.Handler(&event);
      eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
  }

  AppTask::sAppTask.Process();

}
