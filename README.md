
openthread_nrf52_arduino(ot-arduino)
======================================

original [README.md rename to BlueFruit_nRF52.md](BlueFruit_nRF52.md)

ot-arduino is an arduino package for nRF52840 that is integrated with the OpenThread subsystem.
This package provides API that can communicate by OpenThread protocol.

Resources
----------------

- Gitter: https://gitter.im/openthread_nrf52_arduino/community

Installation
--------------

Add https://soburi.github.io/openthread_nrf52_arduino/package_soburi_otnrf52_index.json as an 'Additional Board Manager URL' in Arduino property.
And install from Board Manager.

Preparation
-------------

Burn the bootloader at first from `[Tools] -> [Burn Bootloader]`
(Need J-Link adapter to burn bootloader.)


Libraries
---------------

### OpenThread library

OpenThread library provides two types of API.

#### CLI-command-like API

The  CLI-command-like API provides the same function as CLI commands.

Each API function is a similar name to the CLI command.
It can use to implement as arduino sketch the same command typed in CLI.

#### Low-Level API

Low-Level API is wrapped OpenThread API with exclusive execution.
That ensures API can call safely.


### Sockets Library

UdpSocket is available.
TcpSocket(Server/Client) planned to implements the after https://github.com/openthread/openthread/pull/6650 merged.

