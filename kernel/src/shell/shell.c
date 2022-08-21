#include <shell/shell.h>

int shell(){
    builtins_init();
    /* Why compiler does not do this for me ??? */
    /*
    builtins[0].ptrfn = clear;
    builtins[0].name = "clear";
    builtins[1].ptrfn = help;
    builtins[1].name = "help";
    */

    kprint(SHELL_PROMPT);
    char c = 0;
    char cmdline[CMDLINE_MAX+1] = {0};
    int cmdlinepos = 0;
    while(1){
        c = kgetchar_non_blocking();
        switch(c){
            //no input at the time
            case 0x0:
                vt100_console_update_draw_screen(&fb);
                break;
            case '\r':
                kputc('\n');
                shell_exec(cmdline);
                memset(cmdline, 0, CMDLINE_MAX);
                cmdlinepos = 0;
                kprint(SHELL_PROMPT);
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
            case 0x7f:
                if( (cmdlinepos - 1) >= 0){
                    cmdlinepos--;
                    rmchar(cmdline, cmdlinepos);
                    refreshline(cmdline, cmdlinepos);
                }
                break;
            //Also Backspace
            case 0x8:
                if( (cmdlinepos - 1) >= 0){
                    cmdlinepos--;
                    rmchar(cmdline, cmdlinepos);
                    refreshline(cmdline, cmdlinepos);
                }
                break;
            //Escape sequence
            case 0x1b:
                // literral [
                if(kgetchar() == 0x5b){
                    char esc = kgetchar();
                    switch(esc){
                        //right arrow
                        case 0x43:
                            if( cmdlinepos + 1 < CMDLINE_MAX && cmdline[cmdlinepos] != 0x0){
                                kprint("\033[1C");
                                cmdlinepos++;
                            }
                            break;
                        //left arrow
                        case 0x44:
                            if( cmdlinepos - 1 >= 0){
                                kprint("\033[1D");
                                cmdlinepos--;
                            }
                            break;
                        //non handled char
                        default:
                            kprintf("0x%x\n", esc);
                            memset(cmdline, 0, CMDLINE_MAX);
                            cmdlinepos = 0;
                            kprint(SHELL_PROMPT);
                            break;
                    }
                }
                break;
            default:
                if( (cmdlinepos < CMDLINE_MAX - 1)){
                    if(c != 0x0){
                        cmdline[cmdlinepos] = c;
                        cmdlinepos++;
                        if(c < 32)
                            kprintf("0x%x", c);
                        else
                            kputc(c);
                    }
                }
                else{ 
                    kprintf("CMDLINE_MAX overflow : %d !\n", cmdlinepos);
                    memset(cmdline, 0, CMDLINE_MAX);
                    cmdlinepos = 0;
                    kprint(SHELL_PROMPT);
                }
                break;
        }
    }
    return 0;
}
int shell_exec(char cmdline[CMDLINE_MAX]){
    if(cmdline[0] == 0x0)
        return 0;
    for(int i = 0; i < builtins.length; i++){
        if(strcmp(cmdline, builtins.builtins[i].name) == 0){
            return builtins.builtins[i].ptrfn(0, 0);
        }
    }
    kprintf("-"SHELL_NAME": %s: command not found\n", cmdline);
    return -1;
}
void rmchar(char cmdline[CMDLINE_MAX], int pos){
    int l = strlen(cmdline);
    for(int i = pos; i < l; i++){
        cmdline[i] = cmdline[i+1];
    }
}

void refreshline(char cmdline[CMDLINE_MAX], int cmdlinepos){
    if(cmdlinepos == 0){
        kprint("\033[1D \033[1D");
        return;
    }
    kprintf("\033[%dD%s \033[1D\033[%dD\033[%dC",
        (cmdlinepos+1), 
        cmdline,
        strlen(cmdline), 
        cmdlinepos
    );
    /*
    kprintf("\033[%dD", cmdlinepos+1);
    kprint(cmdline);
    kputc(' ');
    kputc('\b');
    kprintf("\033[%dD", strlen(cmdline));
    kprintf("\033[%dC", cmdlinepos);
    */
}
