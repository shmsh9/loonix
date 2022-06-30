#ifndef SHELL_H_
#define SHELL_H_
#include <kstd.h>

#define SHELL_NAME   "sh3w4x"
#define SHELL_PROMPT SHELL_NAME" $> "
#define CMDLINE_MAX  1024
int32_t shell();
int32_t shell_exec(uint8_t cmdline[CMDLINE_MAX]);
#endif
