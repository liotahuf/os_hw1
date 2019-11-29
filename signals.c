// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers


#include "signals.h"
#include "commands.h"

job* L_Fg_Cmd;


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
		if (kill(jobs.fg_job->pid, SIGINT) == -1)
		{
			perror("^C: ");
			jobs.fg_job = NULL;
		}
		else
		{
			printf("[%d]  Terminated  %s\n", jobs.fg_job->jobID, jobs.fg_job->name.c_str());
			jobs.fg_job->proc_state = BG;
			jobs.fg_job = NULL;
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

}

