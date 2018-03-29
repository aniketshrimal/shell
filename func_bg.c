#include"header.h"
#include"global.h"
#include "parse.h"
// #include "remove_job.h"

void function_bg(char *cmd){
    int i;
	char command[1000];
	for (i=0;i<strlen(cmd);i++)
        command[i] = cmd[i];
    char **args = parse_for_one_delim(command, " ");

    if(args[1] != NULL){
        int index = atoi(args[1]);
        if(index > background_processes || index < 1)
            printf(" job number is incorrect\n");
        else{
            if(kill(job[index - 1].pid, SIGCONT) < 0){
                perror("kill(SIGCONT)");
                return ;
            }
            char *name = job[index-1].name;
            // pop_job(index - 1);
            if (job[index - 1].flag != 1){
                job[index - 1].flag = 1;
                strcpy(job[index - 1].status, "Running");
                printf("%s : Process is now Running in the background\n", name);
            }
            else
                printf ("%s : Process already running\n", name);
        }
    }
    else{
        printf("Usage: bg [job number]\n");
    }
}