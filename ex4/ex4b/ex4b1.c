// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
// ---- consts ----
#define ARR_SIZE 100
#define NUM_OF_GEN 3
#define TRUE 1
#define FALSE 0

// ---- struct ----
struct my_msgbuf
{
	long m_type;
	//[prime number],[gen_id]
	int m_text[2];
};
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
void print_array(int array[], int size)
{
	printf("[");
	for (int i = 0; i < size; i++)
	{
		printf("%d,", array[i]);
	}
	printf("]\n");
}
//--------------------------------------------------------
void exec_child(int number)
{
	if (number == 0)
	{
		if (execlp("./ex4b2", "ex4b2", "1", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 1)
	{
		if (execlp("./ex4b2", "ex4b2", "2", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 2)
	{
		if (execlp("./ex4b2", "ex4b2", "3", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
}

//--------------------------------------------------------
void send_to_gen(int queue_id,int gen_id, int message)
{
	struct my_msgbuf my_msg;
	my_msg.m_type = gen_id;
	my_msg.m_text[0] = message;
	my_msg.m_text[1] = gen_id;
	if(msgsnd(queue_id,&my_msg,sizeof(my_msg.m_text),0) == -1)
	{
		perror("msgsnd failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
void print_stats(int array[],int index,int queue_id)
{
	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n", index);
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n", array[0]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n", array[index - 1]);

	//now we send to each generator command to print his stats:
	send_to_gen(queue_id,1,-1);
	send_to_gen(queue_id,2,-1);
	send_to_gen(queue_id,3,-1);
}
//--------------------------------------------------------
void filler( )
{
	int array[ARR_SIZE], prime, gen_id,
	index = 0,failures = 0, last_unadded = -1;
	int check1 = FALSE, check2 = FALSE, check3 = FALSE;
	struct my_msgbuf my_msg;
	key_t key;
	pid_t status;
	int queue_id;

	// ======= Queue Creation: =========
	//get external Queue ID:
	if((key = ftok(".",'4')) == -1)
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

	// =================================
	// Fork & Executing ex4a2 program which is the number
	// generators program:
	for (int i = 0; i < NUM_OF_GEN; i++)
	{
		status = fork();
		check_fork(status);
		if(status == 0)
		{
			exec_child(i);
			exit(EXIT_SUCCESS);
		}
	}

	//checking that all generators are up:
	for(int i = 1; i<4 ; i++)
	{
		struct my_msgbuf my_msg;
		my_msg.m_type = i;
		//reading a message form generator , each GenX sends a message
		//with type = X
		if(msgrcv(queue_id, &my_msg,sizeof(my_msg.m_text),my_msg.m_type,0) == -1)
		{
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}
		//now we stored the message from queue:
		switch(my_msg.m_text[0])
		{
		case 1:
			check1 = TRUE;
			break;
		case 2:
			check2 = TRUE;
			break;
		case 3:
			check3 = TRUE;
			break;
		}
	}
	//now if the three generators are up, we send a command to start
	//sending prime numbers:
	if (check1 == TRUE && check2 == TRUE && check3 == TRUE)
	{
		for(int gen = 1; gen < 4; gen++)
		{
			my_msg.m_type = gen;
			my_msg.m_text[0] = 1;
			my_msg.m_text[1] = gen;
			if(msgsnd(queue_id,&my_msg,sizeof(my_msg.m_text),0) == -1)
			{
				perror("msgsnd failed");
				exit(EXIT_FAILURE);
			}
		}
	}
	//else, generators is not running for some reason
	else
	{
		perror("At Least One Gen Is Not Running");
		exit(EXIT_FAILURE);
	}


	//while index did not reach the array size:
	//we fill the array:
	while (index < ARR_SIZE)
	{
		//read from the main queue a prime number:
		struct my_msgbuf my_msg;
		my_msg.m_type = 4;
		//reading a message form generator , each GenX sends a message
		//with type = X
		if(msgrcv(queue_id, &my_msg,sizeof(my_msg.m_text),my_msg.m_type,0) == -1)
		{
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}
		//now we stored the message from queue:
		prime = my_msg.m_text[0];
		gen_id = my_msg.m_text[1];

		//if the array is empty:
		if (index == 0)
		{
			array[index] = prime;
			index++;
			//send to the relevant generator that the
			//prime number was added:
			send_to_gen(queue_id,gen_id,TRUE);
		}
		//index > 0
		else
		{
			//the new prime is larger or equal to the current maximum in array:
			if (prime >= array[index - 1])
			{
				array[index] = prime;
				index++;
				//send to the relevant generator that the
				//prime number was added:
				send_to_gen(queue_id,gen_id,TRUE);
			}
			//if the new prime is smaller than the current maximum in array:
			else
			{
				if (last_unadded == prime)
				{
					failures++;
					//failed to add 100 times new prime numbers to array:
					if (failures == ARR_SIZE)
					{
						break;
					}
				}
				if (last_unadded != prime)
				{
					last_unadded = prime;
					failures = 0;
				}
				//failed to add 100 times new prime numbers to array:
				if (failures == ARR_SIZE)
				{
					break;
				}
				//send to the relevant generator that the
				//prime number was not added:
				send_to_gen(queue_id,gen_id,FALSE);
			}
		}
	}

	//DELETE ME LATER:
	print_array(array, ARR_SIZE);

	//now the array is full or failed 100 times to add new numbers:
	print_stats(array,index,queue_id);

	sleep(1);

	//remove the Queue:
	if(msgctl(queue_id,IPC_RMID,NULL) == -1)
	{
		perror("msgctl failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
int main( )
{
	filler();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------




