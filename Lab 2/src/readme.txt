This is the manual for myshell.

myshell is an application that interprets user input by tokenizing user given arguments and processing them to 
invoke code related to each command that is available. The processing is structured through a loop that goes
through a series of check to determine which particular behavior to invoke.

There are three streams for different data, standard input (stdin - 0), standard output (stdout - 1) and 
standard error (stderr - 2). Each stream is responsible for a different job, standard input takes user data to 
pass to a function sometimes taking program data to pass to another part of the program, standard output performs
a similar role but usually taking program data and passing it to a console window to display to the user, sometimes
program data will be passed to another part of the program. Standard error displays any error messages thrown by
the program to the user for debugging purposes. 

It is possible to use special characters to "aim" data from one stream to another, so you can for examplel take
a function's output  and give it to an input stream for another function to do work with. You can also take the
output of a function and send it to the error stream to give the user a specific warning or give the programmer
specific data to help with debugging.

A program environment contains various environment variables which are globally accessible and used during 
execution of the program. Specifically they allow for a running process to ask for the value of a label or 
environment variable like TEMP, HOME or, USERPROFILE, these can be numerical, alphabetical, paths and more.
The program environment also receives signals like the exit status of the Main() function in a program. Some of 
these are set by the system while others can be custom set by a programmer.

myshell is capable of changing directories, viewing their contents, checking current working directory, displaying
help instructions, displaying environment variables, display any user input back to the console window, clear the
screen, pause operation and, quit out of the shell. The shell will also display its current working directory with
after each command.