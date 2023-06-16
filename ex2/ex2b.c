// Author : Rami Ayoub //
// ID : 315478966 //


//in this program we fork a child process,
//both the parent and the child process sleep for random time (2 secs max)
//gambles number in range [0,100] the process who gets 1000 numbers greater than 95 first wins
//the process that gets 60,000 numbers smaller than 55 first loses
// ---- includes ------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
// ---- consts --------
#define WIN_COUNT 1000
#define LOSE_COUNT 60000


int  win_counter = 0 , lose_counter = 0;

//--------------------------------------------------------
void catch_sigusr(int sig_num)
{
	//signal handler check who won and prints the loser flag
	//and vice versa

	if(sig_num == SIGUSR1)
	{
		printf("congrats!\n");

	}
	if(sig_num == SIGUSR2)
	{
		printf("thanks!\n");

	}
	printf("numbers greater or equal to 95 : %d\n",win_counter);
	printf("numbers smaller than 55 : %d\n", lose_counter);

	exit(EXIT_SUCCESS);
}
//--------------------------------------------------------
void do_son()
{
	int temp_num;
	//declaring the signal handler for SIGUSR1:
	signal(SIGUSR1,catch_sigusr);
	//declaring the signal handler for SIGUSR2:
	signal(SIGUSR2,catch_sigusr);

	srand(18);
	while(win_counter != WIN_COUNT && lose_counter != LOSE_COUNT)
	{
		sleep( rand() % 3);
		// gambling random number:
		temp_num = rand() % 101;
		//if the number is greater or equal to 95:
		if(temp_num >= 95)
			win_counter++;
		//if the number is smaller than 55 :
		if(temp_num < 55)
			lose_counter++;
		//if the winning counter is 1,000:

		if(win_counter == WIN_COUNT)
		{
			printf("I win\n");
			kill(getppid(),SIGUSR1);
			exit(EXIT_SUCCESS);
		}
		//if the losing counter is 60,000:
		if(lose_counter == LOSE_COUNT)
		{
			printf("I lose\n");
			kill(getppid(),SIGUSR2);
			exit(EXIT_SUCCESS);
		}
	}


}
//--------------------------------------------------------
void do_father(pid_t son_pid)
{
	int temp_num;
	srand(17);
	//declaring the signal handler for SIGUSR1:
	signal(SIGUSR1,catch_sigusr);
	//declaring the signal handler for SIGUSR2:
	signal(SIGUSR2,catch_sigusr);
	while(win_counter != WIN_COUNT && lose_counter != LOSE_COUNT)
	{
		sleep( rand() % 3);
		// gambling random number:
		temp_num = rand() % 101;

		//if the number is greater or equal to 95:
		if(temp_num >= 95)
			win_counter++;

		//if the number is smaller than 55 :
		if(temp_num < 55)
			lose_counter++;


		//if the winning counter is 1,000:
		if(win_counter == WIN_COUNT)
		{
			printf("I win\n");
			kill(son_pid,SIGUSR1);
			exit(EXIT_SUCCESS);
		}
		//if the losing counter is 60,000:
		if(lose_counter == LOSE_COUNT)
		{
			printf("I lose\n");
			kill(son_pid,SIGUSR2);
			exit(EXIT_SUCCESS);
		}
	}

}
//--------------------------------------------------------
void gamble()
{
	pid_t pid;



	//fork a child:
	pid = fork();
	switch(pid)
	{
	case -1 :
		//failed fork , exit with failure:
		perror("fork() failed\n");
		exit(EXIT_FAILURE);
	case 0 :
		//son is born:
		do_son();
		exit(EXIT_SUCCESS);
	default:
		do_father(pid);
		exit(EXIT_SUCCESS);
	}
}
//--------------------------------------------------------
int main()
{
	gamble();

	return (EXIT_SUCCESS);
}
