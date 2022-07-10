#ifndef SHELL_H_
#define SHELL_H_
#include <kstd.h>
#include <shell/builtins.h>

#define SHELL_NAME   "sh3w4x"
#define SHELL_PROMPT SHELL_NAME" $> "
#define CMDLINE_MAX  1024
int shell();
int shell_exec(char cmdline[CMDLINE_MAX]);
void rmchar(char cmdline[CMDLINE_MAX], int pos);
void refreshline(char cmdline[CMDLINE_MAX], int cmdlinepos);
#endif
