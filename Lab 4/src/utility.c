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
  int offset = (sizeof(res.memory)/sizeof(res.memory[0])) - 64;
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

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, dispatch_list)) != -1)
  {
    if (read == 1) continue;
    int tempData[9] = {0};
	  int num;


    for (int chars = 0; chars < 9; chars++)
	  {
		  sscanf(line, " %d", &num);
      tempData[chars] = num;
      printf("%d |", tempData[chars]);

      //char data = fgetc(dispatch_list);
      //int toInt = data - '0';
      //tempData[chars] = toInt;
      //// Skip over comma and space
      //fgetc(dispatch_list);
      //fgetc(dispatch_list);
    }
    // Initialize process using array and add to queue
    Proc newArrival = {tempData[0], tempData[1], tempData[2],
                       tempData[3], tempData[4], tempData[5],
                       tempData[6], tempData[7], tempData[8]};
    push(queue, newArrival);
    // Skip newline chars
    //fgetc(dispatch_list);
    //fgetc(dispatch_list);
  }
  printf("done");

  fclose(dispatch_list);
}
