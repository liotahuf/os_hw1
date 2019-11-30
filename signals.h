#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

typedef void(*signal_handler)(int signum);

void signal_handler_func(int signal, signal_handler signal_handler);
void handler_cntlc();
void handler_cntlz();




#endif

