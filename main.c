#define clear() printf("\033[H\033[J")
#define MAKE_GREEN "\e[32m" 
#define RESET_COLOR "\e[m" 
#define MAKE_BLUE "\e[36m"
#include "func_cd.h"
#include "func_ls.h"
#include "func_pwd.h"
#include "foreback.h"
#include "pinfo.h"
#include "nightswatch.h"
#include "redirection.h"
#include "parse.h"
#include "func_setunsetenv.h"
#include "global.h"
#include "func_jobs.h"
#include "header.h"
#include "func_kjob.h"
#include "remove_job.h"
#include "func_fg.h"
#include "func_overkill.h"
#include "signal_handle.h"
#include "func_bg.h"
#include "pipe.h"
int saved_stdout; 
int saved_stdin;
int background_processes;
jobs curid;
jobs job[1000];

int pipecheck(char *s)
{
	int i=0;
	while(s[i])
	{
		if(s[i]=='|')
			return 1;
		i++;
	}
	return 0;
}

int main ()
{
	// signal(SIGINT,SIG_IGN);
	signal(SIGINT,sig_handle);
	// signal(SIGCHLD,SIG_IGN);
	signal(SIGCHLD,child_sig);
	// signal(SIGTSTP,SIG_IGN);
	signal(SIGTSTP,sig_z);
	// signal(SIGQUIT,SIG_IGN);
	signal(SIGQUIT,sig_handle);
	background_processes = 0;
	// struct utsname sysname;
	struct passwd *pass;
	char *token,*temp1,*temp[1000];
	char delim[2] = " ";
	char delim1[2] = ";";
	saved_stdout = dup(1);
	saved_stdin = dup(0);
	
	pass = getpwuid (geteuid());

    // if (uname(&sysname) != 0) {
    // 	perror("uname");
    // 	return 0;
    // }

	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	// printf("Hostname: %s\n", hostname);

	char homedir[500];
	if (getcwd (homedir, sizeof(homedir)) == NULL) {
		perror ("getcwd");
	}
	int j=0,k,val=1;
	clear();
	// printf("%s\n", homedir);
	while (1) {
		
		char *args[102],line[1000],arg[1000],*path=NULL;
		printf(MAKE_GREEN"<%s@%s : %s>"RESET_COLOR, pass->pw_name, hostname, get_function_pwd(homedir));
		k=0;
		while(1)
		{
			scanf("%c",&arg[k]);
			if(arg[k]=='\n')
			{	
				arg[k]='\0';
				break;
			}
			k++;
		}
		
		temp1= strtok (arg,delim1);
		j=0;
		while(temp1!=NULL)
		{
			temp[j]=temp1;
			j++;
			temp1=strtok (NULL,";");
		}	

		for(k=0;k<j;k++)
		{
			if(pipecheck(temp[k]))
			{
				parse_pipe(temp[k]);
				continue;
			}
			inp_redirection(temp[k]);
			opt_redirection(temp[k]);
			char **temp123 = parse_for_one_delim(temp[k], "<");
			char **command = parse_for_one_delim(temp123[0], ">");
			strcpy (temp[k], command[0]);
			strcpy (line, temp[k]);
			
			token = strtok (temp[k], delim);
			if (strcmp (token, "quit") == 0) {
				val=0;
				clear();
				exit(0);
			}
			if (strcmp (token, "clear") == 0)
				clear ();
			else if (strcmp(token,"cd")==0) {
				token = strtok (NULL, delim);
				function_cd (homedir, token);
			}
			else if (strcmp (token, "echo") == 0) {
				token = strtok (NULL, delim);
				while (token != NULL) {
					printf("%s ", token);
					token = strtok (NULL, delim);
				}
				printf("\n");
			}
			else if (strcmp (token, "ls") == 0) {
				int yo = 0;
				token = strtok (NULL, delim);
				while (token != NULL) {
					// token = strtok (NULL, delim);
					if (strcmp (token, "-a") == 0) {
						if (yo == 0)
							yo = 1;
						else
							yo = 3;
					}
					else if (strcmp (token, "-l") == 0) {
						if (yo == 0)
							yo = 2;
						else
							yo = 3;
					}
					else if (strcmp (token, "-al") == 0 || strcmp (token, "-la") == 0)
						yo = 3;
					else{
						path=token;
					}
					token = strtok (NULL, delim);
				}
				function_ls (yo,path,homedir);
			}
			else if (strcmp (token, "pinfo") == 0)
			{
				pinfo(line);

			}
			else if (strcmp (token, "setenv") == 0) {
				function_setenv (line);
			}
			else if (strcmp (token, "unsetenv") == 0)
				function_unsetenv (line);
			else if (strcmp (token, "jobs") == 0)
				function_jobs ();
			else if (strcmp (token, "kjob") == 0)
				function_kjob(line);
			else if (strcmp (token, "fg") == 0)
				function_fg(line);
			else if (strcmp (token, "overkill") == 0)
				function_overkill();
			else if (strcmp (token, "bg") == 0)
				function_bg(line);
			else if (strcmp (token, "nightswatch") == 0) {
				int counter = 2;
				token = strtok (NULL, delim);
				while (token != NULL) {
					if (strcmp (token, "-n") == 0) {
						token = strtok (NULL, delim);
						counter = atoi (token);
					}
					else {
						if (strcmp (token, "dirty") == 0) {
							nightswatch (counter, "dirty");
						}
						else if(strcmp (token, "interrupt") == 0) {
							nightswatch (counter, "interrupt");
						}
						else
							printf("invalid command\n");
					}
					token = strtok (NULL, delim);
				}

			}
			else
			{
				// printf ("%s", line);
				parse(line, args);
				// printf ("%s", args[1]);
				int i=0;
				while (args[i])
					i++;
				if (strcmp(args[i-1], "&") == 0) {
					args [i-1] = NULL;
					signal(SIGCHLD, signalhandler);
					back(args, args[0]);
				}
				else
				{
					// printf ("%s %s\n", args[0], args[1]);					
					fore(args, args[0]);
				}	
			}
			if (dup (STDIN_FILENO) != saved_stdin) {
				close (STDIN_FILENO);
				dup2 (saved_stdin, STDIN_FILENO);
			}
			
			if (dup(STDOUT_FILENO) != saved_stdout) {
				close(STDOUT_FILENO);
				dup2 (saved_stdout, STDOUT_FILENO);
			}
		}
		if (val==0)
			return 0;
		

	}
	return 0;
}
