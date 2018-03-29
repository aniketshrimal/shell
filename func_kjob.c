#include "header.h"
#include "parse.h"
#include "remove_job.h"
#include "global.h"

void function_kjob(char *cmd)
{
    int i;
	char command[1000];
	for (i=0;i<strlen(cmd);i++)
        command[i] = cmd[i];
    char **args = parse_for_one_delim(command, " ");
    
    if(args[0] == NULL || args[1] == NULL || args[2] == NULL){
        printf("Usage: kjob [jobNumber] [signalNumber]\n");
    }
    else{
        int index = atoi(args[1]);
        if(index > background_processes){
            printf("Job does not exist\nRun \"jobs\" for all background processes\n");
        }
        else{
            if (kill(job[index - 1].pid, atoi(args[2])) < 0 ){
                perror("kill(SIGCONT)");  
                return ;
            }
            if(atoi(args[2]) == 9){         // In this case job must be deleted from the array and no. of jobs are decreased
                pop_job(index - 1);
            }
        }
    }
}