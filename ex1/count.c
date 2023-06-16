// Author : Rami Ayoub //
// ID : 315478966 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 1000


//--------------------------------------------------------
//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <string1> <string2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
int count_occurrence(char first[], char second[])
{
	int i = 0, j = 0;
	int counter = 0;
	for (i = 0; i < strlen(first); i++)
	{
		for (j = 0; j < strlen(second); j++)
		{
			if (first[i] == second[j])
				counter++;
		}
	}
	return counter;

}
//--------------------------------------------------------
int main(int argc, char** argv)
{

	handle_args(argc, argv);
	printf("%d\n", (count_occurrence(argv[1], argv[2])));




	return (EXIT_SUCCESS);
}
