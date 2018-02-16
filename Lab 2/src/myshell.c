/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here

// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };

    // Parse the commands provided using argc and argv

    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        // Perform string tokenization to get the command and argument
        char delimeters[] = " \n";
        char* token = strtok(buffer, delimeters);
        strcpy(command, token);

        while (token) {
          strcpy(arg, token);
          token = strtok(NULL, delimeters);
        }

		//puts("here");

        // Check the command and execute the operations for each command
        // cd command -- change the current directory
        if (strcmp(command, "cd") == 0)
        {
            puts("cd command");
			int report;
			report = chdir(arg);
			if (report != 0)
				puts("Failed to change directory, no such directory found.");
        }
		else if (strcmp(command, "help") == 0)
		{
			//puts("in help");
			FILE *file;
			file = fopen("readme.txt", "r");

			if (file == NULL)
			{
				fputs("Error opening help instructions.", stdout);
			}
			//puts("after file open");
			char readBuffer[100];
			while (fgets(readBuffer, sizeof readBuffer, file) != NULL)
			{
				puts(readBuffer);
			}
			//puts("finished reading file");
		}

        // other commands here...
        else if (strcmp(command, "echo") == 0) {
          printf("\n%s\n", &arg[0]);
        }

        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }

        // Unsupported command
        else
        {
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }

        memset(command, 0, 255);
        memset(arg, 0 , 255);
    }
    return EXIT_SUCCESS;
}
