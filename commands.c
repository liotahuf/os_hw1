//		commands.c
//********************************************
#include "commands.h"


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(job jobs[MAX_JOBS_SIZE], char* lineSize, char* cmdString, history* hist)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
	
	static char* lastDir = NULL;
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		int chdir_error =0;
		if (num_arg != 1) // not enougth args for cd command
		{
			illegal_cmd = TRUE; 
		}
		else 
		{	char *buf =NULL;//so getdir mallocs the size of pwd
			if (args[1]=="-") // return to last dir
			{
				if (lastDir!= NULL)
				{
					chdir_error = chdir(lastDir);
					if(chdir_error==-1) // chdir returned an error
					{
						perror("chdir fail\n");
						return 1;
					}
					else //chdir successed,update lastDir
					{
						lastDir = getcwd(buf);
					}	return 0; //success
				}
				else {} // TODO

			}else //chdri to dir in arg[1]
			{
				chdir_error = chdir(args[1]);
				if(chdir_error==-1) // chdir returned an error
				{
					perror("smash error: > %s - path not found",args[1]);
					return 1;
				}
				else //chdir successed,update lastDir
				{
					lastDir = getcwd(buf);
				}	return 0; //success
			}
		}
			
			
		
	}
/*************************************************/
	else if (!strcmp(cmd, "history")) 
	{
  		if (num_arg != 0){
			illegal_cmd = TRUE;
		}
		else
		{
			int curr_cmd = hist->cmds[hist->oldest_cmd_idx];
			while(curr_cmd != hist->newest_cmd_idx-1)
			{
				if(hist->cmds[curr_cmd]!=NULL)
				{
					printf("%s",hist->cmds[curr_cmd]);
					
				}
				curr_cmd = (curr_cmd +1)% HIST_SIZE;
			}
		}
		
	}	
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if (num_arg != 0){
			illegal_cmd = TRUE;
		}
		else {
			if (getcwd(pwd,sizeof(pwd))!= NULL){
				printf("%s\n", pwd);
			}
			else {
				perror ("pwd failed\n");
			}
		}
	}
	
	/*************************************************/

	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
		if (num_arg != 0) {
			illegal_cmd = TRUE;
		}
		else
		{
			updateJobs(jobs);
			int i;
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs->pid = -1)
				{
					break;
				}
				//get elapsed time in seconds since the Epoch (00:00:00 UTC, January 1, 1970)
				time_t seconds;
				seconds = time(NULL);
				long int curr_time= (long int)seconds;
				//calculate time in jobs -> elapsed time - entry time(entry time is updated to each jobs entering the jobs list,and it is time in seconds from the Epoch (00:00:00 UTC, January 1, 1970) until the jobs enetered the list )
				
				long int time_in_job_list;
				time_in_job_list = curr_time - jobs->entry_time;
				if (jobs->stopped == 1)
				{
					printf("[%d] %s : %d %ld secs (Stopped)\n", i, jobs->job_name[i], jobs->pid, curr_time);
				}
				else
				{
					printf("[%d] %s : %d %ld secs\n", i, jobs->job_name[i], jobs->pid, curr_time);
				}
				
			}

		}


	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
			if (num_arg != 0) // showpid should have no addicional arguments
		{
			illegal_cmd = TRUE;
		}
		else //legal commmand
		{
			printf("smash pid is %d\n",getpid());
			return 0;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1:
				perror("fork error\n");
				return;
							
        	case 0 :
                	// Child Process
               		setpgrp();
					execv(cmdString, args);
					//execv only return in case of error,so if the code reached the next line an error ocurred
					perror("cannot execute command");
					return;
			
			default:
                	// in this case, this is the parent process,we need to update the fg process.
				// TBDwaitpid(pID);
					
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		return 0;
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, job jobs[MAX_JOBS_SIZE])
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (strlen(lineSize) < 2) // not long enougth to be bgcmd
	{
		return -1;
	}
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// parse comd
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
			return -1;
		args[0] = Command;
		for (int i = 1; i < MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters);
		}
		
		int pID;
		
		switch (pID = fork())
		{
		case -1:
			perror("fork error\n");
			return;

		case 0:
			// Child Process
			setpgrp();
			execv(Command, args);
			//execv only return in case of error,so if the code reached the next line an error ocurred
			perror("cannot execute command");
			return -1;

		default:
			// in this case, this is the parent process,we need to update the jobs list process.
			
			{
				int i;
				//find free idx in jobs array
				for (i = 0; i < MAX_JOBS_SIZE; i++)
				{
					if (jobs->pid == -1)
					{
						break;
					}
				}
				strcpy(jobs->job_name, Command);
				//get entry time in jobs list
				time_t seconds;
				seconds = time(NULL);
				long int curr_time = (long int)seconds;
				jobs->entry_time = curr_time;
				//job pid is the int pID because in case of father process, fork returns the child pid
				jobs->pid = pID;
				jobs->stopped = FALSE;//was not stoped by ctrl-Z
			}
			

		}
			   			
	}
	return -1;
}
//**************************************************************************************
// function name: updateHistory
// Description: update the history list of commands
// Parameters: command string, pointer to history(struc of array of cmds,idx to oldest comd and idx to newest cmd)
// Returns: no resturn(void)
//**************************************************************************************

