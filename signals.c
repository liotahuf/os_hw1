// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

#include "signals.h"
#include "commands.h"


//pid -1
//if pid !=-1 check if !stopped then enter to the list  
//fgCmd
//stped
//time
//update....

extern job jobs[MAX_JOBS_SIZE];


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
	printf("\n");
	if (L_Fg_Cmd != NULL) {
		if (kill(L_Fg_Cmd->pid, SIGINT) == -1)
		{
			perror("ctrl C did not work\n");
		}
		else
		{
			printf("%d job terminated  %s\n", L_Fg_Cmd->pid, L_Fg_Cmd->job_name);

			L_Fg_Cmd ;
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
	printf("\n");
	if (L_Fg_Cmd != NULL) {
		if (kill(L_Fg_Cmd->pid, SIGTSTP) == -1)
		{
			perror("ctrl Z did not work\n");
		}
		else
		{
			printf("%d job stopped  %s\n", L_Fg_Cmd->pid, L_Fg_Cmd->job_name);
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs[i] == NULL)
				{
					jobs[i] = L_Fg_Cmd;
					break;
				}
			}
			L_Fg_Cmd->stopped = true;
		}
	}
}

