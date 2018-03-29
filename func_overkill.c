#include "header.h"
#include "global.h"
#include "remove_job.h"

void function_overkill()
{
    int i;
    if(background_processes < 1){
        printf("No Background Process present\n");
        return;
    }
    else{
        for(i = background_processes - 1;i >= 0; i--){
            kill(job[i].pid, 9);
            pop_job(i);
        }
    }
    background_processes = 0;
}
