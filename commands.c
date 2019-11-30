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
	
	static char lastDir[MAX_LINE_SIZE];
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
		{	char buf [MAX_LINE_SIZE];
			if( (getcwd(buf,MAX_LINE_SIZE) )== NULL )
			{
				perror("getcwd error\n");
				return 1;
			}
			if (!strcmp(args[1],"-")) // return to last dir
			{
				if (lastDir!= NULL)
				{
					chdir_error = chdir(lastDir);
					if(chdir_error==-1) // chdir returned an error
					{
						perror("chdir error\n");
						return 1;
					}
					else //chdir successed,update lastDir
					{
						strcpy(lastDir,buf);
					}	return 0; //success
				}
				else
				{
					illegal_cmd = TRUE;
				}

			}else //chdri to dir in arg[1]
			{
				chdir_error = chdir(args[1]);
				if(chdir_error==-1) // chdir returned an error
				{
					//perror("smash error: > %s - path not found",args[1]);
					perror("smash error: > - path not found");
					return 1;
				}
				else //chdir successed,update lastDir
				{
					strcpy(lastDir,buf);
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
			int curr_cmd =hist->oldest_cmd_idx;
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
				if (jobs[i].pid == -1)
				{
					break;
				}
				//get elapsed time in seconds since the Epoch (00:00:00 UTC, January 1, 1970)
				time_t seconds;
				seconds = time(NULL);
				long int curr_time= (long int)seconds;
				//calculate time in jobs -> elapsed time - entry time(entry time is updated to each jobs entering the jobs list,and it is time in seconds from the Epoch (00:00:00 UTC, January 1, 1970) until the jobs enetered the list )
				
				long int time_in_job_list;
				time_in_job_list = curr_time - jobs[i].entry_time;
				if (jobs[i].stopped == 1)
				{
					printf("[%d] %s : %d %ld secs (Stopped)\n", i+1, jobs[i].job_name, jobs[i].pid, time_in_job_list);
				}
				else
				{
					printf("[%d] %s : %d %ld secs\n", i+1, jobs[i].job_name, jobs[i].pid, time_in_job_list);
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
		updateJobs(jobs);//first,update jobs list
		if (num_arg == 0) // fg last "backgrounded" process
		{
			int i;
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs[i].pid == -1)
				{
					break;
				}
			}
			if (i == 0) // if there are no real jobs at jobs list
			{
				illegal_cmd = TRUE;
			}
			else
			{
				i--;

				fg_job.pid = jobs[i].pid;
				strcpy(fg_job.job_name, jobs[i].job_name);
				fg_job.entry_time = jobs[i].entry_time;;
				fg_job.stopped = jobs[i].stopped;
				printf("%s\n", fg_job.job_name);
				if (jobs[i].stopped == 1) //suspeded process need to wake it up
				{
					int kill_error = kill(fg_job.pid, SIGCONT);
					if (kill_error == -1)
					{
						perror("kill error\n");
						return 1;
					}
				}
				printf("signal SIGCONT was sent to pid %d\n", fg_job.pid);
				int waitpid_error = (waitpid(fg_job.pid, NULL, WUNTRACED)); //wait until fg process is over or has been suspended
				if (waitpid_error == 0)//error in waitpid
				{
					perror("waitpid error\n");

				}
				//return fg process variable to "no fg" process state
				fg_job.pid = -1;
				strcpy(fg_job.job_name, "\0");
				fg_job.entry_time = 0;
				fg_job.stopped = FALSE;
				return 0;

			}
			

		}
		else //if (num_arg == 1)?? what to do when fg 1 3?
		{
			int i;
			sscanf(args[i], "%d", &i);//get number of the chose job to move to fg
						
			if (jobs[i].pid == -1)//make sure it is a legal cmd,i.e., the chosen process does exist in job list
			{
				illegal_cmd = TRUE;

			}
			else
			{

				fg_job.pid = jobs[i].pid;
				strcpy(fg_job.job_name, jobs[i].job_name);
				fg_job.entry_time = jobs[i].entry_time;;
				fg_job.stopped = jobs[i].stopped;
				printf("%s", fg_job.job_name);
				if (jobs[i].stopped == 1) //suspeded process need to wake it up
				{
					int kill_error = kill(fg_job.pid, SIGCONT);
					if (kill_error == -1)
					{
						perror("kill error\n");
						return 1;
					}
				}
				printf("signal SIGCONT was sent to pid %d\n", fg_job.pid);
				int waitpid_error = (waitpid(fg_job.pid, NULL, WUNTRACED)); //wait until fg process is over or has been suspended
				if (waitpid_error == 0)//error in waitpid
				{
					perror("waitpid error\n");

				}
				//return fg process variable to "no fg" process state
				fg_job.pid = -1;
				strcpy(fg_job.job_name, "\0");
				fg_job.entry_time = 0;
				fg_job.stopped = FALSE;
				return 0;
			}
			
		}
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		updateJobs(jobs);//first,update jobs list
		if (num_arg == 0) // bg last suspended process
		{
			int i;
			//find last process in jobs list
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs[i].pid == -1)//last jobs found
				{
					break;
				}
			}
			if (i == 0)//jobs list is empty
			{
				illegal_cmd = TRUE;
			}
			else
			{
				i--;//return to last real jobs in jobs list
				for (; i >= 0; i--)//search for last suspende job
				{
					if (jobs[i].stopped == 1)
					{
						break;
					}
				}
				if (i < 0)//no job in jobs list is a suspende job
				{
					illegal_cmd = TRUE;
				}
				else //last suspende job in jobs list found
				{
					int kill_error = kill(jobs[i].pid, SIGCONT);
					if (kill_error == -1)
					{
						perror("kill error\n");
						return 1;
					}
					printf("signal SIGCONT was sent to pid %d\n", jobs[i].pid);
					
				}
				
			}
		}
		else //if(num_args==1)?
		{
			int i;
			sscanf(args[i], "%d", &i);//get number of the chose job to move to fg

			if (jobs[i].pid == -1)//make sure it is a legal cmd,i.e., the chosen process does exist in job list
			{
				illegal_cmd = TRUE;

			}
			else
			{
				if (jobs[i].stopped == 1)
				{
					int kill_error = kill(jobs[i].pid, SIGCONT);
					if (kill_error == -1)
					{
						perror("kill error\n");
						return 1;
					}
					printf("signal SIGCONT was sent to pid %d\n", jobs[i].pid);

				}
			}

		}
			
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg == 0) // need to quit smash
		{
			exit(0);
		}
		else if (!strcmp(args[1],"kill") && (num_arg==1)) //exit every child still running
		{
			updateJobs(jobs);
			int i;
			for (i = 0; i < MAX_JOBS_SIZE; i++)
			{
				if (jobs[i].pid == -1)//found last job in jobs list
				{
					break;
				}
				int kill_error = kill(jobs[i].pid, SIGTERM);
				if (kill_error == -1)
				{
					perror("kill error\n");
					return 1;
				}
				printf("signal SIGTERM was sent to pid %d\n", jobs[i].pid);
				sleep(5);
				 
				if (waitpid(jobs[i].pid, NULL, WNOHANG) == 0) //in this case, the job is not over! need to send sigkill
				{
					int kill_error = kill(jobs[i].pid, SIGKILL);
					if (kill_error == -1)
					{
						perror("kill error\n");
						return 1;
					}
					printf("signal SIGKILL was sent to pid %d\n", jobs[i].pid);
					
				}

			}
			exit(0); // every jobs was killed,now exit smahs
		}
		else
		{
			illegal_cmd = TRUE;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
		if (num_arg != 2)
		{
			illegal_cmd = TRUE;
		}
		else
		{
			int rename_error = rename(args[1], args[2]);
			if (rename_error != 0)
			{
				perror("rename error\n");
				return 1;
			}
			else
			{
				printf("%s has been renamed to %s\n", args[1], args[2]);
			}
		}

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmd);
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
					execvp(cmdString, args);
					//execv only return in case of error,so if the code reached the next line an error ocurred
					perror("cannot execute command");
					exit(1);
			
			default:
                	// in this case, this is the parent process,we run the fg process.
				//update fg process in order to know who to suspend in case of ctrl-Z
				fg_job.pid = pID;
				strcpy(fg_job.job_name, cmdString);
				fg_job.entry_time = 0;
				fg_job.stopped = FALSE;
				int waitpid_error = (waitpid(pID,NULL, WUNTRACED)); //wait until fg process is over or has been suspended
				if (waitpid_error == 0)//error in waitpid
				{
					perror("waitpid error\n");
				
				}
				//return fg process variable to "no fg" process state
				fg_job.pid = -1;
				strcpy(fg_job.job_name, "\0");
				fg_job.entry_time = 0;
				fg_job.stopped = FALSE;
					
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
	//char ExtCmd[MAX_LINE_SIZE+2];
	//char *args[MAX_ARG];
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
	updateJobs(jobs);
	if (strlen(lineSize) < 2) // not long enougth to be bgcmd
	{
		return 1;
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
			return -1;

		case 0:
			// Child Process
			setpgrp();
			execvp(Command, args);
			//execv only return in case of error,so if the code reached the next line an error ocurred
			perror("cannot execute command");
			exit(1);

		default:
			// in this case, this is the parent process,we need to update the jobs list process.
			
			{
				int i;
				//find free idx in jobs array
				for (i = 0; i < MAX_JOBS_SIZE; i++)
				{
					if (jobs[i].pid == -1)
					{
						break;
					}
				}
				strcpy(jobs[i].job_name, Command);
				//get entry time in jobs list
				time_t seconds;
				seconds = time(NULL);
				long int curr_time = (long int)seconds;
				jobs[i].entry_time = curr_time;
				//job pid is the int pID because in case of father process, fork returns the child pid
				jobs[i].pid = pID;
				jobs[i].stopped = FALSE;//was not stoped by ctrl-Z
			}
			

		}
		return 0;
	}
	return 1;
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
			strcpy(jobs[i].job_name, tmp_jobs[i].job_name);
			jobs[i].pid = tmp_jobs[i].pid ;
			jobs[i].entry_time = tmp_jobs[i].entry_time ;
			jobs[i].stopped = tmp_jobs[i].stopped ;
			
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
