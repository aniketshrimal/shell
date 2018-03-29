#include "header.h"

extern int saved_stdout; 
extern int saved_stdin;
extern int background_processes;
typedef struct jobs {
    char status[10], name[101];
    pid_t pid;
    int flag;
}jobs;
extern jobs curid;
extern jobs job [1000];