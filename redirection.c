#include <stdio.h>
#include <unistd.h>  // for current directory (getcwd)
#include <stdlib.h>  // also required for chdir
#include <errno.h>
#include <sys/utsname.h>
#include <pwd.h>    // for struct passwd
#include <sys/types.h>
#include <string.h>
#include <dirent.h>     // for dirent and scandir
#include <sys/dir.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include "redirection.h"
#include "parse.h"

void opt_redirection (char *cmd) {
	int i;
	char command[1000];
	for (i=0;i<strlen(cmd);i++)
        command[i] = cmd[i];
    command[strlen(cmd)] = '\0';

    // printf("%s\n", command);

	int output;
	i=0;
	if (strstr, ">>", command) {
		char **temp = parse_for_one_delim (command, ">");
		// printf ("%s\n%s\n%s\n", temp[0], temp[1], temp[2]);
		char ** new = parse_for_one_delim(temp[1], " ");
		output = open(new[0], O_WRONLY| O_CREAT | O_APPEND, 0666);
		dup2(output,STDOUT_FILENO);
		close(output);
	}
	else {
		char **temp = parse_for_one_delim (command, ">");
		// printf("%s\n", temp[1]);
		// return ;
		if (temp[1] != NULL) {
			//printf("%s\n", temp[1]);
			char ** new = parse_for_one_delim(temp[1], " ");
			// printf("%s\n", new[0]);
			output = open(new[0], O_WRONLY| O_TRUNC | O_CREAT, 0777);
			dup2(output,STDOUT_FILENO);
			close(output);
		}
	}
}

void inp_redirection (char *cmd) {
	int i;
	char command[1000];
	for (i=0;i<strlen(cmd);i++)
        command[i] = cmd[i];
    command[strlen(cmd)] = '\0';

	int input;
	i=0;
	char **temp = parse_for_one_delim (command, "<");
	if (temp[1] != NULL) {
		char ** new = parse_for_one_delim(temp[1], " ");
		input = open(new[0], O_RDONLY);
		dup2(input,STDIN_FILENO);
		close(input);
	}
}