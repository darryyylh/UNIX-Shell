#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <time.h>
#include "funcPrototypes.h"
#include <signal.h>
#include <fcntl.h>

/*
	Darryl Hennessy (March 2016).
	
	Compiling and running:
		> gcc main.c functions.c -o shell
		> ./shell

	This .c file contains the implementation of the functions defined
	in the header file funcPrototypes.h
*/


/* Prints the formatted time and date information, along with the '#' prompt. */
void printPrompt() {

	time_t rawtime;
   	struct tm *info;
   	char buffer[15]; // buffer size will never exceed 15 characters.

   	time(&rawtime);
   	info = localtime(&rawtime);

   	strftime(buffer, 15, "%d/%m %H:%M", info);

   	printf("[%s] # ", buffer);
}


/* Function will exit the shell if the user enters 'exit' */
void isExitCmd(char* input) {
	
	if (strcmp(input, "exit") == 0)
		exit(0);
		
}


/* Function forks and executes the command inputted by the user.
   If an unknown command is given, the child process is destroyed,
   and if we can't fork, the shell exits. 
   Appropriate error checking is also performed. */
void executeCmd(char** command, int isRedirectCmd, char* redirectFile) {
	
	pid_t child_pid;
  	int child_status;

	child_pid = fork();

	if (child_pid == 0) {

		// Indicates we are performing a stdout redirect.
		if (isRedirectCmd == 1) {
			int f = open(redirectFile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
			dup2(f, 1);
		}
	
		execvp(command[0], command); 
		printf("Unknown command\n");
		exit(0);
	}

	else if (child_pid == -1) {
		printf("Cannot fork process...exiting\n");
		exit(0);	
	}

	else {
		signal(SIGINT, SIG_IGN);

		//Parent process waits for child to finish.
		wait(&child_status);

		signal(SIGINT, SIG_DFL);
	}
}


/* Function to catch the SIGINT signal */
void signalHandler(int signo) {
	
	if (signo == SIGINT) {
		printf("\n");
		printPrompt();
		fflush(stdout);
	}
}


/* This function checks the user input command and determines whether they requested to
   cd or not. If yes, the function will change the directory. */
int checkForChangeDirectoryCmd(char** command) {

	int changeDirectoryBool = 0;	
	
	// If the first arg is 'cd' we will change directory.
	if (strcmp("cd", command[0]) == 0) {
		
		changeDirectoryBool = 1;

		// if no args are given, change to path given by HOME env var.
		if (command[1] == NULL)
			chdir(getenv("HOME"));

		else {
			if (chdir(command[1]) != 0)
				perror("Error caught");
		}

	}

	return changeDirectoryBool;
}


/* Function checks for the '>' token, meaning we will perform stdout redirect.
   If the token is found, the input is parsed. The command is pulled out, and
   the filename too. Then the command is executed and the output sent to the file. */
int checkForRedirectCmd(char** command) {
	
	int redirectBool = 0;
	int i = 0;

	while (command[i] != NULL) {

		if (strcmp(">", command[i]) == 0) {
			
			redirectBool = 1;

			char* file = command[i + 1];
			char** parsedCmd = malloc(sizeof(char*));
			int argCount = 0;

			// Store the arguments up to '>' in an array, to be used as the command.
			while (argCount < i) {
							
				parsedCmd[argCount] = command[argCount];
				argCount++;

				parsedCmd = realloc(parsedCmd, (argCount + 1) * sizeof(char*));
			}

			// Add a NULL terminator (needed for execvp()).
			parsedCmd = realloc(parsedCmd, (argCount + 1) * sizeof(char*));
			parsedCmd[argCount++] = NULL;
			
			// Execute the command.
			executeCmd(command, 1, file);
		}
		
		// counter for the while loop.		
		i++;
	}
		
	return redirectBool;
}



