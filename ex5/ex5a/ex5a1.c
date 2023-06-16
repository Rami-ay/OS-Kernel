// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
// ---- consts ----
#define ARR_SIZE 105
#define NUM_OF_GEN 3
#define OPENED 1
#define CLOSED 0
//--------------------------------------------------------
//this function checks if generating a public key failed:
void check_key(key_t key)
{
	if (key == -1)
	{
		perror("initiator: ftok() failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function checks if shmget failed:
void check_shm_id(int shm_id)
{
	if (shm_id == -1)
	{
		perror("initiator: shmget failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function gets a pointer and checks if it is NULL:
void check_attach(int* array)
{
	if (!array)
	{
		perror("initiator: shmat failed");
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
//function that prints :( if the alarm's time passed:
void catch_sigusr(int sig_num)
{
	signal(SIGUSR1,catch_sigusr);
	//do nothing!
}
//--------------------------------------------------------
void exec_child(int number)
{
	if (number == 0)
	{
		if (execlp("./ex5a2", "ex5a2", "1", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 1)
	{
		if (execlp("./ex5a2", "ex5a2", "2", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 2)
	{
		if (execlp("./ex5a2", "ex5a2", "3", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
}
//--------------------------------------------------------
void initiator()
{
	key_t key;
	int shm_id, prime_counter = 0, index = 5;
	int* array;
	struct shmid_ds shm_desc;
	pid_t status;
	signal(SIGUSR1,catch_sigusr);

	//create a public key for the shared memory:
	key = ftok(".", '5');
	//check if ftok failed:
	check_key(key);

	//get the identifier for the shared memory:
	shm_id = shmget(key, ARR_SIZE, IPC_CREAT | 0600);
	//check if failed:
	check_shm_id(shm_id);

	//attach to the array pointer:
	array = (int*)shmat(shm_id, NULL, 0);
	//check if the attachment failed:
	check_attach(array);

	//insert the filler proccess id:
	array[0] = getpid();
	//insert zero to the next three elements:
	array[1] = 0;
	array[2] = 0;
	array[3] = 0;
	//Open the key element - array[4]:
	array[4] = OPENED;
	//fill the rest of the array with zeros:
	for (int i = 5; i < ARR_SIZE; i++)
	{
		array[i] = 0;
	}

	// =================================
	// Fork & Executing ex5a2 program which is the number
	// generators program:
	for (int i = 0; i < NUM_OF_GEN; i++)
	{
		status = fork();
		check_fork(status);
		if (status == 0)
		{
			exec_child(i);
			exit(EXIT_SUCCESS);
		}
	}
	// =================================
	//pause until the filler gets a signal
	//that indicates that the generators ended
	// generating and filling the array:

	for (int sig = 0; sig < 3; sig++)
	{
		pause();
	}

	//now we got the signal:
	while (index < ARR_SIZE)
	{
		if (array[index] == 0)
		{
			break;
		}
		else
		{
			prime_counter++;
			index++;
		}
	}

	//DELETE ME LATER:
	print_array(array,ARR_SIZE);

	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n", (index - 5));
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n", array[0]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n", array[index - 1]);


	//close the shared memory:
	if (shmctl(shm_id, IPC_RMID, &shm_desc) == -1)
	{
		perror("initiator: shmctl IPC_RMID failed: ");
		exit(EXIT_FAILURE);
	}
}


//--------------------------------------------------------
int main()
{
	initiator();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
