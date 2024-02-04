#ifndef IPV4_H_
#define IPV4_H_
#include <network/ether.h>

#define IPV4_PROTO_ICMP 0x01
#define IPV4_PROTO_TCP  0x06
#define IPV4_PROTO_UDP  0x11

typedef struct __attribute__((__packed__)) _ipv4hdr{
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
}ipv4hdr;
void ipv4_debug_print(ipv4hdr *header);
void ipv4packet_debug_print(uint8_t *packet, uint16_t len);
uint8_t *ipv4_extract_payload(uint8_t *ipv4packet, uint16_t packetsize, uint16_t *len);
#endif
