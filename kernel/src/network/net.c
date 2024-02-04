#include <network/net.h>

void net_debug_print_packet(uint8_t *packet, uint16_t len){
    etherhdr eth = *(etherhdr *)packet;
    ether_debug_print(&eth);
    switch ((eth.ethertype << 8 | ((eth.ethertype >> 8) & 0xff))){
        case ETHER_TYPE_IPV4:{
            uint8_t *ippacket = packet+sizeof(etherhdr);
            ipv4hdr iphdr = *(ipv4hdr *)(ippacket);
            ipv4_debug_print(&iphdr);
            uint16_t l = 0;
            ippacket = ipv4_extract_payload(
                ippacket, 
                len-sizeof(etherhdr), 
                &l
            );
            ipv4packet_debug_print(ippacket, l);
            break;
        }

        default:
            break;
    }
    kprintf("\n");
}
