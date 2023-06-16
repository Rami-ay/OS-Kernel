// Author : Rami Ayoub //
// ID : 315478966 //

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h> //socket interface functions
#include <unistd.h>    //for read/write/close
#include <sys/types.h>	//Standard system types
#include <netinet/in.h> //internet address struct
// ---- consts ----
#define ARR_SIZE 100
#define NOT_PRIME -1
#define MAX_MSG_LEN 1000
#define FALSE 0
#define TRUE 1
//--------------------------------------------------------
//function that gets argc and check if it isn't equal to
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr,"Usage: %s <IP_Adress> <Port_Number>\n", argv[0]);
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

		//exit:
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
void prime_server( char** argv)
{
	char temp_str[MAX_MSG_LEN];
	char* space = " ";
	char* result_string;
	int number;
	//socket parameters:
	int rc; //return code
	int main_socket;
	int serving_socket;
	struct addrinfo con_kind, * addr_info_res;

	//starting the socket connection:
	memset(&con_kind,0,sizeof(con_kind));
	con_kind.ai_family = AF_UNSPEC;
	con_kind.ai_socktype = SOCK_STREAM;
	con_kind.ai_flags = AI_PASSIVE; 	//any IP Address

	if((rc = getaddrinfo(NULL,argv[2],&con_kind,&addr_info_res)) != 0)
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


	//read series of prime numbers:
	//start the loop:
	while(1)
	{
		//read
		read(main_socket,&temp_str,MAX_MSG_LEN);

		//check the primes:

		// --------------------------------------------------
		//split the string to tokens splitted by " " (space):
		// --------------------------------------------------

		//split the first token:
		char* token = strtok(temp_str," ");
		//while the string still has numbers in it, keep spliting by
		//spaces:
		while(token != NULL)
		{
			//convert the number to integer:
			number = atoi(token);
			//if the number is prime:
			if(is_prime(number) == TRUE)
			{
				//add it to the result string:
				strcat(result_string,token);
				//add a space to the result string:
				strcat(result_string,space);
			}
			token = strtok(NULL, " ");
		}

		// --------------------------------------------------------------
		//now we want to write back the result string to the socket:
		// --------------------------------------------------------------
		struct sockaddr_storage client_info;
		socklen_t client_info_size = sizeof(client_info);
		serving_socket = accept(main_socket,(struct sockaddr*)&client_info,&client_info_size);


		write(serving_socket,&result_string,strlen(result_string));
		close(serving_socket);




	}
}


//--------------------------------------------------------
int main(int argc, char** argv)
{
	handle_args(argc,argv);
	prime_server(argv);
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------
