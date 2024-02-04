#ifndef NET_H_
#define NET_H_
#include <kstd/kstd.h>
#include <network/ether.h>
#include <network/ipv4.h>

void net_debug_print_packet(uint8_t *packet, uint16_t len);
#endif
