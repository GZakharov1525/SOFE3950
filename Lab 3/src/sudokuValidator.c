#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Prep 2D array for incoming sudoku solution
int sudoku[9][9];

int results[27];
int position = 0;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct arguments
{
	int row;
	int col;
};

void* checkColumn(void* args)
{
	puts("entered checkColumn \n");
	// Array to keep track of which numbers have been encountered
	int check[9] = { -1 };
	// Cast void pointer to the data structure you need
	//int* argPtr = args;
	//int row = argPtr;
	//puts("checkpoint 0 \n");
	int row = *((int *)args);
	//puts("checkpoint 1 \n");

	for (int column = 0; column < 9; column++)
	{
		// Check if number has been found already, if so
		//return value corresponding to an invalid solution.
		// Arrays are zero based so must offset number by 1
		int temp = sudoku[row][column];
		//puts("checkpoint 2 \n");
		if (temp == 0)
		{
			// Ignore zero values
		}
		else if (temp == check[temp - 1])
		{
			// Duplicate number has been found, invalid solution
			//return 0;
			//argPtr = 0;
			//pthread_exit(argPtr);
			//pthread_mutex_lock(&mutex);
			results[position] = 0;
			position++;
			//pthread_mutex_unlock(&mutex);
			puts("--invalid column \n");
			pthread_exit(0);
		}
		else
		{
			// Number has now been encountered for the first time
			//and should be added to list of encountered numbers.
			check[temp - 1] = temp;
		}
		//puts("checkpoint 3 \n");
	}

	// Valid solution, no duplicate numbers found
	//return 1;
	//argPtr = 1;
	//pthread_exit(argPtr);
	//puts("checkpoint 4 \n");
	//pthread_mutex_lock(&mutex);
	results[position] = 1;
	position++;
	//pthread_mutex_unlock(&mutex);

	//puts("exiting checkColumn \n");

	pthread_exit(0);
}

void* checkRow(void* args)
{
	puts("entered checkRow \n");
	// Array to keep track of which numbers have been encountered
	int check[9] = { -1 };
	// Cast void pointer to the data structure you need
	//int* argPtr = args;
	//int column = argPtr;
	int column = *((int *)args);

	for (int row = 0; row < 9; row++)
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
			//return 0;
			//argPtr = 0;
			//pthread_exit(argPtr);
			//pthread_mutex_lock(&mutex);
			results[position] = 0;
			position++;
			//pthread_mutex_unlock(&mutex);
			puts("--invalid row \n");
			pthread_exit(0);
		}
		else
		{
			// Number has now been encountered for the first time
			//and should be added to list of encountered numbers.
			check[temp - 1] = temp;
		}
	}

	// Valid solution, no duplicate numbers found
	//return 1;
	//argPtr = 1;
	//pthread_exit(argPtr);
	//pthread_mutex_lock(&mutex);
	results[position] = 1;
	position++;
	//pthread_mutex_unlock(&mutex);

	//puts("exiting checkRow \n");

	pthread_exit(0);
}

void* checkSubGrid(void* args)
{
	puts("entered checkSubGrid \n");
	int startRow;
	int startCol;
	int endRow;
	int endCol;
	int gridSize = 3;
	int checkGrid[9] = { -1 };
	int subGrid[] = { 0,3,6 };
	//int* argPtr = 0;
	// Cast void pointer to the data structure you need
	int* thread_args = (int*)args;

	// Used to retrieve arguments from pointer
	//int arg1 = thread_args[0];
	//printf("%d ", arg1);
	//int arg2 = thread_args[1];
	//printf("%d \n", arg1);

	struct arguments* test = (struct arguments *) args;
	int arg1 = test->row;
	printf("%d ", arg1);
	int arg2 = test->col;
	printf("%d ", arg2);
	// Variables to determine end index of for loops
	//used to avoid re-calculating index in each iteration
	//of the loop if using 'row <= startIndex + gridSize'.
	startRow = subGrid[arg1];
	startCol = subGrid[arg2];
	endRow = subGrid[arg1] + gridSize;
	endCol = subGrid[arg2] + gridSize;

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
				//return 0;
				//pthread_exit(argPtr);
				//pthread_mutex_lock(&mutex);
				results[position] = 0;
				position++;
				//pthread_mutex_unlock(&mutex);
				puts("INVALID SUBGRID");
				pthread_exit(0);
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
	//return 1;
	//argPtr = 1;
	//pthread_exit(argPtr);
	//pthread_mutex_lock(&mutex);
	results[position] = 1;
	position++;
	//pthread_mutex_unlock(&mutex);

	puts("exited checkSubGrid \n");

	pthread_exit(0);
}

int main(int argc, char* argv[])
{
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

	// Create threads to handle validations
	pthread_t thid[27];
	//int results[27];
	int thread_args[2];
	int counter = 0;
	int thread_vars[9] = { 0,1,2,3,4,5,6,7,8 };
	int subgrid_vars[3] = { 0,1,2 };

	struct arguments run;

	puts("Creating row/col threads \n");

	for (int i = 0; i < 9; i++)
	{
		//int temp = i;
		//int* ptr = &i;
		// Spawns a thread for each row and column
		if (pthread_create(&thid[counter], NULL, checkColumn, &thread_vars[i]) != 0)
		{
			perror("pthread_create() error for columns");
			exit(1);
		}
		//puts("col thread created \n");
		counter++;
		// pthread_create(&thid[counter], NULL, checkRow, (void *)&i or ptr)
		if (pthread_create(&thid[counter], NULL, checkRow, &thread_vars[i]) != 0)
		{
			perror("pthread_create() error for rows");
			exit(1);
		}
		//puts("row thread created \n");
		counter++;
	}

	puts("creating subgrid threads \n");
	// Spawns a thread for each subgrid
	for (int grid = 0; grid <= 2; grid++)
	{
		//printf("grid: %d \n", grid);
		for (int subgrid = 0; subgrid <= 2; subgrid++)
		{
			//printf("subgrid: %d \n", subgrid);
			//thread_args[0] = grid;
			//thread_args[1] = subgrid;
			//pthread_create(&thid[counter], NULL, checkSubGrid, &thread_args)
			//thread_args[0] = subgrid_vars[grid];
			//thread_args[1] = subgrid_vars[subgrid];
			run.row = subgrid_vars[grid];
			run.col = subgrid_vars[subgrid];
			if (pthread_create(&thid[counter], NULL, checkSubGrid, &run) != 0)
			{
				perror("pthread_create() error for subgrids");
				exit(1);
			}
			//puts("created subgrid thread \n");
			counter++;
		}
	}

	// Takes values from pthread_exit and adds them to an array
	//that contains all results from each thread.
	for (int i = 0; i < 27; i++)
	{
		//results[i] = pthread_join(thid[i], NULL);
		pthread_join(thid[i], NULL);
	}

	puts("verifying solution \n");
	for (int j = 0; j < 27; j++)
	{
		printf("%d |", results[j]);
	}

	// Verify that all results passed back from checks are valid
	for (int verify = 0; verify < 27; verify++)
	{
		if (results[verify] == 0)
		{
			printf("Invalid solution. \n");
			exit(1);
		}
	}

	// Close the file before terminating
	fclose(inputFile);
	return 0;
}