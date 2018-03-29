#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <stdlib.h>

void parseline (char *temp, char **args)
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


void pinfo(char temp[])
{
	int pid;
	FILE *file;
	char *args[64];
	char str[1000], new_str[1000], str1[1000], line[1000];
	parseline (temp, args);
	if (strcmp (args[0], "pinfo") == 0) {
		if (args[1] == NULL) {
			pid = getpid();
			printf("PID -- %d\n", pid);
			sprintf (str, "%s%d", "/proc/",pid);
			sprintf (new_str, "%s%s", str, "/exe");
			// printf("%s\n", new_str);
			ssize_t len = readlink (new_str, str1, sizeof(str1)-1);
			if (len > 0) {
				str1[len] = '\0';
				printf("Executable path -- %s\n", str1);
			}
			else
				perror ("readlink");
		}
		else
			pid = atoi(args[1]);
		sprintf (new_str, "%s%d%s", "/proc/", pid, "/status");
		if (! (file = fopen (new_str, "r"))) {
			perror ("fopen");
			return;
		}
		if (args[1] != NULL) {
			printf("PID -- %d\n", pid);
			sprintf (str, "%s%d", "/proc/",pid);
			sprintf (new_str, "%s%s", str, "/exe");
			// printf("%s\n", new_str);
			ssize_t len = readlink (new_str, str1, sizeof(str1)-1);
			if (len > 0) {
				str1[len] = '\0';
				printf("Executable path -- %s\n", str1);
			}
			else
				perror ("readlink");
		}
		struct rusage usage;
		getrusage(RUSAGE_SELF,&usage);
		printf("Memory Usage (max) -- %ld KB's\n", usage.ru_maxrss);
		// sprintf (str, "%s%s", new_str, "/status");
		// printf("%s\n", str);
		
		if ( file != NULL ) {
			while (fgets(line, sizeof(line), file) != NULL) {
				// printf("%s\n", line);
				char *argv[10];
				char ltemp[1000];
				strcpy (ltemp, line);
				parseline (line, argv);
				// printf("%s\n", argv[0]);
				if (strcmp (argv[0], "State:") == 0) {
					printf("%s\n", ltemp);
					break;
				}
 			// 	if (count == lineNumber) {
				// 	printf("%s",line);
				// 	break;
				// }
				// else
				// {
				// 	count++;
				// }
			}
			// printf("Executable path: %s\n",exec);
			// printf("Memory Usage: %ld\n",usage.ru_maxrss);
			fclose(file);
		}
	}
}