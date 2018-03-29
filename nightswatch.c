#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <poll.h>
#include <termios.h>


void nightswatch(int time,char* command);

void parse2 (char *temp, char **args)
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


void func_dirty () {
	char *path = "/proc/meminfo";
	char line[1000];
	FILE *file;
	if (! (file = fopen (path, "r"))) {
		perror ("fopen");
		exit (0);
	}
	if ( file != NULL ) {
		while (fgets(line, sizeof(line), file) != NULL) {
			// printf("%s\n", line);
			char *argv[10];
			parse2(line, argv);
			if (strcmp (argv[0], "Dirty:") == 0) {
				printf("%skb\n", argv[1]);
				break;
			}
		}
		fclose(file);
	}
	return ;
}
int flag=0;

void func_interrupt () 
{
	char *path = "/proc/interrupts";
	char line[1000];
	FILE *file;
	if (! (file = fopen (path, "r"))) {
		perror ("fopen");
		exit (0);
	}
	if ( file != NULL ) {
		while (fgets(line, sizeof(line), file) != NULL) {
			// printf("%s\n", line);
			char *argv[10];
			parse2 (line, argv);
			if (flag==0) {
				printf("\t");
				while (argv[flag][0] == 'C') {
					printf("%s\t", argv[flag]);
					flag++;
				}
				printf("\n");
			}
			if (strcmp (argv[0], "0:") == 0) {
				printf("\t");
				for (int i = 0; i < flag; i++) {
					printf("%s\t", argv[i+1]);
				}
				printf("\n");
				break;
			}
		}
		fclose(file);
	}
	return ;
}

void nightswatch(int time,char* command)
{
    int n;
    struct termios old;
    struct termios new;
    unsigned char key;

    tcgetattr(0,&old);
    
    new = old;
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    new.c_lflag &= ~ISIG;
    new.c_cc[VMIN] = 0;
    new.c_cc[VTIME] = 0;
    tcsetattr(0,TCSANOW,&new);
    while(1)
    {
        n = getchar();
        if (n != EOF)
        {
            key = n;
            if(key == 'q')
            {
                break;
            }
        }
        if(strcmp(command,"interrupt") == 0)
            func_interrupt ();
        if(strcmp(command,"dirty") == 0)
            func_dirty ();
        sleep(time);
        fflush(stdout);
    }
    tcsetattr(0,TCSANOW, &old);
    return ;
}