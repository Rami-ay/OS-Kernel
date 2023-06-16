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
#define ARR_SIZE 102
#define NOT_PRIME -1
#define FALSE 0
#define TRUE 1
//global variables:
key_t key;
int shm_id;
int* array;
struct shmid_ds shm_desc;
//--------------------------------------------------------
//this function checks if generating a public key failed:
void check_key(key_t key)
{
	if (key == -1)
	{
		perror("prime_server: ftok() failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function checks if shmget failed:
void check_shm_id(int shm_id)
{
	if (shm_id == -1)
	{
		perror("prime_server: shmget failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function gets a pointer and checks if it is NULL:
void check_attach(int* array)
{
	if (!array)
	{
		perror("prime_server: shmat failed");
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
	if(sig_num == SIGUSR1)
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
		//close the shared memory:
		if (shmctl(shm_id, IPC_RMID, &shm_desc) == -1)
		{
			perror("prime_server: shmctl IPC_RMID failed: ");
			exit(EXIT_FAILURE);
		}
		//exit:
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
void prime_server()
{

	signal(SIGINT, catch_sigint);
	signal(SIGUSR1, catch_sigusr);
	//create a public key for the shared memory:
	key = ftok(".", 'p');
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

	//insert the prime server p_id in array[0]:
	array[0] = getpid();

	//fill the array with zeros:
	for (int i = 1; i < ARR_SIZE; i++)
	{
		array[i] = 0;
	}

	while (1)
	{
		//wait for sigusr1:
		pause();

		//after we got sigusr1 from the client:

		for (int i = 2; i < ARR_SIZE; i++)
		{
			if (array[i] > 0 && is_prime(array[i]) == FALSE)
			{
				array[i] = NOT_PRIME;
			}
		}
		//send SIGUSR1 to the client:
		kill(SIGUSR1, array[1]);
	}

}
//--------------------------------------------------------
int main()
{
	prime_server();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
