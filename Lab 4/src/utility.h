/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Georgy Zakharov, Thomas Blain
 * All rights reserved.
 * 
 */
#ifndef UTILITY_H_
#define UTILITY_H_

// The amount of available memory
#define MEMORY 1024

// Resources structure containing integers for each resource constraint and an
// array of 1024 for the memory
typedef struct 
{
	int printersUp;
	int scannersUp;
	int modemsUp;
	int cdDrivesUp;
	int memory[MEMORY];
} Resources;


// Processes structure containing all of the process details parsed from the 
// input file, should also include the memory address (an index) which indicates
// where in the resources memory array its memory was allocated
typedef struct 
{
	int arrival;
	int priority;
	int cpuTimeLeft;
	int memSize;
	int printersReq;
	int scannersReq;
	int modemsReq;
	int cdDrivesReq;
	int memIndex;
} Proc;

// Your linked list structure for your queue
// Moved from queue.h to avoid an 'unknown type name' error
//due to the struct being found by the compiler after its
//first instance of being used.
typedef struct node
{
	Proc process;
	struct node* next;
} node_t;

// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following examples:

// Function to allocate a contiguous chunk of memory in your resources structure
// memory array, always make sure you leave the last 64 values (64 MB) free, should
// return the index where the memory was allocated at
extern int alloc_mem(Resources res, int size);

// Function to free the allocated contiguous chunk of memory in your resources
// structure memory array, should take the resource struct, start index, and 
// size (amount of memory allocated) as arguments
extern void free_mem(Resources res, int index, int size);

// Function to parse the file and initialize each process structure and add
// it to your job dispatch list queue (linked list)
extern void load_dispatch(char *dispatch_file, node_t *queue);


#endif /* UTILITY_H_ */