/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
//#define MEMORY 1024 // Defined in utility.h

// Put global environment variables here

// Define functions declared in hostd.h here

void print_process(Proc process) {
	printf("%d\n", process.cpuTimeLeft);
}

int main(int argc, char *argv[])
{
    // Load the dispatchlist
	node_t* test = NULL;
	node_t* priorityQueue0 = NULL;
	node_t* priorityQueue1 = NULL;
	node_t* priorityQueue2 = NULL;
	node_t* priorityQueue3 = NULL;

	if (argc > 1)
	{
		load_dispatch(argv[1], test);
		printf("%d", test->process.priority);
		printf("A");
	}

	Resources res;

	while(1) {
		 node_t* current = NULL;
		 while ((current = pop(test)) != NULL) {
			 if (current->process.priority == 0)
			 	push(priorityQueue0, current->process);
			 else if (current->process.priority == 1)
			 	push(priorityQueue1, current->process);
			 else if (current->process.priority == 2)
			 	push(priorityQueue2, current->process);
			 else
			 	push(priorityQueue3, current->process);
		 }



	 // Allocate the res for each process before it's executed
		 node_t* priority;
		 pid_t pid;
		 if (priorityQueue0) {
			 priority = pop(priorityQueue0);
		 } else if (priorityQueue1) {
			 priority = pop(priorityQueue1);
		 } else if (priorityQueue2) {
			 priority = pop(priorityQueue2);
		 } else if (priorityQueue3) {
			 priority = pop(priorityQueue3);
		 } else {
			 break;
		 }

		 // alloc_mem(res, priority->process);

		 // Execute the process binary using fork and exec
		 pid = fork();

		 if (pid == 0) {
			 print_process(priority->process);
			 res.printersUp -= priority->process.printersReq;
			 res.scannersUp -= priority->process.scannersReq;
			 res.modemsUp -= priority->process.modemsReq;
			 res.cdDrivesUp-= priority->process.cdDrivesReq;
			 int time = rand() % priority->process.cpuTimeLeft;
			 wait(&time);
			 priority->process.cpuTimeLeft -= time;
			 res.printersUp += priority->process.printersReq;
			 res.scannersUp += priority->process.scannersReq;
			 res.modemsUp += priority->process.modemsReq;
			 res.cdDrivesUp += priority->process.cdDrivesReq;
			 push(test, priority->process);
			 kill(pid, SIGKILL);
		 } else {
			 wait(NULL);
		 }

	}

    return EXIT_SUCCESS;
}
