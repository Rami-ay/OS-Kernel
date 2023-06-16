// Author : Rami Ayoub //
// ID : 315478966 //


//this program is the first child gets as input a file name and int for srand.
//declare an array of size 50,000 and fill it randomly
//after that he sort it using bubble sort algorithm:
// ---- includes ------
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
// ---- consts --------
#define ARRAY_SIZE 50000

//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <inp file> <number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
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
//this function takes as input, pointer for two integers
//and swaps them:
void swap(int* first_num, int* second_num)
{
	int temp = *first_num;
	*first_num = *second_num;
	*second_num = temp;
}
//--------------------------------------------------------
// this function takes as input an array and it's size,
// sorting them by swapping every two element the smallest value
// to the left of the array until it's sorted.
void bubble_sort(int aray[], int size)
{
	int i, j;
	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (aray[j] > aray[j + 1])
				swap(&aray[j], &aray[j + 1]);
		}
	}
}
//--------------------------------------------------------
//the first child function:
//function that gets as input the name of the input file and number to use it
//in srand. builds an array, build it with random values and sort using bubble sort
//algo:
void do_child1(FILE* file, int number)
{
	int array[ARRAY_SIZE];
	long before, after;
	

	// starting the srand:
	srand(number);
	//-----------------------
	// initiating the array:
	// ----------------------
	
	//for each arr[i] insert random value:
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		array[i] = rand();
	}
	
	//starting timing before bubble sort:
	before = clock();
	//now we bubble sort the full array:
	bubble_sort(array, ARRAY_SIZE);
	after = clock();

	//printing the stats:
	fprintf(file, "b %f\n", (double)(after - before));

	//closing the file:
	fclose(file);


}

int main(int argc, char** argv)
{
	int number;
	handle_args(argc, argv);
	FILE* file;
	//open the input folder:
	file = fopen(argv[1], "a");
	//check if the file opened successfully:
	check_if_opened(file);
	number = atoi(argv[2]);

	//call the do child func:
	do_child1(file, number);

	return EXIT_SUCCESS;
}