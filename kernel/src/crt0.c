#include <shell/builtins.h>
#include <mem.h>

void __init_glob(){
    builtins.length = 0;
    SHELL_INIT_BUILTIN(clear, "clear");
    SHELL_INIT_BUILTIN(help, "help");
    SHELL_INIT_BUILTIN(useless, "1234567812345678");
}
