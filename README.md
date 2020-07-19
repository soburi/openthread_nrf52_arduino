
openthread_nrf52_arduino(ot-arduino)
======================================

original [README.md rename to BlueFruit_nRF52.md](BlueFruit_nRF52.md)

ot-arduino is a arduino package for nRF52840 that integrated with openthread subsystem.
This package provides API that is able to communicate by OpenThread protocol.

Resources
----------------

- Gitter: https://gitter.im/openthread_nrf52_arduino/community

Installation
--------------

Add https://soburi.github.io/openthread_nrf52_arduino/package_soburi_otnrf52_index.json as an 'Additional Board Manager URL' in Arduino property.
And install from Borad Manager.

Limitation
-------------

Need J-Link adapter to upload binary.  (Not implement DFU yet.)
This packages uses SoftDevice S140 v7.0.1.
Write `s140_nrf52_7.0.1_softdevice.hex` and generated .hex file manually,

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

