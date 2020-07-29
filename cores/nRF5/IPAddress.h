/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.
  Copyright (c) 2014-2018 Tokita, Hiroshi

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef IPAddress_h
#define IPAddress_h

#include <stdint.h>
#include <machine/endian.h>
#include <openthread/ip6.h>

#include "Printable.h"
#include "WString.h"

#define ENABLE_IPV6 1

#define COPY_V6ADDR(addr, u16addr) \
{ \
  uint16_t* ptr16= reinterpret_cast<uint16_t*>(addr); \
  for(int i=0; i<8; i++) { ptr16[i] = u16addr[i]; } \
}

#define V6Address_from_bytes(bytes) \
  IPAddress(IPAddress::ntohs(bytes[0]), \
            IPAddress::ntohs(bytes[2]), \
            IPAddress::ntohs(bytes[4]), \
            IPAddress::ntohs(bytes[6]), \
            IPAddress::ntohs(bytes[8]), \
            IPAddress::ntohs(bytes[10]), \
            IPAddress::ntohs(bytes[12]), \
            IPAddress::ntohs(bytes[14]) )

struct ot_in_addr {
  union {
    otIp6Address ot;
#if ENABLE_IPV6
    uint16_t u16[8];
    uint8_t u8[16];
#else
    uint16_t u16[2];
    uint8_t u8[4];
#endif
    struct {
#if ENABLE_IPV6
      uint8_t prefix[12];
#else
      uint8_t prefix[0];
#endif
      union {
        uint8_t bytes[4];
        uint32_t dword;
      };
    };
  };
};

static_assert(sizeof(struct ot_in_addr) == sizeof(otIp6Address), "Invalid ot_in_addr structure layout");

// A class to make it easier to handle and pass around IP addresses

class IPAddress : public Printable {
private:
    struct ot_in_addr _address;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address.bytes; };
    uint16_t* raw_address6() { return _address.u16; };
    bool isV6MappedAddress() const;

public:
    // Constructors
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);
#if defined(ENABLE_IPV6)
    IPAddress(uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4,
              uint16_t d5, uint16_t d6, uint16_t d7, uint16_t d8);
    IPAddress(const uint16_t *address);
#endif

    bool fromString(const char *address);
    bool fromString(const String &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t() const { return _address.dword; };
#if defined(ENABLE_IPV6)
    bool operator==(const IPAddress& addr) const { return ((*this) == addr._address.u16); };
#else
    bool operator==(const IPAddress& addr) const { return ((*this) == addr._address.u8); };
#endif
    bool operator==(const uint8_t* addr) const;
#if defined(ENABLE_IPV6)
    bool operator==(const uint16_t* addr) const;
#endif

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address.bytes[index]; };
    uint8_t& operator[](int index) { return _address.bytes[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator=(uint32_t address);
#if defined(ENABLE_IPV6)
    IPAddress& operator=(const IPAddress& addr);
    IPAddress& operator=(const uint16_t *address);
    operator const otIp6Address*() const { return &_address.ot; }
    operator otIp6Address*() { return &_address.ot; }
#endif

    virtual size_t printTo(Print& p) const;

    friend class EthernetClass;
    friend class UDP;
    friend class Client;
    friend class Server;
    friend class DhcpClass;
    friend class DNSClient;

    class V6RawAccessor : public Printable
    {
        uint16_t* addr;
        V6RawAccessor(uint16_t* a) : addr(a) { }
        uint16_t* raw_address() { return addr; }

        public:
        uint16_t operator[](int index) const { return IPAddress::htons(addr[index]); }
        uint16_t& operator[](int index) { return addr[index]; }
        virtual size_t printTo(Print& p) const;

        friend class IPAddress;
    };

    static inline uint16_t bswap16(uint16_t _x) {
        return (uint16_t)((_x >> 8) | ((_x << 8) & 0xff00));
    }
    
    static inline uint32_t bswap32(uint32_t _x) {
	return (uint32_t)(((_x >> 24) & 0x000000ffL) |
			  ((_x >>  8) & 0x0000ff00L) |
			  ((_x <<  8) & 0x00ff0000L) |
			  ((_x << 24) & 0xff000000L) );
    }
    
    static inline uint16_t htons(uint16_t _x) {
#if BYTE_ORDER == LITTLE_ENDIAN
        return bswap16(_x);
#else
	return _x;
#endif
    }

    static inline uint32_t htonl(uint32_t _x) {
#if BYTE_ORDER == LITTLE_ENDIAN
        return bswap32(_x);
#else
	return _x;
#endif
    }

    static inline uint16_t ntohs(uint16_t _x) { return htons(_x); }
    static inline uint32_t ntohl(uint32_t _x) { return htonl(_x); }

#if !defined(ENABLE_IPV6)
private:
#endif
    V6RawAccessor v6;
};

const IPAddress INADDR_NONE(0,0,0,0);

class INADDR {
public:
  static const IPAddress NONE;
  static const IPAddress ANY;
};

const INADDR INADDR;
#if ENABLE_IPV6
class IN6ADDR {
public:
  static const IPAddress ANY_INIT;
  static const IPAddress LOOPBACK_INIT;
  static const IPAddress LINKLOCAL_ALLNODES_INIT;
  static const IPAddress INKLOCAL_ALLROUTERS_INIT;
  static const IPAddress INTERFACELOCAL_ALLNODES_INIT;
  static const IPAddress INTERFACELOCAL_ALLROUTERS_INIT;
  static const IPAddress SITELOCAL_ALLROUTERS_INIT;
};

const IN6ADDR IN6ADDR;
#endif

#endif
