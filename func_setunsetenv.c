#include "header.h"

void parse21 (char *temp, char **args)
{
	while (*temp != '\0') {
		while (*temp == ' ' || *temp == '\t')
			*temp++ = '\0';
		*args++ = temp;
		while (*temp != ' ' && *temp != '\0' && *temp != '\t')
			temp++;
	}
	*args = '\0';
}

void function_setenv (char *cmd) {
    char command[5000];
    char *args[100];
    int i;
    for (i = 0; i< strlen(cmd);i++) {
        command[i] = cmd[i];
    }
    command[i] = '\0';
    parse21(command, args);
    i=0;
    while (args[i]!=NULL) 
        i++;
    if (i == 1 || i > 3)
        fprintf(stderr,"Invalid arguements: give 1 or 2 arguements\n");
    
    if (i == 2) {
        setenv (args[1], "", 1);
    }
    if (i==3) {
        setenv (args[1], args[2], 1);
    }
}

void function_unsetenv (char *cmd) {
    char command[5000];
    char *args[100];
    int i;
    for (i = 0; i< strlen(cmd);i++) {
        command[i] = cmd[i];
    }
    command[i] = '\0';
    parse21(command, args);
    i=0;
    while (args[i]!=NULL) 
        i++;
    if (i != 2)
        fprintf(stderr,"Invalid arguements: give exactly 2 arguements\n");
    else {
        unsetenv (args[1]);
    }
}