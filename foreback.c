#include "header.h"
#include "global.h"


void parse(char *line, char **argv)
{
	while (*line != '\0') {
		while (*line == ' ')
		*line++ = '\0';
		*argv++ = line;
		while (*line != '\0' && *line != ' ') 
		line++;
	}
	*argv = '\0';
}

void fore (char **argv, char *command)
{
	int  pid;
	int   status;
	
	if ((pid = fork()) < 0) {     /* fork a child process           */
		printf("*** ERROR: forking child process failed\n");
		return;
	}
	else if (pid == 0) {
		//		printf("yes\n");
		
		int ch=execvp(command, argv);          /* for the child process:         */
		if( ch< 0) {
			printf("Command does not exist\n");
			exit(0);
		}
		return;
	}
	else {
		curid.pid = pid;
		strcpy(curid.name, command);
		strcpy(curid.status, "Stopped");
		curid.flag = 0;
										/* for the parent:      */
		while (waitpid(pid,&status,WUNTRACED) != pid);      /* wait for completion  */
		// 	exit(0);
	}
	
}

void back (char **argv, char *command)
{
	int pid;
	int id = -4;
	if ((pid = fork()) < 0) {
		printf("Failed while forking\n");
		return;
	}
	else if (pid == 0) {
		// printf("%d\n", getpid());
		
		if (execvp (command, argv) < 0){
			printf("Command does not exist\n");
			return;	
		}
		
		
		
	}
	else {
		// id = getpid ();
		
		// adding the process into the job array since the process is a background process.
		job[background_processes].pid = pid;
		strcpy(job[background_processes].status, "Running");
		strcpy(job[background_processes].name, command);
		job[background_processes].flag = 1;
		background_processes++;
		
		printf("[%d]\n", pid);	
	}
}


