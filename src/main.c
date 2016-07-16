#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <time.h>
#include "funcPrototypes.h"
#include <signal.h>

/*
	Darryl Hennessy (March 2016).
	
	Compiling and running:
		> gcc main.c functions.c -o shell
		> ./shell
*/

void shell();

int main() {

	while (!feof(stdin)) {

		shell();
	}

	printf("\n");					
	return 0;				
}


void shell() {
	
	fflush(stdout);

	if (signal(SIGINT, signalHandler) == SIG_ERR)
		printf("\nCan't catch SIGINT\n");			

	char* input = NULL;
	size_t len = 0;

	printPrompt();

	getline(&input, &len, stdin);

	if (feof(stdin)) {
		printf("\n");
		exit(0);
	}

	// remove the trailing newline from the input string.
	strtok(input, "\n");

	// Check for 'exit' command, meaning we should quit the shell.
	isExitCmd(input);

	// Split the input string into an array, and NULL terminate it for passing to execvp().
  	char** command = malloc(sizeof(char*));
  	int count = 0;
  	char* temp = NULL;

  	temp = strtok(input, " ");

	while (temp != NULL) {

		command[count] = temp;
		count++;

		// Add space for next.
		command = realloc(command, (count + 1) * sizeof(char*));

		temp = strtok(NULL, " ");
	}
	
	// Add NULL terminator to the end of the command arguments.
	command = realloc(command, (count + 1) * sizeof(char*));
	command[count++] = NULL;


	// If the command is cd, a different function handles the command.
	if (checkForChangeDirectoryCmd(command) == 1)
		shell();

	// If the command is a redirect, a different function handles the command.
	else if (checkForRedirectCmd(command) == 1)
		shell();
	
	// Otherwise fork and execute the command.
	else	
		executeCmd(command, 0, NULL);

}



