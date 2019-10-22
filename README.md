
openthread_nrf52_arduino(ot-arduino)
======================================

original [README.md rename to BlueFruit_nRF52.md](BlueFruit_nRF52.md)

ot-arduino is a arduino package for nRF52840 that integrated with openthread subsystem.
This package provides API that is able to communicate by OpenThread protocol.

Installation
--------------

Add https://soburi.github.io/openthread_nrf52_arduino/package_soburi_otnrf52_index.json as an 'Additional Board Manager URL' in Arduino property.
And install from Borad Manager.

Requirements
--------------

MSYS2 (make, cmake) C:\msys2
nrfjprog
J-link adapter

Limitation
-------------

Currently, Need J-Link adapter to upload binary.
(Not implement DFU yet.)
libtool seem to can't handle long path name.
Set build.path to short path name property in preferences.txt.
(It checked work correctly with "C:\ArduinoBuild".)

Libraries
---------------

### OpenThread library

OpenThread library provides two type of api.

#### High Level API

The High Level API provides a function with the same name as the CLI command.

Each CLI command implements as function that name is same as command.
Thus, it can use like a script on arduino code.

#### LowLevel API

Low Level API is a wrapper function of openthread api that enclosed otrLock() and otrUnlock().
That ensure API can execute without explicit exclusion.


### Sockets Library

arduino-esp32 has [Arduino style commnuication (WiFi) API](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src) that suitable for porting to this package because built on same lwip stack as ot-rtos.
This package provides Socket API that is ported from arduino-esp32's API.



Design
------------------

This package forks from [Adafruit's Arduino Core for Adafruit Bluefruit nRF52 Boards](https://github.com/adafruit/Adafruit_nRF52_Arduino).
And this package includes [ot-rtos](https://github.com/openthread/ot-rtos) that is executable openthread system integrated with FreeRTOS.
Adafruit's implementation also uses FreeRTOS.
So they can be integrated without much change.

Internally, openthread process runs as FreeRTOS's task in ot-rtos, also arduino main loop is run as FreeRTOS's task in Adafruit's implementation.

We only need to run concurrently these two tasks.
The one is openthread task and the other is arduino main loop tasks.

Happily, ot-rtos is well splitting application code, thus we can put arduino process into as application code.

Most part of arduino process is left as Adafruit's implementation.
It is contains creating arduino main loop task.

Calling vTaskStartScheduler() was replaced by otrStart() because otrStart() is internally calls vTaskStartScheduler().
openthread task is created just before task scheduler started in otrStart().
So two tasks runs concurrently by vTaskStartScheduler() calls.

Build process
--------

Arduino has unique build system that far from cmake which is used from ot-rtos.
This package is built by bit tricky way.
Glue tool (it executable named [intiki](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src), all "trick" is in it) gather command line arguments that is invoked from arduino-builder.
Tool generate CMakeList.txt after all build command is run through and run build process.


