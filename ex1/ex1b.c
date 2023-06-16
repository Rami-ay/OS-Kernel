// Author : Rami Ayoub //
// ID : 315478966 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define STR_SIZE 1000

//--------------------------------------------------------
void do_child(char command[], char str1[], char str2[])
{
	//if the user asked to do str_str:
	if (strcmp(command,"./str_str")==0)
	{
		if(execlp("./str_str","str_str",str1,str2,NULL)!=0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}

	}
	//if the user asked to do count:
	if (strcmp(command,"./count")==0)
	{
		if(execlp("./count","count",str1,str2,NULL)!=0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}



}
//--------------------------------------------------------
void do_childmax(char command[], char str1[])
{

	//if the user asked to do max:
	if (strcmp(command,"./max")==0)
	{
		if(execlp("./max","max",str1,NULL)!=0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}



}
//--------------------------------------------------------
void shell(char command[], char str1[], char str2[])
{
	pid_t status;

	//fork a child:
	status = fork();
	//check if the fork succeded:
	if (status < 0)
	{
		fputs("error_in_fork()",stderr);
		exit(EXIT_FAILURE);
	}
	//child was forked and sent to the do_child func:
	else if (status == 0)
	{
		do_child(command,str1,str2);
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
void shellmax(char command[], char str1[])
{
	pid_t status;

	//fork a child:
	status = fork();
	//check if the fork succeded:
	if (status < 0)
	{
		fputs("error_in_fork()",stderr);
		exit(EXIT_FAILURE);
	}
	//child was forked and sent to the do_child func:
	else if (status == 0)
	{
		do_childmax(command,str1);
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
int main(int argc, char** argv)
{
	char command[STR_SIZE];
	char str1[STR_SIZE];
	char str2[STR_SIZE];

	scanf("%s", &command);
	while (strcmp(command,"exit")!=0)
	{
		if (strcmp(command,"./str_str")==0 ||  strcmp(command,"./count")==0)
		{
			scanf("%s", &str1);
			scanf("%s", &str2);
			shell(command,str1,str2);

		}
		if (strcmp(command,"./max")==0)
		{
			scanf("%[^\n]s", &str1);
			shellmax(command,str1);
		}

	}
	if (strcmp(command,"exit")==0)
			return(EXIT_SUCCESS);



	return(EXIT_SUCCESS);
}
