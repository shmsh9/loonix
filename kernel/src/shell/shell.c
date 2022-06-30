#include <shell/shell.h>

int32_t shell(){
    kprint(SHELL_PROMPT);
    uint8_t c = 0;
    uint8_t cmdline[CMDLINE_MAX] = {0};
    int32_t cmdlinepos = 0;
    while(1){
        c = kgetchar();
        switch(c){
            case '\r':
                kputc('\n');
                shell_exec(cmdline);
                kprint(SHELL_PROMPT);
                memset(cmdline, 0, CMDLINE_MAX);
                cmdlinepos = 0;
                break;
            case '\n':
                kputc('\n');
                shell_exec(cmdline);
                memset(cmdline, 0, CMDLINE_MAX);
                cmdlinepos = 0;
                kprint(SHELL_PROMPT);
                break;
            //CTRL+C
            case 0x3:
                kprint("^C\n");
                memset(cmdline, 0, CMDLINE_MAX);
                cmdlinepos = 0;
                kprint(SHELL_PROMPT);
                break;
            //Backspace
            case 0x8:
                if( (cmdlinepos - 1) >= 0){
                    kputc('\b');
                    kputc(' ');
                    kputc('\b');
                    cmdline[cmdlinepos-1] = 0x0;
                    cmdlinepos--;
                }
                //kprint("\033[2A");
                break;
            //left arrow
            case 0x1b:
                if (cmdlinepos - 1 >= 0){
                    cmdlinepos -= 1;
                    kputc('\b');
                }
                break;
            default:
                if(cmdlinepos < CMDLINE_MAX - 1){
                    cmdline[cmdlinepos++] = c;
                }
                else{   
                    cmdlinepos = 0;
                    cmdline[cmdlinepos] = c;
                    cmdlinepos++;
                }
                if(c < 32)
                    kprintf("0x%x", c);
                else
                    kputc(c);
                break;
        }
    }
    return 0;
}

int32_t shell_exec(uint8_t cmdline[CMDLINE_MAX]){
    if(cmdline[0] == 0x0)
        return 0;
    kprintf("-"SHELL_NAME": %s: command not found\n", cmdline);
    return -1;
}
