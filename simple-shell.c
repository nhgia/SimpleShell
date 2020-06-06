/***************************************************************
 * Simple UNIX Shell
 * @authors: Nguyen Hoang Gia (1751064) & Pham Bao Duy (1751065)
 * 
 **/


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> //calloc
#include <sys/types.h>
#include <sys/wait.h>


// For clear screen
#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define MAX_LENGTH 80 // The maximum length of the commands

void init_shell() //Open when launch shell
{ 
    clrscr(); 
    printf("\n============================================"); 
    printf("\n|              SIMPLE SHELL                |"); 
    printf("\n____________________________________________"); 
	printf("\n|  Project 2 - CS333 Spring 2020           |"); 
	printf("\n|  Authors: Nguyen Hoang Gia (1751064)     |"); 
	printf("\n|           Pham Bao Duy     (1751063)     |"); 
    printf("\n============================================"); 
	printf("\n"); 
    char* username = getenv("USER"); 
    printf("\nHello @%s, type help for menu.", username); 
    printf("\n\n"); 
} 

void help_panel() 
{ 
    puts("\n____________________________________________"
		 "\n|________________HELP PANEL________________|"
         "\n|__________________________________________|"
         "\n| Some commands supported:                 |"
         "\n|    >!!                                   |"
         "\n|    >ls                                   |"
		 "\n|    >pwd                                  |"
		 "\n|    >ping                                 |"
         "\n|    >exit                                 |"
         "\n|__________________________________________|\n"); 
  
    return; 
} 

void parse(char *command, char **args)
{
	while (*command !='\0') //if not the end of line, continue
	{
		while(*command == ' ' || *command == '\t' || *command == '\n')
		{
			*command++ = '\0'; //replace white spaces with 0
		}
		*args++ = command;
		while (*command != '\0' && *command != ' ' && *command != '\t' && *command != '\n')
		{
			command++;
		}
		*args = '\0'; //mark the end of argument list
	}
}

int findAmpersand(char **args)
{
    //Find "&"
    int count = 0;
    while (args[count] != NULL && strcmp(args[count], "&") != 0)
        count++;
    
    if (args[count] != NULL)
		//Found
        return 1;
    return 0;
}

int findPipe(char **args)
{
    //Find "|""
    int count = 0;
    while (args[count] != NULL && strcmp(args[count], "|") != 0)
        count++;
    
    if (args[count] != NULL)
		//Found
        return 1;
    return 0;
}

void exec_w_Pipe(char** args) 
{ 
	if (findPipe(args) != 1) {
		printf("\nPipe error."); 
		return;
	}
	//Pre-process: Take argument before "|" and after.
	char* parsed[MAX_LENGTH]; 
	char* parsedpipe[MAX_LENGTH]; 

	int countTemp = 0, foundPipe = 0, argLen = 0, pipeLen = 0;
    while (args[countTemp] != NULL) {
		if (strcmp(args[countTemp], "|") == 0)
			foundPipe = 1;
		if (foundPipe == 0) {
			parsed[argLen] = args[countTemp];
			argLen++;
		}
		else if (strcmp(args[countTemp], "|") != 0) {
			parsedpipe[pipeLen] = args[countTemp];
			pipeLen++;
		};
		countTemp++;
	};
    
	parsed[argLen] = '\0';
	parsedpipe[pipeLen] = '\0';

	printf("\n%s",parsed);
	printf("\n%s",parsedpipe);

    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t pipe1, pipe2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nInit error."); 
        return; 
    } 
    pipe1 = fork(); 
    if (pipe1 < 0) { 
        printf("\nFork error."); 
        return; 
    } 
  
    if (pipe1 == 0) { 
        // pipe1
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        pipe2 = fork(); 
  
        if (pipe2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (pipe2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for two children 
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 

void execute(char **args)
{
	pid_t pid;
	int status;

	int isPipe = findPipe(args);
	int isConcurrence = findAmpersand(args);

	if (isPipe == 1) {
		//Execute if find pipe
		exec_w_Pipe(args);
	}
	else if (isConcurrence == 1) {
		//Execute concurrence
	}
	else
	{
		//Single command
		if ((pid = fork()) < 0)
		{
			printf("*** ERROR: forking child process failed\n");
			exit(1);
		}
		else if (pid == 0)
		{
			if (execvp(*args, args) < 0)
			{
				printf("*** ERROR: Exec failed\n");
				exit(1);
			}
		}
		else
		{
			while (wait(&status) != pid)
				printf("...\n");
		}
	}
}

int main(void) {

	char command[MAX_LENGTH];
	char prevCommand[MAX_LENGTH];
	char *args[MAX_LENGTH/2 + 1]; // MAximum 40 argments
	prevCommand[0] = '\0';
	int should_run = 1;
	init_shell();
	while (should_run) {
		printf("ssh>> ");
		fflush(stdout);
		fgets(command, MAX_LENGTH, stdin);
		command[strlen(command)-1] = '\0';
		if (strcmp(command, "") == 0)
			continue;
		else if (strcmp(command, "!!") == 0)
		{
			if (strcmp(prevCommand, "") == 0)
			{
				printf("No commands in history\n");
				continue;
			}
			strncpy(command, prevCommand, strlen(prevCommand) + 1);
		}
		//Parse command and arguments.
		parse(command, args);
		if (strcmp(args[0], "exit") == 0)
			exit(0);
		else if (strcmp(args[0], "help") == 0)
			help_panel();
		else {
			strncpy(prevCommand, command, strlen(command) + 1);
			execute(args);
		}
		//If command contains output redirection argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		open the redirected file in write only mode invoking open() system call
		//		copy the opened file descriptor to standard output file descriptor (STDOUT_FILENO) invoking dup2() system call
		//		close the opened file descriptor invoking close() system call
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//		
		//If command contains input redirection argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		open the redirected file in read  only mode invoking open() system call
		//		copy the opened file descriptor to standard input file descriptor (STDIN_FILENO) invoking dup2() system call
		//		close the opened file descriptor invoking close() system call
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//	
		
		//If command contains pipe argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		create a pipe invoking pipe() system call
		//		fork another child process invoking fork() system call and perform the followings in this child process:
		//			close the write end descriptor of the pipe invoking close() system call
		//			copy the read end  descriptor of the pipe to standard input file descriptor (STDIN_FILENO) invoking dup2() system call
		//			change the process image of the this child with the new image according to the second UNIX command after the pipe symbol (|) using execvp() system call
		//		close the read end descriptor of the pipe invoking close() system call
		//		copy the write end descriptor of the pipe to standard output file descriptor (STDOUT_FILENO) invoking dup2() system call
		//		change the process image with the new process image according to the first UNIX command before the pipe symbol (|) using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//
		//If command does not contain any of the above
		//	fork a child process using fork() system call and perform the followings in the child process.
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
	}
	return 0;
}

