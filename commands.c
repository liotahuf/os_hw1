//		commands.c
//********************************************
#include "commands.h"

struct History{
	char* cmds [HIST_SIZE-1];
	int oldes_cmd_idx;
	int newest_cmd_idx;

}
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString, history* hist)
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
				else // TODO

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
	}
/*************************************************/
	else if (!strcmp(cmd, "history")) 
	{
  		if (num_arg != 0){
			illegal_cmd = TRUE;
		}
		else
		{
			int curr_cmd = hist->cmds[hist->oldes_cmd]
			while(curr_cmd != hist->newest_cmd)
			{
				if(hist->cmds[curr_cmd]!=NULL)
				{
					printf("%s",hist->cmds[curr_cmd]);
					curr_cmd ++;
				}
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
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
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
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
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
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}
//**************************************************************************************
// function name: updateHistory
// Description: update the history list of commands
// Parameters: command string, pointer history
// Returns: no resturn(void)
//**************************************************************************************

void updateHistory(char *lineSize, history* hist)
{
	if (lineSize == NULL)//the command line is empty
	{
		return; 
	}
	if (hits->cmds[hist->newest_cmd_idx] != NULL)
		free(hits->cmds[hist->newest_cmd_idx]);
	hits->cmds[hist->newest_cmd_idx] = (char*)malloc(sizeof(char)*(strlen(lineSize)+1));
	if (hits->cmds[hist->newest_cmd_idx] == NULL)
	{
		perror("dynamic allocation failed\n");
		return;
	}
	strcpy(hits->cmds[hist->newest_cmd_idx], lineSize);

	hist->newest_cmd_idx = (hist->newest_cmd_idx +1) % HIST_SIZE;
	if (hist->newest_cmd_idx == hist->oldest_cmd_idx)
	{
		hist->oldest_cmd_idx =(hist->oldest_cmd_idx + 1)%HIST_SIZE;
	}
	
}
