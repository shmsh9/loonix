#include <drivers/serial.h>

serial_device serial_device_new(){
    return (serial_device){
        .address = SERIAL_ADDRESS,
        .rx_mask = SERIAL_RX_MASK,
        .tx_mask = SERIAL_TX_MASK,
        .rx_tx_offset = SERIAL_RX_TX_OFFSET,
        .arch = ARCH_UINT
    };
}
uint8_t serial_device_readchar(serial_device *serial){
    serial_device_waittx(serial);
    uint8_t ret = (uint8_t)inb(serial->address);
    return ret;
}
void serial_device_waittx(serial_device *serial){
    switch(serial->arch){
        case ARCH_X64:
            while( (inb(serial->address+serial->rx_tx_offset) & serial->tx_mask) == 0){}
            break;
        case ARCH_AARCH64:
            while( (inb(serial->address+serial->rx_tx_offset) & serial->tx_mask) ){}
            break;
    }
}
void serial_device_waitrx(serial_device *serial){
    switch(serial->arch){
        case ARCH_X64:
            while( (inb(serial->address+serial->rx_tx_offset) & serial->rx_mask) == 0){}
            break;
        case ARCH_AARCH64:
            while(inb((serial->address+serial->rx_tx_offset) & serial->rx_mask)){}
            break;
    }
}
void serial_device_putchar(serial_device *serial, uint8_t c){
    serial_device_waittx(serial);
    outb(serial->address, (uint32_t)c);
}
