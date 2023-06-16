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

#define ARR_SIZE 100
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
void palindrome_server(char** argv)
{
	int array[ARR_SIZE];
	signal(SIGINT, catch_sigint);
	signal(SIGUSR2, catch_sigusr);
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

		//fill the array with zeros:
		for (int i = 0; i < ARR_SIZE; i++)
		{
			array[i] = 0;
		}

		int index = 0;
		while(index < ARR_SIZE)
		{
			//read
			read(main_socket,array[index],1);

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
	palindrome_server(argv);
	return(EXIT_SUCCESS);
}
//--------------------------------------------------------




