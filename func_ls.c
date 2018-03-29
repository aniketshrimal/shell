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

	
#include "func_ls.h"
#include "func_pwd.h"
#define RESET_COLOR "\e[m" 
 #define MAKE_GREEN "\e[32m" 
 #define MAKE_BLUE "\e[36m" 

int function_ls (int value,char *path,char *home) {
	// value = 0 i.e only ls
	// value = 1 i.e ls -a
	// value = 2 i.e ls -l
	// value = 3 i.e ls -al (can be in any order)

	// printf("ls called with value = %d\n", value);
    int total, i;
    struct stat filestat;
	struct dirent **list;
	static 	char current[500];
	if (getcwd (current, sizeof(current)) == NULL) {
    	perror ("getcwd");
    }
				
	if (value == 0) {
		if(path==NULL)
			total = scandir(".", &list, NULL, alphasort);
		else
		{
			char str[1000];
			if (path[0] == '~') {
				sprintf (str, "%s", home);
				int len = strlen (str);
				for (i = 1; i < strlen (path); i++) {
					str[len-1+i] = path[i];
				}
				str[strlen(path) + len-1] = '\0';
			}
			else
				strcpy (str,path);	
			total =	scandir(str, &list, NULL, alphasort);
		}
		if (total < 0) {
			perror("scandir");
			return 0;
		}
		for (i = 0; i < total; i++) {
			if (list[i]->d_name[0] != '.')
					printf("%s\n", list[i]->d_name);
				
		}
    	free(list);
    	return 0;
	}
	if (value == 1) {
		// printf("ls -a called\n");
		if(path==NULL)
			total = scandir(".", &list, NULL, alphasort);
		else
		{
			char str[1000];
			if (path[0] == '~') {
				sprintf (str, "%s", home);
				int len = strlen (str);
				for (i = 1; i < strlen (path); i++) {
					str[len-1+i] = path[i];
				}
				str[strlen(path) + len-1] = '\0';
			}
			else
				strcpy (str,path);	
			total =	scandir(str, &list, NULL, alphasort);
		}
		if (total < 0) {
			perror("scandir");
			return 0;
		}
		for (i = 0; i < total; i++)
				printf("%s\n",  list[i]->d_name);

				
    	free(list);
    	return 0;
	}
	if (value == 2 || value == 3) {
		char str[1000];
			
		if(path==NULL)
			total = scandir(".", &list, NULL, alphasort);
		else
		{
			if (path[0] == '~') {
				sprintf (str, "%s", home);
				int len = strlen (str);
				for (i = 1; i < strlen (path); i++) {
					str[len-1+i] = path[i];
				}
				str[strlen(path) + len-1] = '\0';
			}
			else
				strcpy (str,path);	
			total =	scandir(str, &list, NULL, alphasort);
		}
		if (total < 0) {
			perror("scandir");
			return 0;
		}
		for (int i = 0; i < total; ++i) {
			if (list[i]->d_name[0] == '.' && value == 2)
				continue;
			char *file_name = list[i]->d_name;
			int fd=open(file_name,O_RDONLY||O_WRONLY);
			int p=stat (file_name, &filestat);
			if ( p<0) {
				
				chdir(str);
					// printf("Problem occured while reading the file %s\n", file_name);
				// return 0;
			}
			else {
				if (S_ISDIR(filestat.st_mode)) 
					printf("d");
				else if (S_ISLNK (filestat.st_mode))
					printf("l");
				else
					printf("-");

				if (filestat.st_mode & S_IRUSR)
					printf("r");
				else
					printf("-");

				if (filestat.st_mode & S_IWUSR)
					printf("w");
				else
					printf("-");

				if (filestat.st_mode & S_IXUSR)
					printf("x");
				else
					printf("-");

				if (filestat.st_mode & S_IRGRP)
					printf("r");
				else
					printf("-");

				if (filestat.st_mode & S_IWGRP)
					printf("w");
				else
					printf("-");

				if (filestat.st_mode & S_IXGRP)
					printf("x");
				else
					printf("-");

				if (filestat.st_mode & S_IROTH)
					printf("r");
				else
					printf("-");

				if (filestat.st_mode & S_IWOTH)
					printf("w");
				else
					printf("-");

				if (filestat.st_mode & S_IXOTH)
					printf("x");
				else
					printf("-");
				printf(" %lu ",filestat.st_nlink);
				struct passwd *user = getpwuid(filestat.st_uid);
				printf("%s ", user->pw_name);
				struct group *group = getgrgid(filestat.st_gid);
				printf("%s ", group->gr_name);
				// printf("%lu\t", filestat.st_ino);
				// if (S_ISDIR(filestat.st_mode)) 
				printf("%ld\t", filestat.st_size);
				// else{
					// int siz = lseek (fd, 0, SEEK_END);
					// printf("%d\t", siz);
				// }
				printf("%.16s ", ctime(&filestat.st_mtime));
				if (S_ISDIR(filestat.st_mode)) 
				{
					printf(MAKE_BLUE"%s\t"RESET_COLOR, file_name);
				}
				else
				{
					if(filestat.st_mode & S_IXUSR)
						printf(MAKE_GREEN"%s\t"RESET_COLOR, file_name);
					else
						printf("%s\t", file_name);

				}	
				printf("\n");
			}
			
		}
	}
	chdir(current);
	return 1;
}
