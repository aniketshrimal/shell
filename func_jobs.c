#include "header.h"
#include "global.h"

void function_jobs () {
    // printf ("%d", background_processes);
    if (background_processes == 0)
        printf ("No background process is present currently\n");
    
    for(int i = 0;i < background_processes; i++){
        if(job[i].flag == 1)
            strcpy(job[i].status, "Running");
        else
            strcpy(job[i].status, "Stopped");
        printf("[%d] %s  %s [%d]\n", i+1, job[i].status, job[i].name, job[i].pid);
    }
}