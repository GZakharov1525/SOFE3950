/* Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Georgy Zakharov, Thomas Blain
 * All rights reserved.
 */
#include "utility.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int alloc_mem(Resources res, int size) {
  // Keep track of the starting index
  int startIndex = 0;
  // Leave 64 MBs space for real-time processes using offset
  int offset = (sizeof(res.memory) / sizeof(res.memory[0])) - 64;
  // Start from each position in array and check forwards for
  // sufficient empty space
  int allocation = 0;
  for (int count = 0; count < offset; count++) {
    // Keep track of how much space you have so far
    allocation = 0;
    // Look forwards in the memory to see if you can find the required
    // amount of empty space
    for (int index = count; index < offset; index++) {
      // Check if memory location is free
      if (res.memory[index] == 0) {
        allocation++;
        // If found all needed memory space, move on
        if (allocation == size)
          break;
      }
      // Found an occupied memory position before reaching target
      // amount of memory, reset counter and move onto the next
      // position
      else if (res.memory[index] == 1) {
        allocation = 0;
        break;
      }
    }

    // Verify that required space in memory has been found
    if (allocation == size) {
      startIndex = count;
      break;
    }
  }

  // If having reached the end of the available memory without
  // finding enough free memory, exit with negative value
  if (allocation != size)
    return -1;

  // Mark memory segment as allocated
  for (int reserve = startIndex; reserve < size; reserve++) {
    res.memory[reserve] = 1;
  }

  return startIndex;
}

void free_mem(Resources res, int index, int size) {
  for (int count = index; count < size; count++) {
    res.memory[count] = 0;
  }
}

void load_dispatch(char *dispatch_file, node_t *queue) 
{
  FILE *dispatch_list;
  dispatch_list = fopen(dispatch_file, "r");

  // Check for error when opening file
  if (dispatch_list == NULL) 
  {
    printf("Could not open solution file.");
    exit(1);
  }

  puts("in loader");

  int eof; // Check for end of the file
  Proc newProc; // Declare process to be initialized
  while (eof != EOF) 
  {
	// Read file one line at a time and use it to initialize process
	  // added extra %*c at the end to ignore the \n character that is left over
    eof = fscanf(dispatch_list, "%d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d %*c",
                 &newProc.arrival, &newProc.priority, &newProc.cpuTimeLeft,
                 &newProc.memSize, &newProc.printersReq, &newProc.scannersReq,
                 &newProc.modemsReq, &newProc.cdDrivesReq);

	printf("int %d, EOF %d \n", eof, EOF);

	// Check to make sure data read in was valid
	if (eof == -1)
	{
		//eof = -1;
		puts("reached eof");
	}
	else
	{
		push(queue, newProc);
		puts("created new process: ");
		printf("%d %d %d %d %d %d %d %d \n", newProc.arrival, newProc.priority, newProc.cpuTimeLeft, newProc.memSize,
			newProc.printersReq, newProc.scannersReq, newProc.modemsReq, newProc.cdDrivesReq);
	}
  }
	puts("closing file");
	fclose(dispatch_list);
	puts("closed file");
}
