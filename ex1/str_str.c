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
		fprintf(stderr, "error\n");
		exit(EXIT_FAILURE);
	}
}
//--------------------------------------------------------
int find_first_occurence(char str1[], char str2[])
{
	int str1_lenght = strlen(str1);
	char temp[str1_lenght];
	int i = 0;
	//run over str2:
	while (str2[i] != '\0' && i < (strlen(str2) - strlen(str1) + 1))
	{
		//if the current char in str2 equals to str1[0]
		if (str2[i] == str1[0])
		{
			//copy the suspected substring to temp char:
			for (int j = 0; j < str1_lenght; j++)
			{
				temp[j] = str2[i + j];
			}
			//check if the two strings are equal:
			if (strcmp(temp, str1) == 0)
				return i;
		}
		i++;
	}
	return -1;

}
//--------------------------------------------------------
int main(int argc, char** argv)
{

	handle_args(argc, argv);
	printf("%d\n", find_first_occurence(argv[1], argv[2]));



	return (EXIT_SUCCESS);
}
