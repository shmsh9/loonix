#include <network/ipv4.h>

void ipv4_debug_print(ipv4hdr *header){
    kprintf(
        "ipv4hdr {\n"
        "\tversion: %d hdr_len: %d ttl: %d\n"
        "\tlen: %d\n"
        "\tfrom: %d.%d.%d.%d to: %d.%d.%d.%d\n"
        "}\n",

        (uint64_t)header->version_ihl >> 4 & 0xf,
        (uint64_t)header->version_ihl & 0xf,
        (uint64_t)header->ttl,

        (uint64_t)(header->length >> 8 | (header->length & 0xff) << 8),

        (uint64_t)header->src_addr >> 0  & 0xff,
        (uint64_t)header->src_addr >> 8  & 0xff,
        (uint64_t)header->src_addr >> 16 & 0xff,
        (uint64_t)header->src_addr >> 24,

        (uint64_t)header->dst_addr >> 0  & 0xff,
        (uint64_t)header->dst_addr >> 8  & 0xff,
        (uint64_t)header->dst_addr >> 16 & 0xff,
        (uint64_t)header->dst_addr >> 24
    );
}
uint8_t *ipv4_extract_payload(uint8_t *ipv4packet, uint16_t *len){
    ipv4hdr hdr = *(ipv4hdr *)ipv4packet;
    switch (hdr.version_ihl & 0xf){
    case 5:
        *len = 20;
        return ipv4packet+20;
        break;
    
    default:
        KMESSAGE("unhandled packet size : %d", hdr.version_ihl & 0xf);
        *len = 0;
        return 0x0;
        break;
    }
}
