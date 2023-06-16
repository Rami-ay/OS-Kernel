// Author : Rami Ayoub //
// ID : 315478966 //


//this program runs in a loop 10 times
// in every iteration it informs the user that it ready
// wait to an input key and Enter pressed
// then gives the user one second to press a natural number > 0.
// after the 10 iterations the program shows the user how many numbers he
// pressed and the maximum value of all the pressed numbers:
// ---- includes ------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
// ---- consts --------
#define ITERATIONS_NUM 10


//--------------------------------------------------------
//function that prints :( if the alarm's time passed:
void catch_alarm(int sig_num)
{
	printf(":(\n");

}
//--------------------------------------------------------
//this function is responsible to control the loop
// and the printing with and to the user:
void controller()
{

	int counter = 0;
	int maximum_num = 0;
	char throw;
	int num;
	struct sigaction act;
	
	act.sa_handler = catch_alarm;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(SIGALRM, &act, NULL);
	
	
	for (int iteration = 0 ; iteration < ITERATIONS_NUM ; iteration++)
	{
		num = 0;
		//inform the user that the program ready:
		printf("Hit enter when ready\n");
		scanf("%c",&throw);

		//start timing 1 sec:
		printf("Enter A Natural Number Greater Than Zero: \n");
		// ---------
		alarm(1);
		//get num from user:
		scanf("%d",&num);

		alarm(0);

		// ----------

		// the user entered a num in under than 1 second:
		if ( num > 0)
		{
			//show the number on screen:
			printf("%d\n",num);
			//Increase the counter of the entered numbers:
			counter++;
			//check if the new number is the maximum:
			if (maximum_num < num)
				maximum_num = num;
		}
	}

	//the 10 iterations finished:
	printf("You managed to enter %d numbers\n",counter);
	printf("The maximum value you entered is: %d\n",maximum_num);


}
//--------------------------------------------------------



int main()
{
	controller();
	return (EXIT_SUCCESS);
}
