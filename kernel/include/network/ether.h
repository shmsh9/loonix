#ifndef NETWORK_H_
#define NETWORK_H_
#include <kstd/kstd.h>

#define ETHER_TYPE_IPV4 0x0800
#define ETHER_TYPE_ARP  0x0806


typedef struct __attribute__((__packed__)) _etherhdr{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
}etherhdr;

void ether_debug_print(etherhdr *header);

#endif
