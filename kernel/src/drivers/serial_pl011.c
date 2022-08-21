#include <drivers/serial_pl011.h>

serial_device *serial_pl011_device_new(){
    serial_device *ret = kcalloc(sizeof(serial_device), 1);
    *ret = (serial_device){
        .data = SERIAL_PL011_ADDRESS,
        .rx_tx = SERIAL_PL011_ADDRESS+SERIAL_PL011_RX_TX_OFFSET,
        .wait_rx = serial_pl011_device_wait_rx,
        .wait_tx = serial_pl011_device_wait_tx,
        .getchar_non_blocking = serial_pl011_device_getchar_non_blocking,
        .rx_mask = SERIAL_PL011_RX_MASK,
        .tx_mask = SERIAL_PL011_TX_MASK

    };
    if(serial_device_init(ret) != 0){
        kfree(ret);
        ret = 0x0;
    }
    return ret;

}

void serial_pl011_device_wait_rx(serial_device *serial){
    while((inb(serial->rx_tx) & serial->rx_mask)){}
}

void serial_pl011_device_wait_tx(serial_device *serial){
    while( (inb(serial->rx_tx)) == 0 ){}
}

uint8_t serial_pl011_device_getchar_non_blocking(serial_device *serial){
    return (inb(serial->rx_tx) & serial->rx_mask) == 0 ? (uint8_t)inb(serial->data) : 0;
}
