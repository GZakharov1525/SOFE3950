#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct cell
{
	int row, col;
	int value;
	bool valid;
};

struct cell solutions_table[9][9];

void* validate(void* arg);

int main(int argc, char* argv[])
{
	pthread_t thid[81];
	int counter = 0;
	bool validSolution = true;

	FILE *inputFile;
	inputFile = fopen("puzzle.txt", "r");
	// Check for error when opening file
	if (inputFile == NULL)
	{
		printf("Could not open solution file.");
		exit(1);
	}

	for (int row = 0; row < 9; row++)
	{
		for (int column = 0; column < 9; column++)
		{
			// Each digit is a character in the text file
			// Need to convert to integer.
			char charFromFile = fgetc(inputFile);
			int intFromChar = charFromFile - '0';
			solutions_table[row][column].value = intFromChar;
			solutions_table[row][column].row = row;
			solutions_table[row][column].col = column;
			// Debugging line
			printf("%d |", solutions_table[row][column].value);
		}
		// Skip the two formatting characters at the end of each line
		//(probably the '\' and 'n' characters used to make '\n')
		fgetc(inputFile);
		fgetc(inputFile);
		// Debugging line
		printf("\n");
	}

	for (int column = 0; column < 9; column++)
	{
		for (int row = 0; row < 9; row++)
		{
			if (pthread_create(&thid[counter], NULL, validate, &solutions_table[row][column]) != 0)
			{
				perror("pthread_create() error");
				exit(1);
			}
			counter++;
		}
	}

	//Join threads here
	for (int i = 0; i < 81; i++)
	{
		pthread_join(thid[i], NULL);
	}

	//Print any table items that were invalid (valid = false)
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (solutions_table[i][j].valid == false)
			{
				printf("%d %d - Invalid \n", i, j);
				validSolution = false;
			}
		}
	}
	if (validSolution)
	{
		puts("Valid Solution");
	}
	else
	{
		puts("Invalid Solution");
	}

	return 0;
}

void* validate(void* arg)
{
	struct cell* test = (struct cell*) arg;
	test->valid = false;
	bool subgridValid = true;
	bool rowValid = true;
	bool columnValid = true;

	// Easier access
	int testRow = test->row;
	int testCol = test->col;

	//test a subgrid
	int rowIndex = -1;
	int colIndex = -1;
	int rowEndIndex;
	int colEndIndex;
	
	// Identify which subgrid cell belongs to
	if (testRow <= 2)
	{
		rowIndex = 0;
	}
	else if (testRow >= 3 && testRow <= 5)
	{
		rowIndex = 3;
	}
	else if (testRow >= 6)
	{
		rowIndex = 6;
	}

	if (testCol <= 2)
	{
		colIndex = 0;
	}
	else if (testCol >= 3 && testCol <= 5)
	{
		colIndex = 3;
	}
	else if (testCol >= 6)
	{
		colIndex = 6;
	}

	// Indexes to stop at
	rowEndIndex = rowIndex + 3;
	colEndIndex = colIndex + 3;

	// Search through subgrid
	for (int row = rowIndex; row < rowEndIndex; row++)
	{
		for (int col = colIndex; col < colEndIndex; col++)
		{
			int cellValue = solutions_table[row][col].value;
			if (cellValue == 0)
			{
				// Ignore
			}
			else if (row == testRow && col == testCol)
			{
				// Ignore, this cell is the same cell as test
			}
			else if (cellValue == test->value)
			{
				subgridValid = false;
				puts("subgrid invalid \n");
			}
		}
	}

	//test a row
	for (int cellCols = 0; cellCols < 9; cellCols++)
	{
		// Value of the cell we're comparing to
		int cellValue = solutions_table[testRow][cellCols].value;
		if (cellValue == 0)
		{
			// Ignore
		}
		else if (cellCols == testCol)
		{
			// Ignore, this cell is the same cell as test
		}
		else if (test->value == cellValue)
		{
			// Value is not unique in row
			rowValid = false;
			puts("row invalid \n");
		}
	}

	//test a column
	for (int cellRows = 0; cellRows < 9; cellRows++)
	{
		// Value of the cell we're comparing to
		int cellValue = solutions_table[cellRows][testCol].value;
		if (cellValue == 0)
		{
			// Ignore
		}
		else if (cellRows == testRow)
		{
			// Ignore, this cell is the same cell as test
		}
		else if (test->value == cellValue)
		{
			// Value is not unique in column
			columnValid = false;
			puts("column invalid \n");
		}
	}

	//Set cell valid member appropriately
	if (subgridValid && rowValid && columnValid)
	{
		test->valid = true;
	}

	pthread_exit(0);
}