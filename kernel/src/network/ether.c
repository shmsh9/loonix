#include <network/ether.h>

void ether_debug_print(etherhdr *header){
    kprintf(
        "etherhdr {\n"
        "\tether_type: 0x%x\n"
        "\tfrom: %x:%x:%x:%x:%x:%x to: %x:%x:%x:%x:%x:%x\n"
        "}\n"
        ,
        (uint64_t)header->ethertype,

        (uint64_t)header->src_mac[0],
        (uint64_t)header->src_mac[1],
        (uint64_t)header->src_mac[2],
        (uint64_t)header->src_mac[3],
        (uint64_t)header->src_mac[4],
        (uint64_t)header->src_mac[5],

        (uint64_t)header->dst_mac[0],
        (uint64_t)header->dst_mac[1],
        (uint64_t)header->dst_mac[2],
        (uint64_t)header->dst_mac[3],
        (uint64_t)header->dst_mac[4],
        (uint64_t)header->dst_mac[5]
    );
}
