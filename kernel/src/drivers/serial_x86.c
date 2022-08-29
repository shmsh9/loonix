#include <drivers/serial_x86.h>
void serial_x86_device_remap_interrupts(){
    //remapping
    uint8_t PIC1 = 0x20;
    uint8_t PIC2 = 0xa0;
    outb(PIC1, 0x11); //restart PIC1
    outb(PIC2, 0x11); //restart PIC2
    outb(PIC1+1, 0x20); //PIC1 now starts at 32
    outb(PIC2+1, 0x28); //PIC2 now starts at 40
    outb(PIC1+1, 0x04); //setup cascading
    outb(PIC2+1, 0x04); //""
    outb(PIC1+1, 0x1);
    outb(PIC2+1, 0x01);
}
void serial_x86_interrupt_32(serial_device *serial){
    KMESSAGE("interrupt fired");
}

serial_device *serial_x86_device_new(){
    serial_device *ret = kcalloc(sizeof(serial_device), 1);
    *ret = (serial_device){
        .data = SERIAL_X86_ADDRESS,
        .rx_mask = SERIAL_X86_RX_MASK,
        .tx_mask = SERIAL_X86_TX_MASK,
        .rx_tx = SERIAL_X86_ADDRESS+SERIAL_X86_RX_TX_OFFSET,
        .getchar_non_blocking = serial_x86_device_getchar_non_blocking,
        .wait_rx = serial_x86_device_wait_rx,
        .wait_tx = serial_x86_device_wait_tx,
        .char_in = 0
    };
    if(serial_device_init(ret) != 0){
        kfree(ret);
        ret = 0x0;
    }
    interrupt_handler_install(serial_x86_interrupt_32, 32);
    serial_x86_device_remap_interrupts();
    uint64_t interrupt_reg = serial->data+1;
    //https://wiki.osdev.org/Serial_Ports#Interrupt_enable_register
    uint8_t interrupt_flags = 0;
    //set bit 0 to enable data available interrupt
    set_bit(&interrupt_flags, 0);
    outb(interrupt_reg, interrupt_flags);
    return ret;
}
uint8_t serial_x86_device_getchar_non_blocking(serial_device *serial){
    return (inb(serial->rx_tx) & serial->tx_mask) == 0 ? 0 : (uint8_t)inb(serial->data);
}
void serial_x86_device_wait_tx(serial_device *serial){
    while( (inb(serial->rx_tx) & serial->tx_mask) == 0){}
}
void serial_x86_device_wait_rx(serial_device *serial){
    while( (inb(serial->rx_tx) & serial->rx_mask) == 0){}
}
