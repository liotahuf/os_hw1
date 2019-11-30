// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

#include "signals.h"
#include "commands.h"
#include <string.h>


//********************************************
// function name: signal_handler_func
// Description: calles sigaction function with the signal handler
// Parameters: signal, signal handler
// Returns: void
//********************************************
void signal_handler_func(int signal, signal_handler signal_handler)
{
	struct sigaction act;
	act.sa_handler = signal_handler;
	if (sigaction(signal, &act, NULL) == -1)
	{
		perror("There is an error in sigaction.\n");
		exit(1);
	}
}


//********************************************
// function name: handler_cntlc
// Description: handle the Control-C 
// Parameters: signal (the signal to handle)
// Returns: None
//**************************************************************************************
void handler_cntlc()
{
	if (fg_job != NULL) {
		printf("signal SIGINT was sent to pid %d\n", fg_job.pid);
		if (kill(fg_job.pid, SIGINT) == -1)
		{
			perror("ctrl C did not work\n");
			exit(1);
		}
		else
		{
			printf("%d job terminated  %s\n", fg_job.pid, fg_job.job_name);
			//change fg_job to no fg process
			fg_job.pid = -1;
			strcpy(fg_job.job_name, "\0");
			fg_job.entry_time = 0;
			fg_job.stopped = FALSE;
		}
	}
}

}

//********************************************
// function name: handler_cntlz
// Description: handle the Control-Z 
// Parameters: signal (the signal to handle)
// Returns: None
//**************************************************************************************
void handler_cntlz()
{
	if (fg_job != NULL) {
		printf("signal SIGTSTP was sent to pid %d\n", fg_job.pid);
		if (kill(fg_job.pid, SIGTSTP) == -1)
		{
			perror("ctrl Z did not work\n");
			exit(1);
		}
		else
		{
			printf("%d job stopped  %s\n", fg_job.pid, fg_job.job_name);
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs[i].pid == -1 && fg_job.stopped == false)
				{
					time_t seconds;
					seconds = time(NULL);
					long int curr_time = (long int)seconds;
					jobs[i].entry_time = curr_time;
					jobs[i].pid = fg_job.pid;
					strcpy(jobs[i].job_name, fg_job.job_name);
					fg_job.stopped = true;
					jobs[i].stopped = fg_job.stopped;
					break;
				}
			}
			//change fg_job to no fg process
			fg_job.pid = -1;
			strcpy(fg_job.job_name, "\0");
			fg_job.entry_time = 0;
			fg_job.stopped = FALSE;
		}
	}
}

