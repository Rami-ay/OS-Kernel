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
	if (argc != 2)
	{
		fprintf(stderr, "error\n");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
// this function checks if the 2d array allocated successfully:
void CheckAllocation(int* array)
{
	if (array == NULL)
	{
		fputs("Cannot Allocate Memory\n", stderr);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
void find_max_substr(char string[])
{
	int num_of_spaces = 0, num_of_words = 0;
	//max holds the index of the word with highest amount of chars:
	int max_chars = 0;
	int max_index = 0;
	int* counters;
	int* start_positions;
	//temp counter that count how many chars does string has:
	int char_counter = 0;
	//index that runs on the allocated memories:
	int index = 0;
	//index that runs over the given string:
	int i = 0;
	//count how many spaces is there to know how many words we have:
	for (int i = 0; i < strlen(string); i++)
	{
		if (string[i] == ' ')
			num_of_spaces++;
	}

	//allocate memory for counters list and start positions list:
	num_of_words = num_of_spaces + 1;

	//each element in counters holds how many chars the first word containt,second word..etc:
	counters = (int*)malloc(num_of_words * sizeof(int));
	CheckAllocation(counters);

	//each element in start_positions contains what index the substring starts at:
	start_positions = (int*)malloc(num_of_words * sizeof(int));
	CheckAllocation(start_positions);


	while (string[i] != '\0')
	{

		if (string[i] != ' ')
		{
			char_counter++;

		}
		if (string[i] == ' ' || string[i + 1] == '\0')
		{
			counters[index] = char_counter;

			if (string[i + 1] == '\0')
				start_positions[index] = i - char_counter + 1;
			else
				start_positions[index] = i - char_counter;
			char_counter = 0;
			index++;
		}
		i++;
	}
	max_chars = counters[0];
	max_index = 0;
	//now we want to know which word has the highest amount of chars:
	for (int k = 0; k < num_of_words; k++)
	{
		if (counters[k] > max_chars)
		{
			max_chars = counters[k];
			max_index = k;
		}
	}
	//the biggest word starts at string[start_positions[max_index]]:
	i = start_positions[max_index];

	while (string[i] != ' ' && string[i] != '\0')
	{
		printf("%c", string[i]);
		i++;
	}



	free(start_positions);
	free(counters);


}
//--------------------------------------------------------
int main(int argc, char** argv)
{

	handle_args(argc, argv);
	find_max_substr(argv[1]);



	return (EXIT_SUCCESS);
}
