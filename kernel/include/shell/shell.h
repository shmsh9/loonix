#ifndef SHELL_H_
#define SHELL_H_
#include <kstd.h>
#include <event_loop.h>
#include <shell/builtins.h>
#include <drivers/vt100.h>
#define SHELL_NAME   "sh3w4x"
#define SHELL_PROMPT SHELL_NAME" $> "
#define CMDLINE_MAX  1024
int shell();
void shell_non_blocking();
int shell_exec(char cmdline[CMDLINE_MAX]);
void rmchar(char cmdline[CMDLINE_MAX], int pos);
void refreshline(char cmdline[CMDLINE_MAX], int cmdlinepos);
#endif
