#include <drivers/ps2.h>
//need to malloc this
ps2_key_pressed *ps2_current_key_pressed = 0;

//https://wiki.osdev.org/PS/2_Keyboard
static char ps2_scancode_pressed_set_1[PS2_SET_1_SIZE] = {
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
    '~',
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
static char ps2_scancode_pressed_set_1_shift[PS2_SET_1_SIZE] = {
    0x0, //(0x0)null
    0x1b,//(0x1)esc
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '_',
    '+',
    0x8,//(0x0E)backspace
    0xb,//(0x0E)tab
    'Q',
    'W',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    '{',
    '}',
    '\n', //enter
    '\033',//left-ctrl
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    ':',
    '"',
    '`',
    0x0, //LSHIFT
    '|',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',
    '<',
    '>',
    '?',
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
uint8_t ps2_device_lastchar_pressed = 0;
void ps2_device_irq_handler(){
    switch ((uintptr_t)ps2){
        case 0x0:
            break;
        default:{
            uint8_t c = 0;
            c = (uint8_t)inb(ps2->data_port);
            ps2_keypress_update(c);
            ps2_device_lastchar_pressed =  ps2_scancode_set_1_to_char(c);
            break;
        }
    }
} 

ps2_device *ps2_device_new(uintptr_t base_port){
    if(!base_port){
        KERROR("base_port 0x0");
        return 0x0;
    }
    ps2_current_key_pressed = kcalloc(PS2_SET_1_SIZE, sizeof(ps2_key_pressed));
    ps2_device *ret = kcalloc(sizeof(ps2_device), 1);
    *ret =  (ps2_device){
        .data_port = base_port,
        .status_register = base_port+4,
        .command_register = base_port+4
    };
    //disable 2 devices
    ps2_device_send_command(ret, 0xad);
    ps2_device_send_command(ret, 0xa7);
    //flush buffer
    inb(ret->data_port);
    //uint64_t config_byte = ps2_device_send_command(ret, 0x20);
    //KDEBUG("config_byte : 0b%b", config_byte);
    uint8_t result = ps2_device_send_command(ret, 0xaa);
    if(result != 0x55){
        //KDEBUG("ps/2 device responded 0x%x still trying", result);
        //kfree(ret);
        //return 0x0;
        //return (ps2_device){0};
    }
    //enable first port
    ps2_device_send_command(ret, 0xae);
    interrupt_handler_install(ps2_device_irq_handler, 33);
    return ret;
}
void ps2_device_set_bit(ps2_device *ps2, uint8_t bit){
    uint8_t reg = inb(ps2->status_register);
    set_bit(&reg, bit);
    outb(ps2->status_register, reg);
}
uint8_t ps2_device_get_bit(ps2_device *ps2, uint8_t bit){
    uint8_t reg = inb(ps2->status_register);
    return get_bit(reg, bit);
}
void ps2_device_unset_bit(ps2_device *ps2, uint8_t bit){
    uint8_t reg = inb(ps2->status_register);
    unset_bit(&reg, bit);
    outb(ps2->status_register, reg);
}
uint8_t ps2_device_send_command(ps2_device *ps2, uint8_t cmd){
    ps2_device_wait_tx(ps2);
    ps2_device_set_bit(ps2, PS2_DEVICE_COMMAND);
    outb(ps2->command_register, cmd);
    uint8_t ret = inb(ps2->data_port);
    ps2_device_end_tx_rx(ps2);
    return ret;
}
uint8_t ps2_device_send_subcommand(ps2_device *ps2, uint8_t cmd){
    ps2_device_wait_tx(ps2);

    ps2_device_set_bit(ps2, PS2_DEVICE_COMMAND);
    outb(ps2->data_port, cmd);
    ps2_device_unset_bit(ps2, PS2_DEVICE_COMMAND);

    ps2_device_wait_rx(ps2);
    uint8_t ret = inb(ps2->data_port);
    ps2_device_end_tx_rx(ps2);
    return ret;
}
void ps2_device_wait_tx(ps2_device *ps2){
    while(ps2_device_get_bit(ps2, PS2_DEVICE_INPUT)){}
}
void ps2_device_wait_rx(ps2_device *ps2){
    while(!ps2_device_get_bit(ps2, PS2_DEVICE_OUTPUT)){}
}
void ps2_device_end_tx_rx(ps2_device *ps2){
    //ps2_device_unset_bit(ps2, PS2_DEVICE_OUTPUT);
}
uint8_t ps2_device_get_scancode(ps2_device *ps2){
    switch ((uintptr_t)ps2){
        case 0x0:
            return 0x0;
            break;

        default:
            //ps2_device_send_command(ps2, 0x7); // send command 0x7 : first ps/2 port data
            ps2_device_wait_rx(ps2);
            uint8_t ret = (uint8_t)inb(ps2->data_port);
            ps2_keypress_update(ret);
            //clear buffer
            ps2_device_end_tx_rx(ps2);
            return ret;
            break;
    }
}
uint8_t ps2_device_getchar(ps2_device *ps2){
    return ps2_scancode_set_1_to_char(ps2_device_get_scancode(ps2));
}
uint8_t ps2_device_getchar_non_blocking(ps2_device *ps2){
    switch ((uintptr_t)ps2){
        case 0x0:
            return 0x0;
            break;

        default:
            if(ps2_device_lastchar_pressed){
                char ret = ps2_device_lastchar_pressed;
                ps2_device_lastchar_pressed = 0;
                return ret;
            }
            break;
    }
    return 0x0;
}
void ps2_keypress_update(uint8_t scancode){
    if(scancode == PS2_KEY_CAPS_LOCK){
        ps2_current_key_pressed[PS2_KEY_SHIFT_LEFT] = 1;
        return;
    }
    if(scancode == PS2_KEY_CAPS_LOCK+0x80){
        ps2_current_key_pressed[PS2_KEY_SHIFT_LEFT] = 0;
        return;
    }
    //released key code
    if(scancode >= 0x80){
        ps2_current_key_pressed[scancode-0x80] = 0;
        return;
    }
    //F12 pressed last supposed key of code 1
    if(scancode <= 0x58){
        ps2_current_key_pressed[scancode] = 1;
        return;
    }
    KERROR("scancode 0x%x not implemented", scancode);

}
uint8_t ps2_scancode_set_1_to_char(uint8_t scancode){
    if(scancode >= 0x81)
        return 0x0;
    
    //CTRL combinations
    if(ps2_current_key_pressed[PS2_KEY_CTRL_LEFT]){
        switch(scancode){
            case PS2_KEY_C:
                return 0x3;
                break;
            case PS2_KEY_M:
                return '\n';
            default:
                break;
        }
    }
    //LSHIFT && RSHIFT
    if( ps2_current_key_pressed[PS2_KEY_SHIFT_LEFT] || ps2_current_key_pressed[PS2_KEY_SHIFT_RIGHT])
        return ps2_scancode_pressed_set_1_shift[scancode];
    return ps2_scancode_pressed_set_1[scancode];
}

bool ps2_key_is_pressed(ps2_key_pressed k){
    if(!ps2_current_key_pressed)
        return false;
    if(k >= PS2_SET_1_SIZE){
        KERROR("k is out of range");
        return false;
    }
    return ps2_current_key_pressed[k];
}
