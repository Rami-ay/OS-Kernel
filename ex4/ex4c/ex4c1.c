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
#define TRUE 1
#define FALSE 0
#define MAX_MSG_LEN 1000
// ---- struct ----
struct my_msgbuf
{
	long m_type;
	//[prime number]
	char* my_text;
};
//--------------------------------------------------------
//function that gets a number and returns true if it is prime number
//false otherwise:
int is_prime(int number)
{
	if (number == 1)
		return FALSE;

	for (int j = 2; j <= number / 2; j++)
	{
		if (number % j == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}
//--------------------------------------------------------
void prime_server()
{
	key_t key;
	int queue_id, number;
	char temp_str[MAX_MSG_LEN];
	char* space = " ";
	char* result_string;


	// ======= Queue Creation: =========
	//get external Queue ID:
	if((key = ftok(".",'p')) == -1)
	{
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}
	//Create the queue, if it exists we exit with failure:
	if((queue_id = msgget(key,0600 | IPC_CREAT | IPC_EXCL)) == -1)
	{
		perror("msgget Failed");
		exit(EXIT_FAILURE);
	}
	//start the loop:
	while(1)
	{
		//initiate struct of type my_msgbuf:
		struct my_msgbuf my_msg;
		//we read and write messages of type = 1:
		my_msg.m_type = 1;
		//recieve a message, if failed exit with failure:
		if(msgrcv(queue_id, &my_msg,MAX_MSG_LEN,my_msg.m_type,0) == -1)
		{
			perror("prime server prog: msgrcv failed");
			exit(EXIT_FAILURE);
		}
		//now we recieved a string inside my_msg:

		//store the string in temp str:
		strcpy(temp_str,my_msg.my_text);

		// --------------------------------------------------
		//split the string to tokens splitted by " " (space):
		// --------------------------------------------------

		//split the first token:
		char* token = strtok(temp_str," ");
		//while the string still has numbers in it, keep spliting by
		//spaces:
		while(token != NULL)
		{
			//convert the number to integer:
			number = atoi(token);
			//if the number is prime:
			if(is_prime(number) == TRUE)
			{
				//add it to the result string:
				strcat(result_string,token);
				//add a space to the result string:
				strcat(result_string,space);
			}
			token = strtok(NULL, " ");
		}

		// --------------------------------------------------------------
		//now we want to write back the result string to the queue:
		// --------------------------------------------------------------
		my_msg.m_type = 1;
		strcpy(my_msg.my_text,result_string);

		//send my_msg to the named pipe:
		if(msgsnd(queue_id,&my_msg,strlen(my_msg.my_text)+1,0) == -1)
		{
			perror("prime server prog: msgsnd failed");
			exit(EXIT_FAILURE);
		}
	}
}
//--------------------------------------------------------
int main( )
{
	prime_server();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
