// =========== ex0a.c ======================
// ======== Author : Rami Ayoub ============
// ======== ID : 315478966 =================


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// ------ consts -------
#define MAX_FILE_NAME 100
#define MAX_DATA_SIZE 50

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
//funtion that gets pointer to input1,input2,output files and open them
// according to the user files names input
// input1 -> read, input2 ->read, output -> write.
void open_files(FILE* input1, FILE* input2, FILE* output)
{

}
// function that gets as an input 3 file pointers that are opened already
// and closes them
void close_files(FILE* input1, FILE* input2, FILE* output)
{
	fclose(input1);
	fclose(input2);
	fclose(output);
}
//function that gets as an input pointers to two input txt files,
// merge the data inside them and the output of the merge is outputted
// in the output file pointer:
void merge(FILE* input1, FILE* input2, FILE* output)
{
	char name1[MAX_DATA_SIZE], name2[MAX_DATA_SIZE];
	char c, new_line = '\n';
	int used_file = 0;
	//insertng the first names from the input files:
	fscanf(input1, "%s", name1);
	fscanf(input2, "%s", name2);

	//while both files did not reach the end-of-file:
	while (!feof(input1) && !feof(input2))
	{
		used_file = 0;
		//the name from the input1 file comes first acording to ASCII:
		if (strcmp(name1, name2) < 0)
		{
			//printing the name in the output folder:
			fprintf(output, "%s", name1);
			//printing the name on the screen:
			printf("%s", name1);
			//start taking the grades:
			c = getc(input1);
			while (c != '\n')
			{
				//copy the grades into the output folder:
				fprintf(output, "%c", c);
				//copy the grades into the screen:
				printf("%c", c);
				//get the next char:
				c = getc(input1);
			}
			//we reached '\n':
			fprintf(output, "%c", new_line);
			printf("\n");
			used_file = 1;
		}

		//the name from the input2 file comes first acording to ASCII
		if (strcmp(name1, name2) > 0)
		{
			//printing the name in the output folder:
			fprintf(output, "%s", name2);
			//printing the name on the screen:
			printf("%s", name2);
			//start taking the grades:
			c = getc(input2);
			while (c != '\n')
			{
				//copy the grades into the output folder:
				fprintf(output, "%c", c);
				//copy the grades on the screen:
				printf("%c", c);
				//get the next char:
				c = getc(input2);
			}
			//we reached '\n':
			fprintf(output, "%c", new_line);
			printf("\n");
			used_file = 2;
		}

		// we have the same name in both files:
		if (strcmp(name1, name2) == 0)
		{
			//printing the name in the output folder:
			fprintf(output, "%s", name1);
			//printing the name on the screen:
			printf("%s", name1);
			//start taking grades:
			c = getc(input1);
			while (c != '\n')
			{
				//copy the grades into the output folder:
				fprintf(output, "%c", c);
				//copy the grades on the screen:
				printf("%c", c);
				//get the next char:
				c = getc(input1);
			}
			//now we getting grades from the next file:
			c = getc(input2);
			while (c != '\n')
			{
				//copy the grades into the output folder:
				fprintf(output, "%c", c);
				//copy the grades on the screen:
				printf("%c", c);
				//get the next char:
				c = getc(input2);
			}
			//we ended printing the both line, output folder needs '\n':
			fprintf(output, "%c", new_line);
			printf("\n");
			used_file = 3;
		}

		if (used_file == 1)
		{
			//moving to the next line in input1 file:
			fscanf(input1, "%s", name1);
		}
		if (used_file == 2)
		{
			//moving to the next line in input2 file:
			fscanf(input2, "%s", name2);
		}
		if (used_file == 3)
		{
			//moving to the next line in both files:
			fscanf(input1, "%s", name1);
			fscanf(input2, "%s", name2);
		}
	}

	// if the second file reached eof:
	if (feof(input2) && !feof(input1))
	{
		//copy the remaining of input1 file:
		// ------------------------------------
		//printing the name in the output folder:
		fprintf(output, "%s", name1);
		//printing the name on the screen:
		printf("%s", name1);
		c = getc(input1);
		while (!feof(input1))
		{
			//copy the grades into the output folder:
			fprintf(output, "%c", c);
			//copy the grades on the screen:
			printf("%c", c);
			c = getc(input1);
		}
	}

	// if the first file reached eof:
	if (feof(input1) && !feof(input2))
	{
		//copy the remaining of input2 file:
		// ------------------------------------
		//printing the name in the output folder:
		fprintf(output, "%s", name2);
		//printing the name on the screen:
		printf("%s", name2);
		c = getc(input2);
		while (!feof(input2))
		{
			//copy the grades into the output folder:
			fprintf(output, "%c", c);
			//copy the grades on the screen:
			printf("%c", c);
			c = getc(input2);
		}
	}
}

int main()
{
	FILE* input1, * input2, * output;
	char filename[MAX_FILE_NAME];
	// ------ first input file : --------
	puts("Enter first input file name\n");
	//getting the file name from the user:
	scanf("%s", filename);
	//open the file:
	input1 = fopen(filename, "r");
	check_if_opened(input1);

	// ------ second input file : --------
	puts("Enter second input file name\n");
	//getting the file name from the user:
	scanf("%s", filename);
	//open the file:
	input2 = fopen(filename, "r");
	check_if_opened(input2);

	// ------ output file : --------
	puts("Enter output file name\n");
	//getting the file name from the user:
	scanf("%s", filename);
	//open the file:
	output = fopen(filename, "w");
	check_if_opened(output);

	merge(input1, input2, output);

	close_files(input1, input2, output);
	return (EXIT_SUCCESS);
}
