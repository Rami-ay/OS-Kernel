// Author : Rami Ayoub //
// ID : 315478966 //


//this program is the second gets as input a file name and int for srand.
//declare an array of size 50,000 and fill it randomly
//after that he sort it using quick sort algorithm:
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
//this function sorts ana array using quick sort algo,
//every elemnt in the array is picked and become pivot 
//we devide the array to two unsorted sub arrays with elements
//that holds values less than the pivot sorted to the left:

void quick_sort(int array[], int first, int last) {
    int i, j, pivot, temp;

    if (first < last) {
        pivot = first;
        i = first;
        j = last;

        while (i < j) {
            while (array[i] <= array[pivot] && i < last)
                i++;
            while (array[j] > array[pivot])
                j--;
            if (i < j) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        temp = array[pivot];
        array[pivot] = array[j];
        array[j] = temp;
        quick_sort(array, first, j - 1);
        quick_sort(array, j + 1, last);

    }
}
//--------------------------------------------------------
// the second child function:
//function that gets as input the name of the input file and number to use it
//in srand. builds an array, build it with random values and sort using quick sort
//algo:
void do_child2(FILE* file, int number)
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
	quick_sort(array, 0, ARRAY_SIZE - 1);
	after = clock();

	//printing the stats:
	fprintf(file, "q %f\n", (double)(after - before));


	//closing the file:
	fclose(file);
}
//--------------------------------------------------------
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
	do_child2(file, number);

	return EXIT_SUCCESS;
}