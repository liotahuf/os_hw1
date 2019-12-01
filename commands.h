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
/*-------------added typedef----------*/
typedef struct History {
	char cmds[HIST_SIZE][MAX_LINE_SIZE];
	int oldest_cmd_idx;
	int newest_cmd_idx;
	bool full;

}history;

typedef struct Job {
	char job_name[MAX_JOB_NAME];
	int pid;
	long int entry_time;
	bool stopped;
} job;
/*-------------added globals----------*/
job fg_job;
job jobs[MAX_JOBS_SIZE];

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, job jobs[MAX_JOBS_SIZE]);
int ExeCmd(job jobs[MAX_JOBS_SIZE], char* lineSize, char* cmdString, history* hist);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
//added functions
void updateHistory(char *lineSize, history* hist);
void updateJobs(job jobs[MAX_JOBS_SIZE]);





#endif

