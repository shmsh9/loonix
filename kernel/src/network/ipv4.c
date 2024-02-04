#include <network/ipv4.h>

static char *ipv4_proto_name[0xff] = {
    [IPV4_PROTO_TCP]  = "TCP",
    [IPV4_PROTO_ICMP] = "ICMP",
    [IPV4_PROTO_UDP]  = "UDP"
};

void ipv4_debug_print(ipv4hdr *header){
    kprintf(
        "ipv4hdr {\n"
        "\tversion: %d hdr_len: %d ttl: %d\n"
        "\tlen: %d proto: %s\n"
        "\tfrom: %d.%d.%d.%d to: %d.%d.%d.%d\n"
        "}\n",

        (uint64_t)header->version_ihl >> 4 & 0xf,
        (uint64_t)header->version_ihl & 0xf,
        (uint64_t)header->ttl,

        (uint64_t)(header->length >> 8 | (header->length & 0xff) << 8),
        ipv4_proto_name[header->protocol] == 0x0 ? 
            "unknown" : ipv4_proto_name[header->protocol],

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
void ipv4packet_debug_print(uint8_t *packet, uint16_t len){
    kprintf("ipv4payload {\n");
    kprintf("\t");
    int j = 0;
    for(int i = 0; i < len; i++, j++){
        if(j == 8){
            kprintf("\n\t");
            j = 0;
        }
        if(packet[i] >= 0x10){
            kprintf("0x%x ", packet[i]);
        }
        else{
            kprintf("0x0%x ", packet[i]);

        }
    }
    kprintf("\n}\n"); 
}
uint8_t *ipv4_extract_payload(uint8_t *ipv4packet, uint16_t packetsize, uint16_t *len){
    ipv4hdr *hdr = (ipv4hdr *)ipv4packet;
    switch (hdr->version_ihl & 0xf){
    case 5:
        *len = packetsize-20;
        return ipv4packet+20;
        break;
    
    default:
        KMESSAGE("unhandled packet size : %d", hdr->version_ihl & 0xf);
        *len = 0;
        return 0x0;
        break;
    }
}
