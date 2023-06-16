// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    //for read/write/close
#include <sys/types.h>	//Standard system types
#include <netinet/in.h> //internet address struct
#include <arpa/inet.h>
#include <sys/socket.h> //socket interface functions
#include <netdb.h>
// ---- consts ----
#define MILLION 1000000
#define TRUE 1
#define FALSE 0
//--------------------------------------------------------
//function that gets argc and check if it isn't equal to
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 4)
	{
		fprintf(stderr,"Usage: %s <srand_seed_number> <IP_Adress> <Port_Number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//--------------------------------------------------------
//this function returns a number in range [0,1M]:
int get_rand_num()
{
	return (rand() % ((MILLION - 1) + 2));
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
//this function get the counter of the prime numbers that this
//generator gambled and were added to the array, prints it and closes
//the program
void closing(int added_counter)
{
	printf("I found %d prime numbers and been added to the array\n", added_counter);
	exit(EXIT_SUCCESS);
}
//--------------------------------------------------------
void client(char** argv)
{
	int checker = 0;
	int temp_number = -1,added = 0;
	int added_counter = 0;
	//socket variables:
	int rc; //system calls return value storage
	int main_socket;
	int serving_socket;
	struct addrinfo con_kind,*addr_info_res;

	//connect to the socket:
	//======================================
	memset(&con_kind,0,sizeof(con_kind));
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;

	if((rc = getaddrinfo(argv[2],argv[3],&con_kind,&addr_info_res) != 0))
	{
		fprintf(stderr," Client : getaddrinfo() failed %s\n",gai_strerror(rc));
		exit(EXIT_FAILURE);
	}

	main_socket = socket(addr_info_res->ai_family,addr_info_res->ai_socktype,addr_info_res->ai_protocol);
	if(main_socket < 0)
	{
		perror("Client: socket allocation failed");
		exit(EXIT_FAILURE);
	}

	rc = connect(main_socket,addr_info_res->ai_addr,addr_info_res->ai_addrlen);
	if(rc)
	{
		perror("Client: Connect failed");
		exit(EXIT_FAILURE);
	}
	//======================================

	//Write OK to the server:
	int OK = 1;
	struct sockaddr_storage client_info;
	socklen_t client_info_size = sizeof(client_info);
	serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);

	//write ok to clients:
	write(serving_socket,&OK,1);
	close(serving_socket);
	//read
	read(main_socket,&checker,1);
	//we are expecting to get OK from server:
	if(checker != OK)
	{
		perror("Client: didn't get OK from server");
		exit(EXIT_FAILURE);
	}

	//we got the OK , we start generating numbers in infinite loop:
	while (1)
	{
		//store a random number in temp_number:
		temp_number = get_rand_num();
		//while we are getting non prime numbers, keep gambling new
		//random numbers:
		while (is_prime(temp_number) == FALSE)
		{
			temp_number = get_rand_num();
		}

		//-----------------------
		//we got a prime number:
		//-----------------------

		//sending the number to the server:
		struct sockaddr_storage client_info;
		socklen_t client_info_size = sizeof(client_info);
		serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);

		//write prime to clients:
		write(serving_socket,&temp_number,1);
		close(serving_socket);

		//waiting answer from the server:
		read(main_socket,&added,1);

		//act according to the server answer:
		//------------------------------
		//in case we got 1, then the number was added to the array
		//we increase the counter and gamble new prime number:
		if (added == 1)
			added_counter++;
		//if the generator recieves -1 , then it can be that the array is full
		//or somthing wrong happened and we need to print how many of my primes were
		//added to the array and close the program:
		if (added == -1)
			closing(added_counter);
	}
}

//--------------------------------------------------------
int main(int argc, char** argv)
{
	handle_args(argc, argv);
	srand(atoi(argv[1]));
	client(argv);
	return (EXIT_SUCCESS);
}
//--------------------------------------------------------



