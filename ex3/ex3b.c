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
#define ARR_SIZE 10
#define MILLION 1000000
#define NUM_OF_CHILDREN 3
#define TRUE 1
#define FALSE 0
#define SEED 17


// ----- struct ----------
struct Pipes
{
	pid_t son_id;
	int son_pipe[2];
};

//general parameters:
int added_to_array = 0;
//--------------------------------------------------------
void catch_sigusr(int sig_num)
{
	//if we got SIGUSR1 then the father done adding prime numbers
	//to the array:
	if(sig_num == SIGUSR1)
	{
		//now the son prints how many prime numbers
		//he gambled and been added to the array:
		printf("I found %d prime numbers and been added to the array\n",added_to_array);
	}
	exit(EXIT_SUCCESS);
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
//function that gets a number and returns true if it is prime number
//false otherwise:
int is_prime(int number)
{
	if (number == 1)
		return FALSE;

	for (int j = 2; j <= number/2; j++)
	{
		if (number % j == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//--------------------------------------------------------
//this function returns a number in range [0,1M]:
int get_rand_num()
{
	return (rand()%((MILLION -1)+2));
}
//--------------------------------------------------------
void print_array(int array[], int size)
{
	printf("[");
	for(int i=0 ; i < size; i++)
	{
		printf("%d,",array[i]);
	}
	printf("]\n");
}
//--------------------------------------------------------
void do_child(int general_pipe[],struct Pipes son_pipes)
{
	int added = 0;
	pid_t my_id = getpid();
	//declaring the signal handler for SIGUSR1:
	signal(SIGUSR1,catch_sigusr);


	FILE* sons_pipe_read = fdopen(son_pipes.son_pipe[0],"r");
	check_if_opened(sons_pipe_read);
	FILE* general_pipe_write = fdopen(general_pipe[1],"w");
	check_if_opened(general_pipe_write);
	while(1)
	{

		added = 0;
		//store a random number in temp_number:
		int temp_number = get_rand_num();
		//while we are getting non prime numbers, keep gambling new
		//random numbers:
		while(is_prime(temp_number) == FALSE)
		{
			temp_number = get_rand_num();
		}
		//we got a prime number:
		//      -------
		fprintf(general_pipe_write,"%d %d\n",temp_number,my_id);
		//now the son read from it's dedicated pipe if the prime
		//number that he sent is added to the father array:

		//read 1 if the the prime number is added , 0 if not:
		//fscanf(sons_pipe_read,"%d",&added);
		//if the prime number is added , increase the global added counter:
		if(added == 1)
			added_to_array++;

	}

}
//--------------------------------------------------------
void father()
{
	pid_t status;
	int array[ARR_SIZE];
	int maximum = 0, prime, son_id;
	int index = 0;
	int added = FALSE;
	FILE* write_to_child;
	//declaring the signal handler for SIGUSR1:
	signal(SIGUSR1,catch_sigusr);
	//the general pipe that all children write on it:
	int general_pipe_descs[2];
	//making array of struct pipes to hold each son id and
	//its dedicated pip:
	struct Pipes children_pipes[NUM_OF_CHILDREN];
	//opening the pipe:
	if(pipe(general_pipe_descs) == -1)
	{
		fputs("cannot open pipe",stderr);
		exit(EXIT_FAILURE);
	}

	//fork children:
	//---------------
	for (int i = 0; i < NUM_OF_CHILDREN; i++)
	{
		if(pipe(children_pipes[i].son_pipe) == -1)
		{
			fputs("cannot open pipe",stderr);
			exit(EXIT_FAILURE);
		}
		status = fork();
		check_fork(status);
		//i am the father, and i will prepare dedicated pipe
		//for you son:
		if (status > 0)
			children_pipes[i].son_id = status;
		sleep(1);
		//i am the son
		if (status == 0)
		{
			do_child(general_pipe_descs,children_pipes[i]);
			exit(EXIT_SUCCESS);
		}

	}
	//---------------------------------------
	close(STDIN_FILENO);
	dup(general_pipe_descs[0]);
	//---------------------------------------
	//now the father gets prime numbers from the stdin:
	while (index < ARR_SIZE)
	{
		//flag that tell us if the prime is added to the
		//array:
		added = FALSE;
		//store the prime number from pipe to temp param:
		scanf("%d",&prime);
		//store the son id from pipe to temp param:
		scanf("%d",&son_id);

		//the first prime number:
		if (maximum == 0)
			maximum = prime;
		// if the prime >= maximum we add it to the array:
		if (prime >= maximum)
		{
			maximum = prime;
			array[index] = prime;
			index++;
			added = TRUE;
		}
		//find the relevant son in the children pipes,
		//and write if it is added or not:

		for(int i=0;i < NUM_OF_CHILDREN; i++)
		{
			//we found the son:
			if(children_pipes[i].son_id == son_id)
			{
				write_to_child = fdopen(children_pipes[i].son_pipe[1],"w");
				check_if_opened(write_to_child);
				fprintf(write_to_child,"%d ",added);
			}
		}

	}
	//--------------------------------------------------------
	//now the index = ARR_SIZE Which means the array is full:
	//--------------------------------------------------------

	print_array(array,ARR_SIZE);

	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n",index);
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n",array[0]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n",array[ARR_SIZE -1]);

	//KILL CHILDREN PROCESSES USING SIGUSR1:
	for(int i=0 ; i < NUM_OF_CHILDREN ; i++)
	{
		kill(children_pipes[i].son_id,SIGUSR1);
	}

}


//--------------------------------------------------------
int main()
{
	srand(SEED);
	father();
	return (EXIT_SUCCESS);
}
//--------------------------------------------------------
