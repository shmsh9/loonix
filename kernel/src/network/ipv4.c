#include <network/ipv4.h>

static char *ipv4_proto_name[0xff] = {
    [IPV4_PROTO_TCP ]  = "TCP",
    [IPV4_PROTO_ICMP]  = "ICMP",
    [IPV4_PROTO_UDP ]  = "UDP"
};

static char *icmp_type_name[0xff] = {
    [ICMP_TYPE_ECHO_REQ] = "echo_request",
    [ICMP_TYPE_ECHO_REP] = "echo_reply"
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

        (uint64_t)U16LE(header->length),
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
void ipv4packet_debug_print(ipv4hdr *header, uint8_t *packet, uint16_t len){
    switch (header->protocol){
        case IPV4_PROTO_ICMP:{
            icmphdr h = *(icmphdr *)packet;
            kprintf(
                "icmphdr {\n"
                "\ttype: %s code: %d"
                "\n}\n",
                icmp_type_name[h.type] == 0 ? 
                    "unknown" : icmp_type_name[h.type],
                (uint64_t)h.code
            );
            break;
        }
        case IPV4_PROTO_TCP:{
            tcphdr h = *(tcphdr *)packet;
            kprintf(
                "tcp {\n"
                "\tsrc_port: %d dst_port: %d\n\n\t",
                (uint64_t)U16LE(h.src_port),
                (uint64_t)U16LE(h.dst_port)
            );
            int j = 0;
            for(int i = (h.data_offset >> 4 & 0xf)*sizeof(uint32_t); i < len; i++, j++){
                if(packet[i] == '\n')
                    kprintf("%c\t", packet[i]);

                else
                    kprintf("%c", packet[i]);
                /*
                if(packet[i] >= 0x10)
                    kprintf("0x%x ", packet[i]);
                else
                    kprintf("0x0%x ", packet[i]);
                */
            }
            kprintf("\n}\n");
            break;
        }
        case IPV4_PROTO_UDP:{
            udphdr h = *(udphdr *)packet;
            kprintf(
                "udphdr {\n"
                "\tsrc_port: %d dst_port: %d"
                "\n}\n",
                (uint64_t)U16LE(h.src_port),
                (uint64_t)U16LE(h.dst_port)
            );
            break;
        }
        default:{
            /*
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
            */
            break;
        }
    }
    
}
uint8_t *ipv4_extract_payload(uint8_t *ipv4packet, uint16_t packetsize, uint16_t *len){
    ipv4hdr *hdr = (ipv4hdr *)ipv4packet;
    uint16_t hsz = (hdr->version_ihl & 0xf) * sizeof(uint32_t); //size is in 32bits count
    *len = packetsize-hsz;
    return ipv4packet+hsz;
}
