#ifndef SHELL_H_
#define SHELL_H_
#include <kstd/kstd.h>
#include <shell/builtins.h>
#include <drivers/vt100.h>
#define SHELL_NAME   "sh3w4x"
#define SHELL_PROMPT SHELL_NAME" $> "
#define CMDLINE_MAX  1024

typedef struct _shell_args_wrapped{
    int argc;
    char **argv;
    int (*fn)(int, char**);
}shell_args_wrapped;
int shell();
void shell_non_blocking();
int shell_exec(char cmdline[CMDLINE_MAX]);
void rmchar(char cmdline[CMDLINE_MAX], int pos);
void refreshline(char cmdline[CMDLINE_MAX], int cmdlinepos);
uint8_t shell_get_cursor_char();
void shell_set_exit_code(int e);
int shell_get_exit_code();
#endif
