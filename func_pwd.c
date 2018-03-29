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
#include "func_pwd.h"

void replace(char * current, char * s_string, char * r_string) {
	char buffer[10000];
	char * ch;
	if(!(ch = strstr(current, s_string)))
		return;

	strncpy(buffer, current, ch-current);
	buffer[ch-current] = 0;
	sprintf(buffer+(ch - current), "%s%s", r_string, ch + strlen(s_string)); 
	current[0] = 0;
	strcpy(current, buffer);
	return replace(current, s_string, r_string);
}

char* get_function_pwd (char *homedir)
{
	static char current[500];
	// struct passwd *pass, *home;
	// home = getpwuid(getuid());
	// char *homedir = home->pw_dir;
	char *str;
	if (getcwd (current, sizeof(current)) == NULL) {
    	perror ("getcwd");
    }
    if (strstr (current, homedir) != NULL) {
    	replace(current, homedir, "~");
    }
    return current;
}



char* function_pwd ()
{
	static 	char current[500];
	if (getcwd (current, sizeof(current)) == NULL) {
    	perror ("getcwd");
    }
    return current;
}

