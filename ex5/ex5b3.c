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
#define ARR_SIZE_P 102
#define ARR_SIZE_Q 22
#define FALSE 0
#define TRUE 1
//global variables:
pid_t prime_pid;
pid_t palindrome_pid;

//--------------------------------------------------------
//this function checks if generating a public key failed:
void check_key(key_t key)
{
	if (key == -1)
	{
		perror("front_end: ftok() failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function checks if shmget failed:
void check_shm_id(int shm_id)
{
	if (shm_id == -1)
	{
		perror("front_end: shmget failed");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//this function gets a pointer and checks if it is NULL:
void check_attach(int* array)
{
	if (!array)
	{
		perror("front_end: shmat failed");
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
//function that handles SIGUSR1/SIGUSR2:
void catch_sigusr(int sig_num)
{
	if(sig_num == SIGUSR1)
	{
		//Do nothing!
	}
	if(sig_num == SIGUSR2)
	{
		//Do nothing!
	}
}
//--------------------------------------------------------
//function that handles SIGINT:
void catch_sig_int(int sig_num)
{
	if (sig_num == SIGINT)
	{
		kill(prime_pid, SIGINT);
		kill(palindrome_pid, SIGINT);
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
void exec_child(int number)
{
	//first execute the prime server:
	if (number == 0)
	{
		if (execlp("./ex5b1", "ex5b1", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	//now the palindrome server:
	if (number == 1)
	{
		if (execlp("./ex5b2", "ex5b2", NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
}
//--------------------------------------------------------
void front_end()
{
	char command;
	int temp_num, index_p = 2, index_q = 2;
	pid_t status;

	//variable for prime server shared memory:
	key_t key_p;
	int shm_id_p;
	int* array_p;
	//variable for palindrome server shared memory:
	key_t key_q;
	int shm_id_q;
	int* array_q;

	signal(SIGUSR1, catch_sigusr);
	signal(SIGUSR2, catch_sigusr);
	signal(SIGINT, catch_sig_int);

	// =================================
	// Fork & Executing prime and palindrome servers:
	for (int i = 0; i < 2; i++)
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
	//open the two shared memories:
	// ===========================
	//open the prime server shared_memory:
	///create a public key for the shared memory:
	key_p = ftok(".", 'p');
	//check if ftok failed:
	check_key(key_p);

	//get the identifier for the shared memory:
	shm_id_p = shmget(key_p, ARR_SIZE_P, 0600);
	//check if failed:
	check_shm_id(shm_id_p);

	//attach to the array pointer:
	array_p = (int*)shmat(shm_id_p, NULL, 0);
	//check if the attachment failed:
	check_attach(array_p);

	//insert 1 to array[gen_id]:
	array_p[1] = getpid();
	//store the prime process id:
	prime_pid = array_p[0];
	//			----------

	//now the palindrome shared memory:
	// ===========================
	///create a public key for the shared memory:
	key_q = ftok(".", 'q');
	//check if ftok failed:
	check_key(key_q);

	//get the identifier for the shared memory:
	shm_id_q = shmget(key_q, ARR_SIZE_Q, 0600);
	//check if failed:
	check_shm_id(shm_id_q);

	//attach to the array pointer:
	array_q = (int*)shmat(shm_id_q, NULL, 0);
	//check if the attachment failed:
	check_attach(array_q);

	//insert 1 to array[gen_id]:
	array_q[1] = getpid();
	//store the palindrome process id:
	palindrome_pid = array_q[0];



	// =================================
	while (1)
	{
		//get the desired command from user:
		scanf("%c", &command);

		//if command is p:
		// -------------------
		if (command == 'p')
		{
			//start loading the ints into the array_p:
			scanf("%d", &temp_num);

			while (temp_num != 0 && index_p < ARR_SIZE_P)
			{
				array_p[index_p] = temp_num;
				index_p++;
				scanf("%d", &temp_num);
			}
			//we loaded the data to array_p:
			//now we send SIGUSR1 to the prime server:
			kill(array_p[0], SIGUSR1);
			//now wait to the prime server to send SIGUSR1:
			pause();
			//print the output to the user:
			index_p = 2;
			while (index_p < ARR_SIZE_P)
			{
				if (array_p[index_p] != -1 && array_p[index_p] != 0)
					printf("%d,", array_p[index_p]);
			}
			printf("\n");
		}
		//if command is q:
		// -------------------
		if (command == 'q')
		{
			//start loading the ints into the array_q:
			scanf("%d", &temp_num);

			while (temp_num != 0 && index_q < ARR_SIZE_Q)
			{
				array_q[index_q] = temp_num;
				index_q++;
				scanf("%d", &temp_num);
			}
			//we loaded the data to array_q:
			//now we send SIGUSR2 to the prime server:
			kill(array_q[0], SIGUSR2);
			//now wait to the prime server to send SIGUSR2:
			pause();
			//now print to the user if the array is palindrome:
			if (array_q[ARR_SIZE_Q - 1] == FALSE)
				printf("NOT PALINDROME (SYMTRIC)\n");
			if (array_q[ARR_SIZE_Q - 1] == TRUE)
				printf("ARRAY IS PALINDROME (SYMTRIC)\n");
		}
	}

}




//--------------------------------------------------------
int main()
{
	front_end();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
