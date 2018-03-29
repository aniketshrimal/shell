#include <stdio.h>
#include <unistd.h>  // for current directory (getcwd)
#include <stdlib.h>  // also required for chdir
#include <errno.h>
#include <sys/utsname.h>
#include <pwd.h>    // for struct passwd
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>     // for dirent and scandir
#include <sys/dir.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <grp.h>


char **parse_for_one_delim(char *command, char* delim)
{
    int pos = 0;
    char **args = malloc(1000*sizeof(char *)), *token;
    token = strtok(command, delim);
    while(token) {      
        args[pos] = token;
        pos++;
        token = strtok(NULL, delim); 
    }
    args[pos]=NULL;
    pos++;
    return args;
}