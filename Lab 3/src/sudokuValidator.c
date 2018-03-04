#include <stdio.h>

int main(int argc, char* argv[])
{
	FILE *inputFile;
	inputFile = fopen("puzzle.txt", "r");
	// Check for error when opening file
	if (inputFile == NULL)
	{
		printf("Could not open solution file.");
	}

	// Prep 2D array for incoming sudoku solution
	int sudoku[9][9];

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