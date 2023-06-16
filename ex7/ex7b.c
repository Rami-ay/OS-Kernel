// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
// ---- consts ----
#define NUM_OF_THREADS 3
#define ARR_SIZE 100
#define FALSE 0
#define TRUE 1
// ---- globals ----
pthread_t thread_data[NUM_OF_THREADS];
int prime_input[ARR_SIZE];
int prime_output[ARR_SIZE];
int palindrome_array[ARR_SIZE];
//--------------------------------------------------------
void handle_thread_creation(int status)
{
	if (status != 0)
	{
		perror("pthread create failed in main");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function gets an array as an input and return 1 if the
//array[1:99]is palindrome(symmetric), o.w it returns 0:
int is_palindrome(int* array)
{
	// if flag == 1 the string is not palindrome
	// if flag == 0 the string is palindrome
	int flag = 0;
	int length = 0;
	for (int i = 1; i < ARR_SIZE; i++)
	{
		if (array[i] != 0)
			length++;
	}

	for (int i = 1; i < length; i++)
	{
		//if the integers from left to right != from right to left
		//then the array is not palindrome (symmetric):
		if (array[i] != array[length - i + 1])
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
//function that gets a number and returns true if it is prime number
//false otherwise:
int is_prime(int number)
{
	if (number == 0)
		return FALSE;
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
//function that handles SIGUSR1:
void catch_sigusr(int sig_num)
{
	if (sig_num == SIGUSR1)
	{
		//Do nothing!
	}
	else if (sig_num == SIGUSR2)
	{
		//Do nothing!
	}
}
//--------------------------------------------------------
//function that handles SIGINT:
void catch_sigint(int sig_num)
{
	if (sig_num == SIGINT)
	{
		pthread_kill(thread_data[0], SIGINT);
		pthread_kill(thread_data[1], SIGINT);
		pthread_kill(thread_data[2], SIGINT);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
void* prime_server()
{
	signal(SIGUSR1, catch_sigusr);
	signal(SIGINT, catch_sigint);

	while (1)
	{
		//wait for sigusr1:
		pause();

		//after we got sigusr1 from the client:
		for (int i = 0; i < ARR_SIZE; i++)
		{
			if (prime_input[i] != 0)
				prime_output[i] = is_prime(prime_input[i]);
			else
				prime_output[i] = FALSE;
		}
		//send SIGUSR1 to the front_end:
		pthread_kill(thread_data[0], SIGUSR1);
	}

}
//--------------------------------------------------------
void* palindrome_server()
{
	signal(SIGUSR2, catch_sigusr);
	signal(SIGINT, catch_sigint);

	while (1)
	{
		//wait for sigusr2:
		pause();

		//after we got sigusr2 from the client:
		//we check if the array is symtric and
		//insert the result in array[0]
		palindrome_array[0] = is_palindrome(palindrome_array);

		pthread_kill(thread_data[0], SIGUSR2);
	}
}
//--------------------------------------------------------
void* front_end()
{
	char command;
	int temp_num, index_p = 0, index_q = 1;

	signal(SIGUSR1, catch_sigusr);
	signal(SIGUSR2, catch_sigusr);
	signal(SIGINT, catch_sigint);

	//sleep for 2 sec to let the servers end their preps:
	sleep(2);
	// =================================
	while (1)
	{
		//first fill arrays with zeros:
		for (int i = 0; i < ARR_SIZE; i++)
		{
			prime_input[i] = 0;
			prime_output[i] = 0;
		}

		//fill the array with zeros:
		for (int i = 1; i < ARR_SIZE; i++)
		{
			palindrome_array[i] = 0;
		}

		index_p = 0;
		index_q = 1;
		//get the desired command from user:
		scanf("%c", &command);

		//if command is p:
		// -------------------
		if (command == 'p')
		{
			//start loading the ints into the prime_input:
			scanf("%d", &temp_num);

			while (temp_num != 0 && index_p < ARR_SIZE)
			{
				prime_input[index_p] = temp_num;
				index_p++;
				scanf("%d", &temp_num);
			}
			//we loaded the data to prime_input:
			//now we send SIGUSR1 to the prime server:
			pthread_kill(thread_data[1], SIGUSR1);
			//now wait to the prime server to send SIGUSR1:
			pause();
			//print the output to the user:
			index_p = 0;
			while (index_p < ARR_SIZE)
			{
				if (prime_output[index_p] == TRUE)
					printf("%d ", prime_input[index_p]);
				index_p++;
			}
			printf("\n");
		}
		//if command is q:
		// -------------------
		if (command == 'q')
		{
			//start loading the ints into the array_q:
			scanf("%d", &temp_num);

			while (temp_num != 0 && index_q < ARR_SIZE)
			{
				palindrome_array[index_q] = temp_num;
				index_q++;
				scanf("%d", &temp_num);
			}
			//we loaded the data to array_q:
			//now we send SIGUSR2 to the prime server:
			pthread_kill(thread_data[2], SIGUSR2);
			//now wait to the prime server to send SIGUSR2:
			pause();
			//now print to the user if the array is palindrome:
			if (palindrome_array[0] == FALSE)
				printf("NOT PALINDROME (SYMTRIC)\n");
			if (palindrome_array[0] == TRUE)
				printf("ARRAY IS PALINDROME (SYMTRIC)\n");
		}
	}
}
//--------------------------------------------------------
void service_handler()
{
	int status;

	status = pthread_create(&(thread_data[0]), NULL, front_end, NULL);
	//handle case of creation failure:
	handle_thread_creation(status);

	status = pthread_create(&(thread_data[1]), NULL, prime_server, NULL);
	//handle case of creation failure:
	handle_thread_creation(status);

	status = pthread_create(&(thread_data[2]), NULL, palindrome_server, NULL);
	//handle case of creation failure:
	handle_thread_creation(status);

	//wait for the threads to finish:
	for (int i = 0; i < NUM_OF_THREADS; i++)
	{
		pthread_join(thread_data[i], NULL);
	}

	exit(EXIT_SUCCESS);
}
//--------------------------------------------------------
int main()
{
	service_handler();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
