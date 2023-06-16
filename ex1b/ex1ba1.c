// Author : Rami Ayoub //
// ID : 315478966 //


//this program is the father program.
// it gets as an input a name of input file and int for srand
// 50 times , the father fork 2 children , the first which build an array and
// fill it randomly and sort it bubble sort algo.
// the second do the same but using a quick sort algo.
// when the children is done, the father gather the stats from the input file.
// show his stats on screenand remove the input file when done.
// ---- includes ------
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

//--------------------------------------------------------
//this function gets status and check if it is equal to -1 (spawn failure):
void handle_spwanl_error(int status)
{
	if (status == -1)
	{
		perror("_spawnl failed\n");
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
void father(char** argv)
{
	int status;
	long before, after;
	float bubble_time = 0;
	float min_bubble = 0;
	float max_bubble = 0;

	float quick_time = 0;
	float min_quick = 0;
	float max_quick = 0;
	float temp_number = 0;
	int throw = 0;
	char c;
	FILE* file;

	//starting timing before father start:
	before = clock();

	for (int i = 0; i < 5; i++)
	{
		status = _spawnl(_P_WAIT, "D:\\temp\\os-ex1b\\ex1ba2.exe", "ex1ba2.exe", argv[1], argv[2], NULL);
		handle_spwanl_error(status);
		status = _spawnl(_P_WAIT, "D:\\temp\\os-ex1b\\ex1ba3.exe", "ex1ba3.exe", argv[1], argv[2], NULL);
		handle_spwanl_error(status);
	}
	//------------------------------
	//now we have a file of 100 rows:
	//------------------------------
	file = fopen(argv[1], "r+");
	c = getc(file);
	//calculate the statistics of each sort option:
	while (!feof(file))
	{
		//if it is a bubble sort line:
		if (c == 'b')
		{
			getc(file); //throw the first ' '
			fscanf(file, "%f", &temp_number); //store the time
			bubble_time = bubble_time + temp_number;

			//is it a new min?:
			if (min_bubble > temp_number)
				min_bubble = temp_number;

			//is it a new max?:
			if (max_bubble < temp_number)
				max_bubble = temp_number;

			temp_number = 0;
		}
		// if it is a binary search line:
		if (c == 'q')
		{
			getc(file); //throw the first ' '
			fscanf(file, "%f", &temp_number); //store the time
			quick_time = quick_time + temp_number;

			//is it a new min?:
			if (min_quick > temp_number)
				min_quick = temp_number;

			//is it a new max?:
			if (max_quick < temp_number)
				max_quick = temp_number;

			temp_number = 0;
		}
		c = getc(file);
	}
	//printing the average time of bubble sort time and quick sort time:
	printf("%d %d\n", (double)(bubble_time / 50), (double)(quick_time / 50));
	//printing the minimum time of bubble sort time and quick sort time:
	printf("%d %d\n", (double)min_bubble, (double)min_quick);
	//printing the maximum time of bubble sort time and quick sort time:
	printf("%d %d\n", (double)max_bubble, (double)max_quick);

	
	//!!!!!!!!!!!!!!!!!  remove() !!!!!!!!!!!!!!!!!!!!!
	

	//storing the time until the father finish:
	after = clock();

	//printing the father time:
	printf("%f\n", (double)(after - before));

}


//--------------------------------------------------------
int main(int argc, char** argv)
{
	//int number;
	handle_args(argc, argv);
	//FILE* file;
	////open the input folder:
	//file = fopen(argv[1], "a+");
	////check if the file opened successfully:
	//check_if_opened(file);
	//number = atoi(argv[2]);
	//calling the father:

	father(argv);

	return EXIT_SUCCESS;
}