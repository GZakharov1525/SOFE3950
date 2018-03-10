/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <Thomas Blain, George Zakharov>
 * All rights reserved.
 *
 */
#include "myshell.h"
#include "utility.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define ARG_NUM 10
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Put global environment variables here
char PWD[BUFFER_LEN];
char SHELL[BUFFER_LEN];
int arg_c;
int redirect = 0;
int using_batch = 0;
FILE *batchfile;

// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
  // Input buffer and and commands
  char buffer[BUFFER_LEN] = {0};
  char command[BUFFER_LEN] = {0};
  char arg[BUFFER_LEN] = {0};

  // Parse the commands provided using argc and argv
  if (argc > 2)
          redirect = 1;
  if (argc == 2)
          using_batch = 1;

  if (redirect == 0)
  {
    printf(ANSI_COLOR_GREEN "%s $ " ANSI_COLOR_RESET, getcwd(PWD, sizeof(PWD)));
  }
  else
  {
    getcwd(PWD, sizeof(PWD));
  }
  getcwd(SHELL, sizeof(SHELL));

  // Attempt to open a file that has been invoked through command line
  if (using_batch)
  {
    batchfile = fopen(argv[1], "r");
    if (batchfile == NULL)
    {
      fputs("Error opening invoked file.", stdout);
    }
  }

  // Perform an infinite loop getting command input from users
  while (1)
  {
    //runtime arg counter
    arg_c = 0;

    //use file stream for commands if batchfile specified
    if (using_batch)
    {
      if (fgets(buffer, BUFFER_LEN, batchfile) == NULL)
      {
        break;
      }
    }

    //use stdin otherwise
    else
    {
      if (fgets(buffer, BUFFER_LEN, stdin) == NULL)
      {
        break;
      }
    }

    // Perform string tokenization to get the command and argument
    char delimeters[] = " \n";
    char *token = strtok(buffer, delimeters);
    strcpy(command, token);

    char *args[ARG_NUM];

    for (int i = 0; i < ARG_NUM; i++)
    {
      args[i] = "";
    }

    int j = 0;

    while (token)
    {
      arg_c++;
      if (strcmp(command, token) != 0)
              args[j++] = token;
      token = strtok(NULL, delimeters);
    }

    // Check the command and execute the operations for each command
    // cd command -- change the current directory
    if (strcmp(command, "cd") == 0)
    {
      getcwd(PWD, sizeof(PWD));
      if (arg_c > 2) {
        char d[BUFFER_LEN] = "";
        for (int i = 0; i < arg_c - 1; i++)
        {
          strcat(d, args[i]);
          if (i < arg_c - 2)
                  strcat(d, " ");
        }
        if (chdir(d) != 0)
        {
          perror("Failed to change directory, the directory may not exist. \n");
        }
      }
      else
      {
        if (chdir(args[0]) != 0)
        {
          perror("Failed to change directory, the directory may not exist. \n");
        }
      }

      if (getcwd(PWD, sizeof(PWD)) == NULL)
      {
        perror("Error in setting PWD. \n");
      }
    }

    else if (strcmp(command, "cwd") == 0)
    {
      if (PWD == NULL)
      {
        // cd command has not been used yet, same directory as
        // shell parent process
        char cwd[256];
        getcwd(cwd, sizeof(cwd));

        printf("Current working directory: %s \n", cwd);
      }
      else
      {
        // cd command has been used before
        printf("Current working directory: %s \n", PWD);
      }
    }
    else if (strcmp(command, "help") == 0)
    {
      // puts("in help");
      FILE *file;
      file = fopen("help.txt", "r");
      if (file == NULL) {
        fputs("Error opening help instructions.", stdout);
      }
      // puts("after file open");
      char readBuffer[100];
      while (fgets(readBuffer, sizeof readBuffer, file) != NULL)
      {
              puts(readBuffer);
      }
      // puts("finished reading file");
    }

    else if (strcmp(command, "environ") == 0)
    {
      printf("PWD: %s\n", PWD);
      printf("SHELL: %s\n", SHELL);
      printf("arg_c: %d\n", arg_c);
      printf("redirect: %d\n", redirect);
      printf("using_batch: %d\n", using_batch);
    }

    // other commands here...
    else if (strcmp(command, "echo") == 0)
    {
      for (int i = 0; i < ARG_NUM; i++)
      {
              printf("%s ", args[i]);
      }
      printf("\n");
    }

    else if (strcmp(command, "clr") == 0)
    {
      printf("\e[1;1H\e[2J");
    }

	else if (strcmp(command, "dir") == 0)
	{
		struct dirent **namelist;
		int n;
		char directory[BUFFER_LEN];
		if (PWD == NULL)
		{
			getcwd(directory, sizeof(directory));
			n = scandir(directory, &namelist, NULL, alphasort);
		}
		else
		{
			n = scandir(PWD, &namelist, NULL, alphasort);
		}

		if (n < 0)
		{
			perror("Scandir()");
		}
		else
		{
			while (n--) // while n is a non-zero number
			{
				printf("%s \n", namelist[n]->d_name);
				free(namelist[n]);
			}
			free(namelist);
		}
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
    memset(arg, 0, 255);
    if (redirect == 0) {
      printf(ANSI_COLOR_GREEN "%s $ " ANSI_COLOR_RESET,
             getcwd(PWD, sizeof(PWD)));
    }
  }
  return EXIT_SUCCESS;
}
