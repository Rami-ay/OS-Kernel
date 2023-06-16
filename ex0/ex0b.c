// =========== ex0b.c ======================
// ======== Author : Rami Ayoub ============
// ======== ID : 315478966 =================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// ------ consts -------
#define LINE_LENGTH 1000
#define _GNU_SOURCE


//struct decleration:
struct Sentences {
	char** _data;
	int _num_of_sentences;
};
//help functions:
//----------------------
//function that gets argc and check if it isn't equal to 3
//if so it handles the error
void handle_args(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s <inp file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
//function that gets as an input a pointer to I/O file
//and check if the file is opened successfuly:
void check_if_opened(FILE* file)
{
	if (file == NULL)
	{
		puts("cannot open file\n");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
// this function checks if the 2d array allocated successfully:
void CheckAllocation2d(char** data)
{
	if (data == NULL)
	{
		fputs("Cannot Allocate Memory\n", stderr);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
// this function checks if the  array allocated successfully:
void CheckAllocation1d(char* array)
{
	if (array == NULL)
	{
		fputs("Cannot Allocate Memory\n", stderr);
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
// I didn't fount a function that copies string with start and end index so I made one,
// the functions gets destination string, source string, and copies from the given start and end indexes.
void copy_with_limits(char* source, char* destination, int start, int end)
{
	int range, i;
	range = (end - start) + 1;
	for (i = 0; i < range; i++)
	{
		destination[i] = source[start + i];
	}
	destination[range] = '\0';
}
//--------------------------------------------------------
void read_n_allocate(struct Sentences* sentences, FILE* input)
{
	int i = 0;
	int lineLength = 1000;
	size_t line_length = 1000;
	char* temp=NULL;
	char getten_number;
	int number;
	//getting the number of sentences:
	fscanf(input, "%s", &getten_number);
	//convert it to int:
	number = atoi(&getten_number);

	//store the number of sentences in the data struct:
	sentences->_num_of_sentences = number;
	getc(input);

	//allocating memory of size number_of_sentences:
	sentences->_data = (char**)malloc((sentences->_num_of_sentences + 1) * sizeof(char*));
	//check if the allocation succeded:
	CheckAllocation2d(sentences->_data);

	//run over the 2d data and allocate memory for each sentence:
	for (i = 0; i < sentences->_num_of_sentences; i++)
	{
		sentences->_data[i] = (char*)malloc((lineLength + 1) * sizeof(char));
		CheckAllocation1d(sentences->_data[i]);
		// get a line from the user with maximum LINE_LENGTH charachters:
		getline(&temp,&line_length,input);
		strcpy(sentences->_data[i],temp);
	}
	// sets the last sentence to point to NULL so we can know that we reached the end:
	sentences->_data[number] = NULL;
}
//--------------------------------------------------------
// the functions runs on all the sentences char by char, if the current char is not a space (' ')
	// and the next char is a space " " or end of line '\0' then we just ran over a word and the counters sums 1.
int count_words(const struct Sentences* s)
{
	int word_counter = 0;
	int i, j;
	for (i = 0; i < s->_num_of_sentences; i++)
	{
		for (j = 0; s->_data[i][j] != '\0'; j++) // run over the chars in each sentence
		{
			if (s->_data[i][j] != ' ' && s->_data[i][j + 1] == ' ')
				word_counter++;
			if (s->_data[i][j] != ' ' && s->_data[i][j + 1] == '\0')
				word_counter++;
		}
	}
	return word_counter;
}
//--------------------------------------------------------
// this functions gets a sentences structs, runs over it and shows it's content to the screen:
void show_sentences(struct Sentences* s)
{
	int i;
	printf("\n");
	for (i = 0; s->_data[i]; i++)
	{

		printf("%s\n",s->_data[i]);

	}
}
//--------------------------------------------------------
// this function deletes all the allocated memories
void freearray(char** data)
{
	int i;
	//runs over the allocated array and deletes each cell and cell
	for (i = 0; data[i] != NULL; i++)
		{
		free(data[i]);
	}
	free(data); // finally we delete the main allocated memory
}
//--------------------------------------------------------
//this function takes the full stuct that contains the data from the user,
//allocats a new struct that contains the same data but seperated by ' ' .
struct Sentences seperate_sentences(const struct Sentences* full_sentences)
{
	int start = 0, i = 0, j = 0,k=0;
	char temp[LINE_LENGTH];
	char empty_temp[LINE_LENGTH];
	// defining new struct Sentences that contains the seperated words:
	struct Sentences new_sentences;
	// each sentence of the new struct will contain a word.
	// so we calculate how many words in the full sentences struct by using the help function count_words:
	new_sentences._num_of_sentences = count_words(full_sentences);

	// allocating memory size of the number of words:
	new_sentences._data = (char**)malloc((new_sentences._num_of_sentences + 1) * sizeof(char*));
	//check if the allocation succeded:
	CheckAllocation2d(new_sentences._data);

	while (i < full_sentences->_num_of_sentences)
	{
		start = 0;
		j = 0;
		while (full_sentences->_data[i][j] != '\0')
		{
			//if we reach ' ' or '\0' then we found a word and we want to store it into the new struct:
			if (full_sentences->_data[i][j + 1] == ' ' || full_sentences->_data[i][j + 1] == '\0')
			{
				//allocate memory size of (j- start) which means the length of the word found:
				new_sentences._data[k] = (char*)malloc((j - start + 2) * sizeof(char));
				CheckAllocation1d(new_sentences._data[k]);
				//copy the word from the start to j, into temp:
				copy_with_limits(full_sentences->_data[i], temp, start, j);
				//copy the word from the start to \0 , into the new_sentences:
				copy_with_limits(temp, new_sentences._data[k], 0, strlen(temp + 1));
				// reseting the temp string:
				copy_with_limits(empty_temp, temp, 0, LINE_LENGTH);
				k++;
				if (full_sentences->_data[i][j + 1] != '\0') // if we did not reach the end
				{
					j = j + 2;
					start = j;
				}
				else // if we reached the end , j++ then the internal while loop reaches a stop condition and stops
				{
					j = j + 1;
					start = j;
				}
			}
			else
				j++;
		}
		i++;
	}
	show_sentences(&new_sentences); // show the new struct that containt seperated words to the screen
	return new_sentences; // returning the new struct as requested
}

int main(int argc,char **argv)
{
	struct Sentences s, seperated_s;
	FILE* input;
	//check if the argc is valid:
	handle_args(argc,argv);
	//open the input folder:
	input = fopen(argv[1], "r");
	//check if the file opened successfuly:
	check_if_opened(input);
	//allocate memory for the data from the input file:
	read_n_allocate(&s, input);
	//seperate the given data by words:
	seperated_s = seperate_sentences(&s);
	//free the allocated memories:
	freearray(s._data);
	freearray(seperated_s._data);

	return EXIT_SUCCESS;
}
