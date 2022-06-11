
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; 
    char buf[MAXLINE]; 
    int bg, state;
    pid_t pid;
    sigset_t mask_all, mask_one, prev_one;

    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGCHLD);

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL) /* ignore empty line */
        return;

    if (!builtin_cmd(argv)) {
        /* not a builtin command */
        sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
        if ((pid = fork()) < 0) {
            unix_error("fork error");
            exit(0);
        } else if (pid == 0) {
            sigprocmask(SIG_SETMASK, &prev_one, NULL);
            setpgid(0, 0);
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }

        state = bg? BG : FG;
        sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(jobs, pid, state, cmdline);
        sigprocmask(SIG_SETMASK, &prev_one, NULL);

        /* wait if run in foreground, print message otherwise */
        if (!bg) {
            waitfg(pid);
        }
        else {
            printf("[%d] (%d) %s", getjobpid(jobs, pid)->jid, pid, cmdline);
        }
    }
    return;
}