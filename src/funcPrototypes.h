/*
	Darryl Hennessy (March 2016).
	
	Compiling and running:
		> gcc main.c functions.c -o shell
		> ./shell

	All function prototypes are listed in this file.
*/


/* Prints the formatted time and date info, along with the '#' prompt. */
void printPrompt();


/* Function will exit the shell if the user enters 'exit' */
void isExitCmd(char* input);


/* Function forks and executes the command inputted by the user.
   If an unknown command is given, the child process is destroyed,
   and if we cannot fork, the shell exits. 
   Appropriate error checking is performed. */
void executeCmd(char** command, int isRedirectCmd, char* redirectFile);


/* Function to catch the SIGINT signal */
void signalHandler(int signo);


/* This function checks the user input command and determines whether they requested to
   cd or not. If yes, the function will change the directory. */
int checkForChangeDirectoryCmd(char** command);


/* Function checks for the '>' token, meaning we will perform stdout redirect.
   If the token is found, the input is parsed. The command is pulled out, and
   the filename too. Then the command is executed and the output sent to the file. */
int checkForRedirectCmd(char** command);



