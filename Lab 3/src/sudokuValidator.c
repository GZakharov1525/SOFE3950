#include <stdio.h>

// Prep 2D array for incoming sudoku solution
int sudoku[9][9];

int main(int argc, char* argv[])
{
	FILE *inputFile;
	inputFile = fopen("puzzle.txt", "r");
	// Check for error when opening file
	if (inputFile == NULL)
	{
		printf("Could not open solution file.");
	}

	for (int row = 0; row <= 8; row++)
	{
		for (int column = 0; column <= 8; column++)
		{
			// Each digit is a character in the text file
			// Need to convert to digit
			char charFromFile = fgetc(inputFile);
			int intFromChar = charFromFile - '0';
			sudoku[row][column] = intFromChar;
			// Debugging line
			printf("%d |", sudoku[row][column]);
		}
		// Skip the two formatting characters at the end of each line
		fgetc(inputFile);
		fgetc(inputFile);
		// Debugging line
		printf("\n");
	}
	


	// Close the file before terminating
	fclose(inputFile);
	return 0;
}

int checkColumn(int row)
{
	// Array to keep track of which numbers have been encountered
	int check[9];

	for (int column = 0; column <= 8; column++)
	{
		// Check if number has been found already, if so
		//return value corresponding to an invalid solution.
		// Arrays are zero based so must offset number by 1
		int temp = sudoku[row][column];
		if (temp == 0)
		{
			// Ignore zero values
		}
		else if (temp == check[temp - 1])
		{
			// Duplicate number has been found, invalid solution
			return 0;
		}
		else
		{
			// Number has now been encountered for the first time
			check[temp - 1] = temp;
		}
	}

	// Valid solution, no duplicate numbers found
	return 1;
}

int checkRow(int column)
{
	// Array to keep track of which numbers have been encountered
	int check[9];

	for (int row = 0; row <= 8; row++)
	{
		// Check if number has been found already, if so
		//return value corresponding to an invalid solution.
		// Arrays are zero based so must offset number by 1
		int temp = sudoku[row][column];
		if (temp == 0)
		{
			// Ignore zero values
		}
		else if (temp == check[temp - 1])
		{
			// Duplicate number has been found, invalid solution
			return 0;
		}
		else
		{
			// Number has now been encountered for the first time
			check[temp - 1] = temp;
		}
	}

	// Valid solution, no duplicate numbers found
	return 1;
}

int checkBox()
{


	// Valid solution, no duplicate numbers found
	return 1;
}