void updateHistory(char *lineSize, history* hist)
{
	if (lineSize == NULL)//the command line is empty
	{
		return; 
	}
	
	strcpy(hist->cmds[hist->newest_cmd_idx], lineSize);

	hist->newest_cmd_idx = (hist->newest_cmd_idx + 1);
	if (hist->newest_cmd_idx == HIST_SIZE && hist->full == 0)//cmd list was not full and now we got the cmd n0 50 then it is now full
	{
		hist->full = 1;
		hist->newest_cmd_idx = hist->newest_cmd_idx % HIST_SIZE;
	}
	if ((hist->newest_cmd_idx == hist->oldest_cmd_idx+1)  &&(hist->full ==1)) //if newest command overflowed to low indexed,then need to update oldest idx
	{
		hist->oldest_cmd_idx =(hist->oldest_cmd_idx + 1)%HIST_SIZE;
	}
	
}

//**************************************************************************************
// function name: updateJobs
// Description: update the array of jobs wich are ruunig or suspended
// Parameters: command string, pointer history
// Returns: no resturn(void)
//**************************************************************************************

void updateJobs(job jobs[MAX_JOBS_SIZE])
{
	job tmp_jobs[MAX_JOBS_SIZE];//used to stored jobs from not updated jobs array whicht are not finished yet
	int i = 0;
	int j = 0;
	for (i=0;i<MAX_JOBS_SIZE;i++)
	{
		//if pid -1,cant do waitpid,but this means we are at the jobs array where there are any more real jobs,so exit the loop
		if (jobs[i].pid==-1)
		{
			break;
		}
		if (waitpid(jobs[i].pid, NULL, WNOHANG) == 0) //in this case, the job is not over! need to keep it in the array
		{
			//copy job filed to tmp jobs array
			strcpy(tmp_jobs[j].job_name, jobs[i].job_name);
			tmp_jobs[j].pid = jobs[i].pid;
			tmp_jobs[j].entry_time = jobs[i].entry_time;
			tmp_jobs[j].stopped = jobs[i].stopped;
			j++;
		}
	}
	//now,update the original jobs array
	for (i = 0; i < MAX_JOBS_SIZE; i++)
	{
		//places in the jobs array where there are still jobs
		if (i < j)
		{
			strcpy(jobs[i].job_name, tmp_jobs[j].job_name);
			jobs[i].pid = tmp_jobs[j].pid ;
			jobs[i].entry_time = tmp_jobs[j].entry_time ;
			jobs[i].stopped = tmp_jobs[j].stopped ;
			
		}
		else // initialize rest of jobs array to initial condition
		{
			strcpy(jobs[i].job_name,"\0");
			jobs[i].pid = -1;
			jobs[i].entry_time = 0;
			jobs[i].stopped = 0;
		}

	}

}