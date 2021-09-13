#pragma once

#include <AppEvent.h>

class MatterClass
{
public:
	int init();
	int begin();
	void postEvent(AppEvent * aEvent);
	QueueHandle_t sAppEventQueue;

};

extern MatterClass Matter;
