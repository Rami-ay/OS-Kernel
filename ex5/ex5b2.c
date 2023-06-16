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
#define ARR_SIZE 22
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
//function that handles SIGUSR2:
void catch_sigusr(int sig_num)
{
	signal(SIGUSR2, catch_sigusr);
	//do nothing!
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
			perror("Palindrome_server: shmctl IPC_RMID failed: ");
			exit(EXIT_FAILURE);
		}
		//exit:
		exit(EXIT_SUCCESS);
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
//this function gets an array as an input and return 1 if the
//array[2:20]is palindrome(symmetric), o.w it returns 0:
int is_palindrome(int* array)
{
	// if flag == 1 the string is not palindrome
	// if flag == 0 the string is palindrome
	int flag = 0;
	int length = 2;
	for (int i = 2 ; i < 21 ; i++)
	{
		if(array[i] != 0 )
		length++;
	}

	for (int i = 2; i < length; i++)
	{
		//if the integers from left to right != from right to left
		//then the array is not palindrome (symmetric):
		if (array[i] != array[length - i + 1])
		{
			flag = 1;
			break;
		}
	}
	if (flag)
		return FALSE;
	else
		return TRUE;
}
//--------------------------------------------------------
void palindrome_server()
{

	signal(SIGINT, catch_sigint);
	signal(SIGUSR2, catch_sigusr);
	//create a public key for the shared memory:
	key = ftok(".", 'q');
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
	for (int i = 1; i < ARR_SIZE - 1; i++)
	{
		array[i] = 0;
	}

	while (1)
	{
		//wait for sigusr2:
		pause();

		//after we got sigusr2 from the client:
		//we check if the array is symtric and
		//insert the result in array[21]
		array[21] = is_palindrome(array);

		kill(array[1], SIGUSR2);
	}
}
//--------------------------------------------------------
int main()
{
	palindrome_server();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
