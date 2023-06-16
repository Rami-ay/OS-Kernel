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
#define MILLION 1000000
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
//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <fifoX> <X> "
			"<fifo2> <fifo3>\n", argv[0]);
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
	int temp_number = -1, OK = 0, added = 0;
	int gen_id = atoi(argv[2]);
	int added_counter = 0;
	// ======= FIFO OPENINGS: =========
	//the general FIFO that all children write on it:
	FILE* fifo0 = fopen(argv[1], "w");
	check_if_opened(fifo0);

	FILE* fifox;
	//Generator FIFO:
	switch (gen_id)
	{
	case 1:
		fifox = fopen("fifo1", "r");
		break;
	case 2:
		fifox = fopen("fifo2", "r");
		break;
	case 3:
		fifox = fopen("fifo3", "r");
		break;
	}
	//check if the FIFO opened successfully:
	check_if_opened(fifox);
	// ================================

	//Sending to the filler that the generator is up:
	fprintf(fifo0, "%d\n", gen_id);
	fflush(fifo0);

	//Receive 1 from the filler - Generator can start looking for primes:
	fscanf(fifox, "%d", &OK);
	//if the received number was not 1:
	if (OK != 1)
	{
		perror("Did not get the OK from filler");
		exit(EXIT_FAILURE);
	}

	//we got the OK , we start generating numbers in infinite loop:
	while (1)
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

		//sending the number to the filler and gen_id:
		fprintf(fifo0, "%d %d\n", temp_number, gen_id);
		fflush(fifo0);
		//now recieving the filler response, if the sent
		//prime number was added to the array:
		fscanf(fifox, "%d", &added);

		//in case 0 recieved , do nothing return to loop.

		//in case we got 1, then the number was added to the array
		//we increase the counter and gamble new prime number:
		if (added == 1)
			added_counter++;
		//if the generator recieves -1 , then it can be that the array is full
		//or somthing wrong happened and we need to print how many of my primes were
		//added to the array and close the program:
		if (added == -1)
			closing(added_counter);
	}
	exit(EXIT_FAILURE);
}
//--------------------------------------------------------
int main(int argc, char** argv)
{
	handle_args(argc, argv);
	srand(atoi(argv[2]));
	generator(argv);
	return (EXIT_SUCCESS);
}
//--------------------------------------------------------
