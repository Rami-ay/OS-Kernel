// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
// ---- consts ----
#define ARR_SIZE 101
#define NUM_OF_GEN 3
#define MILLION 1000000
#define TRUE 1
#define OPENED 1
#define CLOSED 0
#define FALSE 0

// ---- globals ----
int array[ARR_SIZE];
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
void* generator()
{
	int temp_number = -1, added_counter = 0 ,index = 1,
			failures = 0;
	while(1)
	{
		index = 1;
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

		//we want to add a prime number to the array,
		//we make sure first that the lock is unlocked:
		while(array[0] == CLOSED)
			sleep(1);

		//================================================================
		//the key is unlocked:
		array[0] = CLOSED; // close it so other gens can't make changes

		while(index < ARR_SIZE && array[index] != 0)
		{
			index++;
		}

		//if we reached the end of the array:
		if (index == ARR_SIZE)
		{
			printf("I found %d prime numbers and been added to the array\n", added_counter);
			//open the array for other generators:
			array[0] = OPENED;
			pthread_exit(NULL);
		}
		else if(index == 1)
		{
			array[index] = temp_number;
			added_counter++;
			array[0] = OPENED;
		}
		//if index > 1:
		else
		{
			//the new prime is larger or equal to the current maximum in array:
			if (temp_number >= array[index - 1])
			{
				array[index] = temp_number;
				added_counter++;
				array[0] = OPENED;
			}
			//if the new prime is smaller than the current maximum in array:
			else
			{
				array[0] = OPENED;
				failures++;
				//failed to add 100 times new prime numbers to array:
				if (failures >= ARR_SIZE - 1)
				{
					printf("I found %d prime numbers and been added to the array\n", added_counter);
					pthread_exit(NULL);
				}
			}
		}

	}
}
//--------------------------------------------------------
//the function that the main thread runs:
void initiator()
{
	pthread_t thread_data[NUM_OF_GEN];
	int vals[NUM_OF_GEN] = {0,1,2};
	int status,j;


	//fill the array zeros:
	for (int i = 0; i < ARR_SIZE ; i++)
	{
		array[i] = 0;
	}

	//creating the threads:
	for(j = 0; j < NUM_OF_GEN ; j++)
	{
		status = pthread_create(&(thread_data[j]),NULL,generator,(void*)&(vals[j]));
		//handle case of creation failure:
		if(status != 0)
		{
			perror("pthread create failed in main");
			exit(EXIT_FAILURE);
		}
	}

	//openening the lock for the threads to start fillin the array:
	array[0] = OPENED;

	//wait for the threads to finish:
	for (int i=0 ; i < NUM_OF_GEN; i++)
	{
		pthread_join(thread_data[i],NULL);
	}


	//printing the stats:
	//===================================================================
	int index = 1;
	while(array[index] != 0 && index < ARR_SIZE)
	{
		index++;
	}


	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n",(index - 1));
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n", array[1]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n", array[index - 1]);
	//===================================================================





}
//--------------------------------------------------------
int main()
{
	srand(17);
	initiator();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
