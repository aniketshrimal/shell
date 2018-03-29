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
#include "func_cd.h"


int function_cd (char *home, char *token)
{
	if (token == NULL) 
	{
		if (chdir (home) == 0) {

			// printf("%s\n", home);
			return 1;
		}
		else {
			perror ("cd");
			return 0;
		}
	}
	else {
		int i=0;
		char str[1000];
		if (token[0] == '~') {
			sprintf (str, "%s", home);
			int len = strlen (str);
			for (i = 1; i < strlen (token); i++) {
				str[len-1+i] = token[i];
			}
			str[strlen(token) + len-1] = '\0';
			// printf("%s\n", str);
		}
		else
			strcpy (str,token);
		if (chdir (str) == 0) 
			return 1;
		else {
			perror ("cd");
			return 0;
		}
	}
	return 0;
}