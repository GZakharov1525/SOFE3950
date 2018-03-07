#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

// Struct representing a single cell in the Sudoku grid
struct cell
{
	int row, col;
	int value;
	bool valid;
};

// Global 2D array representing the solution
struct cell solutions_table[9][9];

// Performs checks on each cell meeting the conditions
//of the game (unique digit in row/col/subgrid
void* validate(void* arg);

int main(int argc, char* argv[])
{
	// Thread ID array
	pthread_t thid[81];
	int counter = 0; //Counter for thread ID array
	// Used for printing conclusion at the end
	bool validSolution = true; 

	// Read in solution
	FILE *inputFile;
	inputFile = fopen("puzzle.txt", "r");
	// Check for error when opening file
	if (inputFile == NULL)
	{
		printf("Could not open solution file.");
		exit(1);
	}

	// Populate solutions table
	for (int row = 0; row < 9; row++)
	{
		for (int column = 0; column < 9; column++)
		{
			// Each digit is a character in the text file,
			//so we need to convert it to an integer.
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

	// Create 81 threads (1 for each cell/square)
	for (int column = 0; column < 9; column++)
	{
		for (int row = 0; row < 9; row++)
		{
			// Adds thread ID to array, performs work on validate(), 
			//takes input of global solution array
			if (pthread_create(&thid[counter], NULL, validate, &solutions_table[row][column]) != 0)
			{
				perror("pthread_create() error");
				exit(1);
			}
			counter++;
		}
	}

	// Join threads, necessary to make sure all threads have time to complete,
	//otherwise the program will terminate before they all finish.
	for (int i = 0; i < 81; i++)
	{
		pthread_join(thid[i], NULL);
	}

	// Print any table items that were invalid (valid member = false)
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

	// Final verdict on given solution
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
	// Convert from void pointer to the data structure you need
	struct cell* test = (struct cell*) arg;
	// Set validity of cell to false;
	test->valid = false;
	// 3 booleans used to check cell validity
	bool subgridValid = true;
	bool rowValid = true;
	bool columnValid = true;

	// For ease of use later
	int testRow = test->row;
	int testCol = test->col;

	// Test a subgrid
	// Calculate start and end indexes
	int rowIndex = -1;
	int colIndex = -1;
	int rowEndIndex;
	int colEndIndex;
	
	// Identify which subgrid cell belongs to
	// Which grid's row
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

	// Which grid's column
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

	// Search through given subgrid
	for (int row = rowIndex; row < rowEndIndex; row++)
	{
		for (int col = colIndex; col < colEndIndex; col++)
		{
			// Value of the cell we are comparing against
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
				// Value is not unique in subgrid
				subgridValid = false;
				puts("subgrid invalid \n");
			}
		}
	}

	// Test a row
	for (int cellCols = 0; cellCols < 9; cellCols++)
	{
		// Value of the cell we're comparing against
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

	// Test a column
	for (int cellRows = 0; cellRows < 9; cellRows++)
	{
		// Value of the cell we're comparing against
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

	// Set cell valid member if all conditions met
	if (subgridValid && rowValid && columnValid)
	{
		test->valid = true;
	}

	// Exit thread, so that pthread_join knows youre done
	pthread_exit(0);
}