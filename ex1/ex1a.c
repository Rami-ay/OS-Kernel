// Author : Rami Ayoub //
// ID : 315478966 //

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
// ---- consts --------
#define ARRAY_SIZE 100000
#define MILLION 1000000

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
//function that gets an array as input , lower index, high index and value
//executes binary search and return -1 if the value is not in the array and the index o.w
int binarySearch(int arr[], int l, int r, int x)
{
	if (r >= l) {
		int mid = l + (r - l) / 2;

		// If the element is present at the middle
		// itself
		if (arr[mid] == x)
			return mid;

		// If element is smaller than mid, then
		// it can only be present in left subarray
		if (arr[mid] > x)
			return binarySearch(arr, l, mid - 1, x);

		// Else the element can only be present
		// in right subarray
		return binarySearch(arr, mid + 1, r, x);
	}

	// We reach here when element is not
	// present in array
	return -1;
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
void do_child(int i, int array[], FILE* file)
{
	//the first child searchs the linear way:
	if (i == 0)
	{
		struct timeval t0, t1;
		//counter to hold how much numbers found:
		int found_counter = 0;
		// repeat the proccess milion times:
		gettimeofday(&t0, NULL);
		for (int j = 0; j < MILLION; j++)
		{
			//generate random number in range [0,1000000):
			int temp_number = rand() % MILLION;
			//now search the number in the given array:
			for (int k = 0; k < ARRAY_SIZE; k++)
			{
				if (array[k] == temp_number)
					found_counter++;
			}
		}
		gettimeofday(&t1, NULL);
		fprintf(file, "s %d %f\n", found_counter, (double)(t1.tv_usec - t0.tv_usec) / 1000000 + (double)(t1.tv_sec - t0.tv_sec));
	}
	//the second child searches the binary way:
	if (i == 1)
	{
		struct timeval t0, t1;
		//counter to hold how much numbers found:
		int found_counter = 0;
		// repeat the proccess milion times:
		gettimeofday(&t0, NULL);
		for (int j = 0; j < MILLION; j++)
		{
			//generate random number in range [0,1000000):
			int temp_number = rand() % MILLION;
			//now search the number in the given array - binary search:
			if (binarySearch(array, 0, ARRAY_SIZE - 1, temp_number) != -1)
				found_counter++;
		}
		gettimeofday(&t1, NULL);
		fprintf(file, "b %d %f\n", found_counter, (double)(t1.tv_usec - t0.tv_usec) / 1000000 + (double)(t1.tv_sec - t0.tv_sec));
	}


}


//--------------------------------------------------------
void father(FILE* file, int number)
{
	struct timeval t0, t1;
	pid_t status;
	char c;
	int array[ARRAY_SIZE];
	float linear_time = 0;
	float binary_time = 0;
	float temp_number = 0;
	int throw = 0;
	gettimeofday(&t0, NULL);
	// starting the srand:
	srand(number);
	//repeat the proccess 10 times:
	for (int time = 0; time < 10; time++)
	{
		//initiating the array :
		//-------------------------
		//starting with array[0]:
		array[0] = (rand() % 10);
		//for each arr[i] insert the prev value + random:
		for (int i = 1; i < ARRAY_SIZE; i++)
		{
			array[i] = array[i - 1] + ((rand() % 10));
		}
		//fork children:
		//---------------
		for (int i = 0; i < 2; i++)
		{
			status = fork();
			check_fork(status);
			if (status == 0)
			{
				do_child(i, array, file);
				exit(EXIT_SUCCESS);
			}
		}
		//wait for both children to finish
		for (int i = 0; i < 2; i++)
		{
			wait(&status);
		}
	}

	//------------------------------
	//now we have a file of 20 rows:
	//------------------------------
	fseek(file, 0, SEEK_SET);
	c = getc(file);
	//calculate the statistics of each search option:
	while (!feof(file))
	{
		//if it is a linear search line:
		if (c == 's')
		{
			getc(file); //throw the first ' '
			fscanf(file, "%d", &throw); //throw the found numbers
			getc(file); // throw the second the second ' '
			fscanf(file, "%f", &temp_number); //store the time
			linear_time = linear_time + temp_number;
			temp_number = 0;
		}
		// if it is a binary search line:
		if (c == 'b')
		{
			getc(file); //throw the first ' '
			fscanf(file, "%d", &throw); //throw the found numbers
			getc(file); // throw the second the second ' '
			fscanf(file, "%f", &temp_number); //store the time
			binary_time = binary_time + temp_number;
			temp_number = 0 ;
		}

		c=getc(file);
	}
		// printing the mean of s an b searches:
		fprintf(file,"%f %f\n",(double)(linear_time / 10),(double)(binary_time / 10));
		gettimeofday(&t1, NULL);
		fprintf(file,"%f\n", (double) (t1.tv_usec - t0.tv_usec) / 1000000 +(double) (t1.tv_sec - t0.tv_sec));






}



//--------------------------------------------------------
int main(int argc, char** argv)

{
	int number;
	FILE* file;
	handle_args(argc, argv);
	//open the input folder:
	file = fopen(argv[1], "r+");
	number = atoi(argv[2]);
	//check if the file opened successfully:
	check_if_opened(file);

	father(file, number);

	fclose(file);
	return (EXIT_SUCCESS);

}
