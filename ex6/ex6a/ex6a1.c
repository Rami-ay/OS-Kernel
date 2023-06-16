// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    //for read/write/close
#include <sys/types.h>	//Standard system types
#include <netinet/in.h> //internet address struct
#include <arpa/inet.h>
#include <sys/socket.h> //socket interface functions
#include <netdb.h>
// ---- consts ----
#define ARR_SIZE 100
#define NUM_OF_GEN 3
#define DEST_PORT "17171"
#define OK 1
#define ADDED 1
#define NOT_ADDED 0
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
//this function print the prime number counter,min value,max value
//of the array, in addition it send to each generator that the filler
//has stoped(array is full / fialed to add more numbers):
void print_stats(int array[], int index)
{
	//print how much values in the array:
	printf("Prime numbers in array counter: %d\n", index);
	//print the minimum value in the array:
	printf("The MIN value in the array: %d\n", array[0]);
	//print the maximum value in the array:
	printf("The MAX value in the array: %d\n", array[index - 1]);
}
//--------------------------------------------------------
void exec_child(int number)
{
	if (number == 0)
	{
		if (execlp("./ex6a2", "ex6a2", "1","localhost",DEST_PORT, NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 1)
	{
		if (execlp("./ex6a2", "ex6a2", "2","localhost",DEST_PORT, NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
	if (number == 2)
	{
		if (execlp("./ex6a2", "ex6a2", "3","localhost",DEST_PORT, NULL) != 0)
		{
			perror("execlp() failed");
			exit(EXIT_FAILURE);
		}
	}
}
//--------------------------------------------------------
void server()
{
	int check_counter = 0,checker = 0;
	int prime, index = 0, failures = 0, last_unadded = -1;
	int array[ARR_SIZE];
	pid_t status;

	//socket parameters:
	int rc; //return code
	int main_socket;
	int serving_socket;
	struct addrinfo con_kind, * addr_info_res;

	//fill the array with zeros:
	for (int i = 0; i < ARR_SIZE; i++)
	{
		array[i] = 0;
	}

	//starting the socket connection:
	memset(&con_kind,0,sizeof(con_kind));
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE; 	//any IP Address

	if((rc = getaddrinfo(NULL,DEST_PORT,&con_kind,&addr_info_res)) != 0)
	{
		fprintf(stderr,"getaddrinfo() failed %s\n",gai_strerror(rc));
		exit(EXIT_FAILURE);
	}

	main_socket = socket(addr_info_res -> ai_family,addr_info_res -> ai_socktype,
			addr_info_res -> ai_protocol);
	if(main_socket < 0)
	{
		perror("socket: allocation failed");
		exit(EXIT_FAILURE);
	}

	rc = bind(main_socket,addr_info_res-> ai_addr, addr_info_res->ai_addrlen);
	if(rc)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	//start listining to clients , max 3 in line:
	rc = listen(main_socket,3);
	if(rc)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
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

	//getting input from the clients to know that they are
	// alive:
	//expecting 3 OKs:
	for(int i = 0 ; i < NUM_OF_GEN ; i++)
	{
		//struct sockaddr_storage client_info;
		//socklen_t client_info_size = sizeof(client_info);
		//serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);

		//read
		read(main_socket,&checker,1);

		if(checker == OK)
			check_counter++;
	}
	//check if we got NUM_OF_GEN OKs:
	//if ALL clients are alive:
	//now we send a command to clients to start:
	int ok = 1;
	if (check_counter == NUM_OF_GEN)
	{
		for(int i = 0 ; i < NUM_OF_GEN ; i++)
		{
			struct sockaddr_storage client_info;
			socklen_t client_info_size = sizeof(client_info);
			serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);

			//write ok to clients:
			write(serving_socket,&ok,1);
			close(serving_socket);

		}
	}
	//if not all the clients alive:
	else
	{
		perror("NOT ALL CLIENTS ALIVE");
		exit(EXIT_FAILURE);
	}

	//while index did not reach the array size:
	//we fill the array:
	while (index < ARR_SIZE)
	{
		//read new prime number:
		read(main_socket,&checker,1);

		//if the array is empty:
		if (index == 0)
		{
			array[index] = prime;
			index++;
			//CHECK LATER HOW TO SEND THIS, IS THE RELEVANT CLIENT
			//WILL READ MY MESSAGE?

			//send to the relevant generator that the
			//prime number was added:
			//================================================
			//struct sockaddr_storage client_info;
			//socklen_t client_info_size = sizeof(client_info);
			//serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);

			//write ok to clients:
			//write(serving_socket,&ok,1);
			//close(serving_socket);
			//================================================
		}

		//index > 0
		else
		{
			//the new prime is larger or equal to the current maximum in array:
			if (prime >= array[index - 1])
			{
				array[index] = prime;
				index++;
				//send to the relevant generator that the
				//prime number was added:

			}
			//if the new prime is smaller than the current maximum in array:
			else
			{
				if (last_unadded == prime)
				{
					failures++;
					//failed to add 100 times new prime numbers to array:
					if (failures == ARR_SIZE)
					{
						break;
					}
				}
				if (last_unadded != prime)
				{
					last_unadded = prime;
					failures = 0;
				}
				//failed to add 100 times new prime numbers to array:
				if (failures == ARR_SIZE)
				{
					break;
				}
				//send to the relevant generator that the
				//prime number was not added:
			}
		}
		//DELETE ME LATER:
		print_array(array, ARR_SIZE);

		//now the array is full or failed 100 times to add new numbers:
		print_stats(array,index);
	}
	//close the socket:
	freeaddrinfo(addr_info_res);
}

//--------------------------------------------------------
int main()
{
	server();
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------


