#include"header.h"
#include "global.h"

void pop_job(int index){
    if (background_processes == 0)
        return;
    int i;
    index;
    for(i = index;i < background_processes - 1; i++){
        job[i] = job[i+1];
    }
    background_processes--;
}