#ifndef IPV4_H_
#define IPV4_H_
#include <network/ether.h>

#define IPV4_PROTO_ICMP 0x01
#define IPV4_PROTO_TCP  0x06
#define IPV4_PROTO_UDP  0x11

#define ICMP_TYPE_ECHO_REP 0x00
#define ICMP_TYPE_ECHO_REQ 0x08

#define U16LE(u16) ((u16 & 0xff) << 8 | u16 >> 8)

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

typedef struct __attribute__((__packed__)) _icmphdr{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t _rest;
}icmphdr;

typedef struct __attribute__((__packed__)) _tcphdr{
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint8_t  data_offset;
    uint8_t  ack_etc_flags;
    uint16_t window_sz;
    uint16_t checksum;
    uint16_t urgent_ptr;
    
}tcphdr;

typedef struct __attribute__((__packed__)) _udphdr{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
}udphdr;

void ipv4_debug_print(ipv4hdr *header);
void ipv4packet_debug_print(ipv4hdr *header, uint8_t *packet, uint16_t len);
uint8_t *ipv4_extract_payload(uint8_t *ipv4packet, uint16_t packetsize, uint16_t *len);
#endif
