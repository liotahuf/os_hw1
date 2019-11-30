#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20


//added defines
#define HIST_SIZE 50
#define MAX_JOBS_SIZE 100
#define MAX_JOB_NAME 50

typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(void* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
//added functions
void updateHistory(char *lineSize, history* hist);
void updateJobs(job jobs[MAX_JOBS_SIZE]);


 typedef struct History {
	 char cmds[HIST_SIZE - 1][MAX_LINE_SIZE];
	 int oldest_cmd_idx;
	 int newest_cmd_idx;
	 bool full;

 }history;

 typedef struct Job {
	 char job_name [MAX_JOB_NAME];
	 int pid;
	 long int entry_time;
	 bool stopped;
 } job;

 int fgPid;
 char fgCmd[MAX_LINE_SIZE];

#endif

