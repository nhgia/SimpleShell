/***************************************************************
 * Simple UNIX Shell
 * @authors: Nguyen Hoang Gia (1751064) & Pham Bao Duy (1751065)
 * 
 **/


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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
         "\n| List of Commands supported:              |"
         "\n|    >!!                                   |"
         "\n|    >!!                                   |"
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

void execute(char **args)
{
	pid_t pid;
	int status;
	
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
	else {
		while (wait(&status) != pid)
			printf("...\n");
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
		printf("ssh>>");
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

