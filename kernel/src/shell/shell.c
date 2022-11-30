#include <shell/shell.h>

int shell(){
    builtins_init();

    kprint(SHELL_PROMPT);
    char c = 0;
    char cmdline[CMDLINE_MAX+1] = {0};
    int cmdlinepos = 0;
    while(1){
        c = kgetchar_non_blocking();
        switch(c){
            //no input at the time
            case 0x0:
                vt100_console_update_draw_screen(fb);
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
char shell_non_blocking_cmdline[CMDLINE_MAX+1] = {0};
int shell_non_blocking_cmdlinepos = 0;
bool shell_non_blocking_first_execution = true;
void shell_non_blocking(){
    if(shell_non_blocking_first_execution){
        builtins_init();
        kprint(SHELL_PROMPT);
        shell_non_blocking_first_execution = false;
    }
    char c = kgetchar_non_blocking();
    switch(c){
        //no input at the time
        case 0x0:
            break;
        case '\r':
            kputc('\n');
            shell_exec(shell_non_blocking_cmdline);
            memset(shell_non_blocking_cmdline, 0, CMDLINE_MAX);
            shell_non_blocking_cmdlinepos = 0;
            kprint(SHELL_PROMPT);
            break;
        case '\n':
            kputc('\n');
            shell_exec(shell_non_blocking_cmdline);
            memset(shell_non_blocking_cmdline, 0, CMDLINE_MAX);
            shell_non_blocking_cmdlinepos = 0;
            kprint(SHELL_PROMPT);
            break;
        //CTRL+C
        case 0x3:
            kprint("^C\n");
            memset(shell_non_blocking_cmdline, 0, CMDLINE_MAX);
            shell_non_blocking_cmdlinepos = 0;
            kprint(SHELL_PROMPT);
            break;
        //Backspace
        case 0x7f:
            if( (shell_non_blocking_cmdlinepos - 1) >= 0){
                shell_non_blocking_cmdlinepos--;
                rmchar(shell_non_blocking_cmdline, shell_non_blocking_cmdlinepos);
                refreshline(shell_non_blocking_cmdline, shell_non_blocking_cmdlinepos);
            }
            break;
        //Also Backspace
        case 0x8:
            if( (shell_non_blocking_cmdlinepos - 1) >= 0){
                shell_non_blocking_cmdlinepos--;
                rmchar(shell_non_blocking_cmdline, shell_non_blocking_cmdlinepos);
                refreshline(shell_non_blocking_cmdline, shell_non_blocking_cmdlinepos);
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
                        if( shell_non_blocking_cmdlinepos + 1 < CMDLINE_MAX && shell_non_blocking_cmdline[shell_non_blocking_cmdlinepos] != 0x0){
                            kprint("\033[1C");
                            shell_non_blocking_cmdlinepos++;
                        }
                        break;
                    //left arrow
                    case 0x44:
                        if( shell_non_blocking_cmdlinepos - 1 >= 0){
                            kprint("\033[1D");
                            shell_non_blocking_cmdlinepos--;
                        }
                        break;
                    //non handled char
                    default:
                        kprintf("0x%x\n", esc);
                        memset(shell_non_blocking_cmdline, 0, CMDLINE_MAX);
                        shell_non_blocking_cmdlinepos = 0;
                        kprint(SHELL_PROMPT);
                        break;
                }
            }
            break;
        default:
            if( (shell_non_blocking_cmdlinepos < CMDLINE_MAX - 1)){
                if(c != 0x0){
                    shell_non_blocking_cmdline[shell_non_blocking_cmdlinepos] = c;
                    shell_non_blocking_cmdlinepos++;
                    if(c < 32)
                        kprintf("0x%x", c);
                    else
                        kputc(c);
                }
            }
            else{ 
                kprintf("CMDLINE_MAX overflow : %d !\n", shell_non_blocking_cmdlinepos);
                memset(shell_non_blocking_cmdline, 0, CMDLINE_MAX);
                shell_non_blocking_cmdlinepos = 0;
                kprint(SHELL_PROMPT);
            }
            break;
    }
}
char **shell_parse_args2(char cmdline[CMDLINE_MAX], int *argc){
    *argc = 1;
	int l = strlen(cmdline);
	while(*cmdline == ' ')
		cmdline++;
	for(int i = 0; i < l; i++){
		if(cmdline[i] == ' '){
			for(int j = i; j < l; j++)
				if(cmdline[j] != ' '){
					*argc +=1;
					i = j;
					break;
				}
		}
	}
    char **argv = kcalloc(*argc, sizeof(char *));
	char *tmp = kcalloc(l, sizeof(char));
	int i = 0;
	int k = 0;
	int x = 0;
	for(; i < l; i++, k++){
		if(cmdline[i] == ' '){
			tmp[k] = '\0';
			k = 0;
			argv[x] = strdup(tmp);
			x++;
			for(int j = i; j < l; j++){
				if(cmdline[j] != ' '){
					i = j;
					break;
				}
			}
		}
		tmp[k] = cmdline[i];
	}
	if(*argc > 1){
		tmp[k] = '\0';
		argv[x] = strdup(tmp);
	}
	else{
		for(int h = 0; h < l; h++){
			if(cmdline[h] == ' ')
				cmdline[h] = '\0';
		}
		argv[0] = strdup(cmdline);
	}
	kfree(tmp);
	return argv;
}
int shell_exec_args_wrapped(shell_args_wrapped *args, task *t){
    return args->fn(args->argc, args->argv);
}
int shell_exec(char cmdline[CMDLINE_MAX]){
    if(cmdline[0] == 0x0)
        return 0;
    int argc = 0;
    char **argv = shell_parse_args2(cmdline, &argc);
    for(int i = 0; i < builtins.length; i++){
        if(strcmp(argv[0], builtins.builtins[i].name) == 0){
            shell_args_wrapped *argw = kmalloc(sizeof(shell_args_wrapped));
            *argw = (shell_args_wrapped){
                .argc = argc,
                .argv = argv,
                .fn = builtins.builtins[i].ptrfn
            };
            task *subproc = task_new(
                (int (*)(void *, task *))shell_exec_args_wrapped,
                (void *)argw,
                argv[0],
                task_priority_high
            );
            while(subproc->status != task_status_ended){
                
                int ctrl = kgetchar_non_blocking();
                //CTRL + C
                if(ctrl == 0x3){
                    task_end(subproc);
                    kprint("^C");
                }
                //sleep_100(50);
            }
            //int ret = builtins.builtins[i].ptrfn(argc, argv);
            for(int i = 0; i < argc; i++)
                kfree(argv[i]);
            kfree(argv);
            kfree(argw);
            return 0;
        }
    }
    kprintf("-"SHELL_NAME": %s: command not found\n", cmdline);
    for(int i = 0; i < argc; i++)
        kfree(argv[i]);
    kfree(argv);
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
