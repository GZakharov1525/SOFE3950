This is the manual for myshell.

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


