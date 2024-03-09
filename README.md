# Linux Shell

## Introduction
This project is the creation of a command line interface, or otherwise a linux shell built in the C programming language. simple. The shell is capable of changing directories and executing system programs such as `pwd` and `ls`. The application is built using OS/system-related library functions, as it allows for us to understand and manipulate the system it is running on (local computer). Overall, the application encompasses system calls, parsing, search, and directory status.

## Project Structure
Four files are included in this project:
* `Makefile`- contains information used to compile the program with the `make` command. **Do not modify.**
* `shell.h`- includes declarations and specifications for all of the functions in `shell.c`. **Do not modify.**
* `shell.c`- contains function definitions for all functions in `shell.c`.
* `driver.c`- contains the main function, which is just a loop that reads in a command and uses the functions written in `shell.c` to determine whether the command is valid and handle it appropriately.
