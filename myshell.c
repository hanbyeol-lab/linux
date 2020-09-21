#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{

	char input[255];
	char *token[255];
	char *cpy[255];
	char *ptr;
	char null[50]=" ";
	int status;
	int i;
	pid_t pid;


	while(1)
	{

		printf("Hanbyeol $ ");
		fgets(input, 255, stdin);

		pid=fork();

		input[strlen(input)-1]='\0';
		ptr=strtok(input, " ");

		i=0;
		while(ptr!=NULL)
		{
			token[i]=ptr;
			ptr=strtok(NULL, " ");
			i++;
		}
		token[i]=NULL;

		if(pid<0) //fork failed
		{
			perror("fork failed\n");
			break;
		}

		else if(pid==0) //child
		{
			execvp(token[0], token);
		}

		else //parent
		{
			while(waitpid(pid, &status, 0)<0);
		}


		if(strcmp(input, "exit")==0)
		{
			exit(0);
			break;
		}
	}
}
