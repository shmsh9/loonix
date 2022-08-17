#include <drivers/ps2.h>
//https://wiki.osdev.org/PS/2_Keyboard
char ps2_scancode_pressed_set_1[0x58] = {
    0x0, //(0x0)null
    0x1b,//(0x1)esc
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    0x8,//(0x0E)backspace
    0xb,//(0x0E)tab
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n', //enter
    '\033',//left-ctrl
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    0x0, //LSHIFT
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    0x0, //RSHIFT
    '*',
    0x0, //left-alt
    ' ', //space
    0x0, //capslock
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};
char ps2_scancode_released_set_1[0x6d] = {0};

ps2_device ps2_device_new(uintptr_t base_port){
    ps2_device ret =  (ps2_device){
        .data_port = base_port,
        .status_register = base_port+4,
        .command_register = base_port+4
    };
    //disable both PS2 ports
    ps2_device_send_command(&ret, 0xAD);
    ps2_device_send_command(&ret, 0xA7);
    //flush buffer
    inb(ret.data_port);
    //self test
    uint8_t response = ps2_device_send_command(&ret, 0xAA);
    if(response != 0x55){
        KERROR("error initializing ps2 device !");
        return (ps2_device){0};
    }
    //enable both PS2 ports again
    ps2_device_send_command(&ret, 0xAE);
    //do not reenable mouse
    //ps2_device_send_command(&ret, 0xA8);
    //flush buffer
    outb(ret.data_port, 0);
    return ret;
}

uint8_t ps2_device_send_command(ps2_device *ps2, uint8_t cmd){
    outb(ps2->command_register, cmd);
    return inb(ps2->data_port);
}

uint8_t ps2_device_get_scancode(ps2_device *ps2){
    switch (ps2->data_port)
    {
    case 0x0:
        return 0x0;
        break;
    
    default:
        outb(ps2->command_register, 0x7); // send command 0x7 : first ps/2 port data
        while(inb(ps2->data_port) == 0xfa){
        }
        uint8_t ret = (uint8_t)inb(ps2->data_port);
        //clear buffer
        outb(ps2->data_port, 0);
        return ret;
        break;
    }
}
uint8_t ps2_device_getchar(ps2_device *ps2){
    return ps2_scancode_set_1_to_char(ps2_device_get_scancode(ps2));
}
uint8_t ps2_scancode_set_1_to_char(uint8_t scancode){
    //key release event
    if(scancode >= 0x58)
        return 0x0;
    return ps2_scancode_pressed_set_1[scancode];
}
