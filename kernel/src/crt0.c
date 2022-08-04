#include <shell/builtins.h>
#include <mem.h>

void __init_glob(){
    uint32_t c = 0;
    SHELL_INIT_BUILTIN(c, clear, "clear");
    SHELL_INIT_BUILTIN(c, help, "help");
}
