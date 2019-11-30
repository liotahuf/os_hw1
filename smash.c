/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20


char* L_Fg_Cmd;

//This represents the list of jobs. Please change to a preferred type (array os structs,each struc with name,pid and time)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals

	/*---------initialize history zrray----------------*/
	history smashHist;
	for (int i = 0; i < HIST_SIZE; i++)
	{
		strcpy(smashHist.cmds[i] ,"\0");// all history commands are nothing
	}
	
	smashHist.oldest_cmd_idx = 0;
	smashHist.newest_cmd_idx = 0;
	smashHist.full = 0; //not fulled,just initialized it

	/*---------initialize jobs array----------------*/

	for (int i = 0; i < MAX_JOBS_SIZE; i++)
	{
		strcpy(jobs[i].job_name, "\0");//initialize jobs name so we know after that 
		jobs[i].pid = -1; //no real process can have pid negative in linux(according to google)
		jobs[i].entry_time = 0;
		jobs[i].stopped = 0;
	}
	/*---------initialize fg process----------------*/
	fg_job.pid = -1;
	strcpy(fg_job.job_name, "\0");
	fg_job.entry_time = 0;
	fg_job.stopped = FALSE;
	
	//calling signal_handler_func with cntlc and cntlz functions.
	signal_handler_func(SIGINT, &handler_cntlc);
	signal_handler_func(SIGTSTP, &handler_cntlz);
	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		//update cmds history
		updateHistory(lineSize,&smashHist);
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue; 
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString,&smashHist);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

