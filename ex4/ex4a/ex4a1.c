// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// ---- consts --------
#define ARR_SIZE 100
#define NUM_OF_GEN 3
#define TRUE 1
#define FALSE 0

//--------------------------------------------------------
//function that gets as an input a pointer to I/O file
//and check if the file is opened successfuly:
void check_if_opened(FILE* file)
{
	if (file == NULL)
	{
		puts("cannot open file\n");
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
//function that gets argc and check if it isn't equal to 5
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 5)
	{
		fprintf(stderr, "Usage : %s <fifo0> <fifo1> "
			"<fifo2> <fifo3>\n", argv[0]);
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
//this function take all the fifo files, and the generator's ID, and the message
//desired to be sent to the generator, and prints the message on the relevant fifo:
void send_to_gen(FILE* fifo1, FILE* fifo2, FILE* fifo3, int gen_id, int message)
{
	if (gen_id == 1)
	{
		fprintf(fifo1, "%d\n", message);
		fflush(fifo1);
	}

	if (gen_id == 2)
	{
		fprintf(fifo2, "%d\n", message);
		fflush(fifo2);
	}

	if (gen_id == 3)
	{
		fprintf(fifo3, "%d\n", message);
		fflush(fifo3);
	}
}
//--------------------------------------------------------
//this function print the prime number counter,min value,max value
//of the array, in addition it send to each generator that the filler
//has stoped(array is full / fialed to add more numbers):
void print_stats(int array[], int index, FILE* fifo0, FILE* fifo1,
	FILE* fifo2, FILE* fifo3)
{
	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n", index);
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n", array[0]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n", array[index - 1]);

	//now we send to each generator command to print his stats:
	send_to_gen(fifo1, fifo2, fifo3, 1, -1);
	send_to_gen(fifo1, fifo2, fifo3, 2, -1);
	send_to_gen(fifo1, fifo2, fifo3, 3, -1);
}
//--------------------------------------------------------
void filler(char** argv)
{
	int checker = 0, check1 = 0, check2 = 0, check3 = 0;
	int prime, gen_id, index = 0, failures = 0, last_unadded = -1;
	int array[ARR_SIZE];
	pid_t status;


	// Fork & Executing ex4a2 program which is the number
	// generators program:
	for (int i = 0; i < NUM_OF_GEN; i++)
	{
		status = fork();
		check_fork(status);
		if (status == 0)
		{
			if (i == 0)
			{
				if (execlp("./ex4a2", "ex4a2", argv[1], "1", NULL) != 0)
				{
					perror("execlp() failed");
					exit(EXIT_FAILURE);
				}
			}
			if (i == 1)
			{
				if (execlp("./ex4a2", "ex4a2", argv[1], "2", NULL) != 0)
				{
					perror("execlp() failed");
					exit(EXIT_FAILURE);
				}
			}
			if (i == 2)
			{
				if (execlp("./ex4a2", "ex4a2", argv[1], "3", NULL) != 0)
				{
					perror("execlp() failed");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	// ======= FIFO OPENINGS: =========
	//the general FIFO that all children write on it:
	FILE* fifo0 = fopen(argv[1], "r");
	check_if_opened(fifo0);
	//First Generator FIFO:
	FILE* fifo1 = fopen(argv[2], "w");
	check_if_opened(fifo1);
	//Second Generator FIFO:
	FILE* fifo2 = fopen(argv[3], "w");
	check_if_opened(fifo2);
	//Third Generator FIFO:
	FILE* fifo3 = fopen(argv[4], "w");
	check_if_opened(fifo3);
	// ================================

	//checking that all generators are up:
	for (int i = 0; i < NUM_OF_GEN; i++)
	{
		fscanf(fifo0, "%d", &checker);
		//check which generator id we got:
		switch (checker)
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
		//print on each gen fifo 1 - command to start:
		fprintf(fifo1, "1\n");
		fflush(fifo1);
		fprintf(fifo2, "1\n");
		fflush(fifo2);
		fprintf(fifo3, "1\n");
		fflush(fifo3);
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
		//read from the main fifo a prime number:
		fscanf(fifo0, "%d", &prime);
		//read from main fifo, the generator ID:
		fscanf(fifo0, "%d", &gen_id);

		//if the array is empty:
		if (index == 0)
		{
			array[index] = prime;
			index++;
			//send to the relevant generator that the
			//prime number was added:
			send_to_gen(fifo1, fifo2, fifo3, gen_id, TRUE);
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
				send_to_gen(fifo1, fifo2, fifo3, gen_id, TRUE);
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
				send_to_gen(fifo1, fifo2, fifo3, gen_id, FALSE);
			}
		}
	}

	//DELETE ME LATER:
	print_array(array, ARR_SIZE);

	//now the array is full or failed 100 times to add new numbers:
	print_stats(array, index, fifo0, fifo1, fifo2, fifo3);

	sleep(1);
	//close all FIFOs:
	fclose(fifo0);
	fclose(fifo1);
	fclose(fifo2);
	fclose(fifo3);
}
//--------------------------------------------------------
int main(int argc, char** argv)
{
	handle_args(argc, argv);
	filler(argv);
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
