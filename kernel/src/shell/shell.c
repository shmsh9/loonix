#include <shell/shell.h>

int shell(){
    char *prompt = "sh3w4x $> ";
    kprint(prompt);
    char c = 0;
    while(1){
        c = kgetchar();
        if(c == '\n'){
            kprintf("\n%s", prompt);
        }
        else{
            SERIAL_PUTCHAR(c);
        }
    }
    return 0;
}
