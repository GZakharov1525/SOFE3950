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
		//(probably the '\' and 'n' characters used to make '\n')
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
			//and should be added to list of encountered numbers.
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
			//and should be added to list of encountered numbers.
			check[temp - 1] = temp;
		}
	}

	// Valid solution, no duplicate numbers found
	return 1;
}

int checkSubGrid(int subRow, int subCol)
{
	int startRow;
	int startCol;
	int endRow;
	int endCol;
	int gridSize = 3;
	int checkGrid[9];
	int subGrid[] = { 0,3,6 };

	// Variables to determine end index of for loops
	//used to avoid re-calculating index in each iteration
	//of the loop if using 'row <= startIndex + gridSize'.
	startRow = subGrid[subRow];
	startCol = subGrid[subCol];
	endRow = subGrid[subRow] + gridSize;
	endCol = subGrid[subCol] + gridSize;

	for (int row = startRow; row <= endRow; row++)
	{
		for (int column = startCol; column <= endCol; column++)
		{
			// Check if number has been found already, if so
			//return value corresponding to an invalid solution.
			// Arrays are zero based so must offset number by 1
			int temp = sudoku[row][column];
			if (temp == 0)
			{
				// Ignore zero values
			}
			else if (temp == checkGrid[temp - 1])
			{
				// Duplicate number has been found, invalid solution
				return 0;
			}
			else
			{
				// Number has now been encountered for the first time
				//and should be added to list of encountered numbers.
				checkGrid[temp - 1] = temp;
			}
		}
	}

	// Valid solution, no duplicate numbers found
	return 1;
}