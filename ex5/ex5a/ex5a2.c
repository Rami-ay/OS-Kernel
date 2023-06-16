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
#define MILLION 1000000
#define ARR_SIZE 105
#define OPENED 1
#define TRUE 1
#define CLOSED 0
#define FALSE 0
#define KEY 4
//--------------------------------------------------------
//this function checks if generating a public key failed:
void check_key(key_t key)
{
    if(key == -1)
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
    if(!array)
    {
        perror("initiator: shmat failed");
        exit(EXIT_FAILURE);
    }
}
//--------------------------------------------------------
//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s <Gen_ID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
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
void gen_and_fill(int gen_id)
{
    key_t key;
    int shm_id, temp_number = -1, added_counter = 0,
            index = 5,last_unadded = -1,failures = 0;
    int* array;

    //create a public key for the shared memory:
    key = ftok(".",'5');
    //check if ftok failed:
    check_key(key);

    //get the identifier for the shared memory:
    shm_id = shmget(key,ARR_SIZE,0600);
    //check if failed:
    check_shm_id(shm_id);

    //attach to the array pointer:
    array = (int*) shmat(shm_id,NULL,0);
    //check if the attachment failed:
    check_attach(array);

    //insert 1 to array[gen_id]:
    array[gen_id] = OPENED;

    //if all the generators inserted one to their
    //dedicated array items we can start the proccess:

    while(array[1] == 0 || array[2] == 0 || array[3] == 0)
	{
		sleep(1);
	}
	//all generators are ready:
     while(1)
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

    	 //we want to add a prime number to the array,
    	 //we make sure first that the lock is unlocked:
    	 while(array[KEY] == CLOSED)
    		 sleep(1);
    	 //================================================================
    	 //the key is unlocked:
    	 array[KEY] = CLOSED; // close it so other gens can make changes

    	 index = 5;
    	 //find the first index which is not filled yet:
    	 while(array[index] != 0 && index < ARR_SIZE)
    		 index++;

    	 //if the array is full:
    	 if (index == ARR_SIZE)
    	 {
    		 printf("I found %d prime numbers and been added to the array\n", added_counter);
    		 array[KEY] = OPENED;
    		 kill(array[0],SIGUSR1);
    		 exit(EXIT_SUCCESS);
    	 }
    	 //if the array is not filled yet:
    	 if(index == 5)
    	 {
    		 array[index] = temp_number;
    		 added_counter++;
    		 array[KEY] = OPENED;
    	 }
    	 //index > 5
    	 else
    	 {
    		 //the new prime is larger or equal to the current maximum in array:
    		 if (temp_number >= array[index - 1])
    		 {
    			 array[index] = temp_number;
    			 added_counter++;
    			 array[KEY] = OPENED;
    		 }
    		 //if the new prime is smaller than the current maximum in array:
    		 else
    		 {
    			 array[KEY] = OPENED;
    			 if (last_unadded == temp_number)
    			 {
    				 failures++;
    			 }
    			 if (last_unadded != temp_number)
    			 {
    				 last_unadded = temp_number;
    				 failures = 0;
    			 }
    			 //failed to add 100 times new prime numbers to array:
    			 if (failures == ARR_SIZE - 5)
    			 {
    				 printf("I found %d prime numbers and been added to the array\n", added_counter);
    				 kill(array[0],SIGUSR1);
    				 exit(EXIT_SUCCESS);
    			 }
    		 }
    	 }
    	 //================================================================
    	 // open the array so other gens can make changes:
    	 array[KEY] = OPENED;
        }
}
//--------------------------------------------------------
int main(int argc, char** argv)
{
    handle_args(argc, argv);
    int gen_id = atoi(argv[1]);
    srand(gen_id);
    gen_and_fill(gen_id);
    return(EXIT_SUCCESS);
}
//--------------------------------------------------------
