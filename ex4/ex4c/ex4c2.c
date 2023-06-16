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
//this function gets a string as an input and return 1 if the
//string palindrome , o.w it returns 0:
int is_palindrome(char* string)
{
	// if flag = 1 the string is not palindrome
	// if flag == 0 the string is palindrome
	int flag = 0;
	int length = strlen(string);

	for(int i = 0;i < length; i++)
	{
		//if the chars from left to right != from right to left
		//then the string is not palindrome:
		if(string[i] != string[length-i-1])
		{
			flag = 1;
			break;
		}
	}
	if (flag)
		return FALSE;
	else
		return TRUE;
}
//--------------------------------------------------------
void palindrome_server()
{
	key_t key;
	int queue_id;
	char temp_str[MAX_MSG_LEN];

	// ======= Queue Creation: =========
	//get external Queue ID:
	if((key = ftok(".",'q')) == -1)
	{
		perror("palindrome server prog : ftok() failed");
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
		//we read and write messages of type = 2:
		my_msg.m_type = 2;
		//Receive a message, if failed exit with failure:
		if(msgrcv(queue_id, &my_msg,MAX_MSG_LEN,my_msg.m_type,0) == -1)
		{
			perror("palindrome server prog: msgrcv failed");
			exit(EXIT_FAILURE);
		}
		//now we received a string inside my_msg:
		//store the string in temp str:
		strcpy(temp_str,my_msg.my_text);

		// ----------------------------------
		//check if the string is polindrome:
		// ----------------------------------
		my_msg.m_type = 2;
		//if the string is polindrome we send to the queue "yes"
		if(is_palindrome(temp_str) == TRUE)
		{
			//write back "yes" string to the queue:
			strcpy(my_msg.my_text,"yes");

			//send my_msg to the queue:
			if(msgsnd(queue_id,&my_msg,strlen(my_msg.my_text)+1,0) == -1)
			{
				perror("palindrome server prog: msgsnd failed");
				exit(EXIT_FAILURE);
			}
		}

		//if the string is NOT a polindrome we send to the queue "no"
		if(is_palindrome(temp_str) == FALSE)
		{
			//write back "no" string to the queue:
			strcpy(my_msg.my_text,"no");
			//send my_msg to the queue:
			if(msgsnd(queue_id,&my_msg,strlen(my_msg.my_text)+1,0) == -1)
			{
				perror("palindrome server prog: msgsnd failed");
				exit(EXIT_FAILURE);
			}
		}
	}
}
//--------------------------------------------------------
int main( )
{
	palindrome_server();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
