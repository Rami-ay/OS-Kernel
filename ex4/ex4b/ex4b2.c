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
// ---- consts --------
#define MILLION 1000000
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
//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s <Gen_ID>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function returns a number in range [0,1M]:
int get_rand_num()
{
	return (rand() % ((MILLION - 1) + 2));
}
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
//this function get the counter of the prime numbers that this
//generator gambled and were added to the array, prints it and closes
//the program
void closing(int added_counter)
{
	printf("I found %d prime numbers and been added to the array\n", added_counter);
	exit(EXIT_SUCCESS);
}
//--------------------------------------------------------
void generator(char** argv)
{
	int temp_number = -1;
	int gen_id = atoi(argv[1]);
	int added_counter = 0;
	struct my_msgbuf initiate;

	key_t key;
	int queue_id;

	// ======= Queue OPENING: =========
	//get external Queue ID:
	if((key = ftok(".",'4')) == -1)
	{
		perror("ftok failed");
		exit(EXIT_FAILURE);
	}

	//open the existing queue:
	queue_id = msgget(key,0);
	check_if_open(queue_id);

	//Sending to the filler that the generator is up:
	struct my_msgbuf active;
	active.m_type = gen_id;
	active.m_text[0] = gen_id;
	active.m_text[1] = gen_id;
	if(msgsnd(queue_id,&active,sizeof(active.m_text),0) == -1)
	{
		perror("msgsnd failed");
		exit(EXIT_FAILURE);
	}
	//Receive 1 from the filler - Generator can start looking for primes:
	initiate.m_type = gen_id;
	if(msgrcv(queue_id,&initiate,sizeof(initiate.m_text),initiate.m_type,0) == -1)
	{
		perror("initiate msgrcv failed");
		exit(EXIT_FAILURE);
	}
	if(initiate.m_text[0] != 1)
	{
		perror("Did not get OK from filler");
		exit(EXIT_FAILURE);
	}
	//we got the OK , we start generating numbers in infinite loop:
	while(1)
	{
		//store a random number in temp_number:
		temp_number = get_rand_num();
		//while we are getting non prime numbers, keep gambling new
		//random numbers:
		while (is_prime(temp_number) == FALSE)
		{
			temp_number = get_rand_num();
		}

		//-----------------------
		//we got a prime number:
		//-----------------------

		//Sending to the filler the prime and the gen_id
		struct my_msgbuf my_msg;
		my_msg.m_type = 4;
		my_msg.m_text[0] = temp_number;
		my_msg.m_text[1] = gen_id;
		if(msgsnd(queue_id,&my_msg,sizeof(my_msg.m_text),0) == -1)
		{
			perror("msgsnd failed");
			exit(EXIT_FAILURE);
		}

		//now recieving the filler response, if the sent
		//prime number was added to the array:

		//Receive from the filler if the prime is added or not:
		my_msg.m_type = gen_id;
		if(msgrcv(queue_id,&my_msg,sizeof(my_msg.m_text),my_msg.m_type,0) == -1)
		{
			perror("initiate msgrcv failed");
			exit(EXIT_FAILURE);
		}
		//if the filler sent 1, then the prime was added:
		if (my_msg.m_text[0] == 1)
			added_counter++;
		//if the filler sent -1 , then he done filling the array
		if(my_msg.m_text[0] == -1)
			closing(added_counter);

		//if the filler sent 0 , then the prime was not adde,
		//gamble another prime
	}
}

//--------------------------------------------------------
int main(int argc, char** argv)
{
	handle_args(argc, argv);
	srand(atoi(argv[1]));
	generator(argv);
	return (EXIT_SUCCESS);
}
//--------------------------------------------------------
