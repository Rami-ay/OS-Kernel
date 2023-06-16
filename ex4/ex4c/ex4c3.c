// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
// ---- consts --------
#define MAX_MSG_LEN 1000
// ---- struct --------
struct my_msgbuf
{
	long m_type;
	char my_text[MAX_MSG_LEN];
};
//--------------------------------------------------------
//function that gets queue_id and checks if it open successfuly:
void check_if_open(int queue_id)
{
	if(queue_id == -1)
	{
		perror("mssget failed in reader");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//function that gets fork status as input and check if the fork
// fails and handle it:
void check_fork(pid_t status)
{
	if (status < 0)
	{
		perror("cannot fork()");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
void front_end()
{
	key_t key;
	int index = 0;
	pid_t status;
	int queue_id;
	char ch;
	char command;
	char prime_str[MAX_MSG_LEN];
	char palindrome_str[MAX_MSG_LEN];
	struct my_msgbuf my_msg;

	while(1)
	{
		//ask the user to enter the desired command:
		printf("Please enter p and numbers for the primes server,\nand q and a string for the palindrome server:\n");
		//get from the user the desired command:
		scanf("%c",&command);

		if (command == 'p')
		{
			while ((ch = getchar()) != 0 && index < MAX_MSG_LEN)
			{
				if( ch == '0')
				{
					prime_str[index] = '\0';
					break;
				}
				else
				{
					prime_str[index] = ch;
					index++;
				}
			}
			//now we have a string to send to the prime server:
			//fork a child process:
			status = fork();
			//check if the fork succeded:
			check_fork(status);
			if(status == 0)
			{
				if (execlp("./ex4c1","ex4c1",NULL) != 0)
				{
					perror("prime execlp() failed");
					exit(EXIT_FAILURE);
				}
			}
			sleep(2);
			// get external queue key:
			if((key = ftok(".",'p')) == -1)
			{
				perror("front-end prog: ftok() failed");
				exit(EXIT_FAILURE);
			}
			//Open a queue, if the it fails then exit:
			queue_id = msgget(key,0);
			check_if_open(queue_id);

			//fill my_msg in order to send to the prime server:
			my_msg.m_type = 1;
			strcpy(my_msg.my_text,prime_str);

			//send my_msg to the prime queue:
			if(msgsnd(queue_id,&my_msg,strlen(my_msg.my_text)+1,0) == -1)
			{
				perror("prime server prog: msgsnd failed");
				exit(EXIT_FAILURE);
			}
			//Receive the primes in the string:
			my_msg.m_type = 1;
			//Receive a message, if failed exit with failure:
			if(msgrcv(queue_id, &my_msg,MAX_MSG_LEN,my_msg.m_type,0) == -1)
			{
				perror("prime server prog: msgrcv failed");
				exit(EXIT_FAILURE);
			}
			//print the primes on the screen:
			printf("%s \n",my_msg.my_text);

		}

		if (command == 'q')
		{

			scanf("%s",palindrome_str);
			//get external Queue ID:

			//fork a child process:
			status = fork();
			//check if the fork succeded:
			check_fork(status);

			if(status == 0)
			{
				if (execlp("./ex4c2", "ex4c2", NULL) != 0)
				{
					perror("palindrome execlp() failed");
					exit(EXIT_FAILURE);
				}
			}
			// get external queue key:
			if((key = ftok(".",'q')) == -1)
			{
				perror("front-end prog: ftok() failed");
				exit(EXIT_FAILURE);
			}
			//Open a queue, if the it fails then exit:
			queue_id = msgget(key,0);
			check_if_open(queue_id);
			//fill my_msg in order to send to the palindrome server:
			my_msg.m_type = 2;
			strcpy(my_msg.my_text,palindrome_str);

			//send my_msg to the palindrome queue:
			if(msgsnd(queue_id,&my_msg,strlen(my_msg.my_text)+1,0) == -1)
			{
				perror("palindrome server prog: msgsnd failed");
				exit(EXIT_FAILURE);
			}


			//Receive if the string is palindrome or not:
			my_msg.m_type = 2;
			//Receive a message, if failed exit with failure:
			if(msgrcv(queue_id, &my_msg,MAX_MSG_LEN,my_msg.m_type,0) == -1)
			{
				perror("palindrome server prog: msgrcv failed");
				exit(EXIT_FAILURE);
			}
			//the string is palindrome:
			if(strcmp(my_msg.my_text,"yes") == 0)
			{
				printf("+\n");
			}
			if(strcmp(my_msg.my_text,"no") == 0)
			{
				printf("-\n");
			}
		}
	}
}
//--------------------------------------------------------
int main( )
{
	front_end();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
