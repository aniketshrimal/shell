#include "header.h"
#include "global.h"
#include "remove_job.h"

void signalhandler(int signal)
{
	int i;
	pid_t pid;
	pid = wait(NULL);

	for (i=0;i<background_processes;i++)
		if (job[i].pid == pid)
			break;
	pop_job (i+1);

	printf("Pid %d exited.\n", pid);
}


// char buffer[100];

void sig_handle(int sign)
{
	if (sign==2 ||sign==3)
	{
		fflush(stdout);
		printf("\n");
		// prompt();
		signal(SIGQUIT,sig_handle);
		signal(SIGINT,sig_handle);
		
	}
		// if(sign == 20) {
		// 	kill(curid,SIGSTOP);
		// }
		// while (background_processes != 0)
		// 	pop_job (1);
		// printf ("PLEASE PRESS ENTER\n");
	
	return;
}

void sig_z(int sign) {
	// printf ("abkvjs\n");
	kill(curid.pid, SIGSTOP);

	job[background_processes++] = curid;

	fflush(stdout);
}

void child_sig(int signo)
{
	pid_t pid;
	int r;
	pid=waitpid(WAIT_ANY,&r, WNOHANG);
	int i;
	for(i=0;i<background_processes;i++)
	{
		if(job[i].pid==pid && job[i].flag == 1)
		{
			job[i].flag = 0;
			printf("\n%s %d exited normally\n",job[i].name,job[i].pid);
			// prompt();
			fflush(stdout);
		}
	}
	signal(SIGCHLD, child_sig);
	return;
}