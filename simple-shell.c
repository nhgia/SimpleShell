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
	char *username = getenv("USER");
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
	while (*command != '\0') //if not the end of line, continue
	{
		while (*command == ' ' || *command == '\t' || *command == '\n')
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
	{
		//Found
		args[count] = NULL; // Remove "&" to ensure that fork() will run properly.
		return 1;
	};
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

void exec_w_Pipe(char **args)
{
	if (findPipe(args) != 1)
	{
		printf("\n*** PIPE: error, doing nothing.");
		return;
	}
	//Pre-process: Take argument before "|" and after.
	char *parsed[MAX_LENGTH];
	char *parsedpipe[MAX_LENGTH];

	int countTemp = 0, foundPipe = 0, argLen = 0, pipeLen = 0;
	while (args[countTemp] != NULL)
	{
		if (strcmp(args[countTemp], "|") == 0)
			foundPipe = 1;
		if (foundPipe == 0)
		{
			parsed[argLen] = args[countTemp];
			argLen++;
		}
		else if (strcmp(args[countTemp], "|") != 0)
		{
			parsedpipe[pipeLen] = args[countTemp];
			pipeLen++;
		};
		countTemp++;
	};

	// Mark the end of char array by "end" character
	parsed[argLen] = '\0';
	parsedpipe[pipeLen] = '\0';

	// Define:
	// fd[0] is read end, fd[1] is write end
	int fd[2];
	pid_t pipe1, pipe2;

	if (pipe(fd) < 0)
	{
		printf("\n*** PIPE: Init error.");
		return;
	}
	pipe1 = fork();
	if (pipe1 < 0)
	{
		printf("\n*** PIPE: Fork error.");
		return;
	}

	if (pipe1 == 0)
	{
		// pipe1
		// write at fd[1]
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO); //duplicate
		close(fd[1]);

		if (execvp(parsed[0], parsed) < 0)
		{
			printf("\n*** PIPE: Error when executing command befor pipe.");
			exit(0);
		}
	}
	else
	{
		// Parent execute
		pipe2 = fork();

		if (pipe2 < 0)
		{
			printf("\n*** PIPE: Fork parent error");
			return;
		}

		// Child 2 execute.
		// read at fd[0]
		if (pipe2 == 0)
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO); //duplicate
			close(fd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0)
			{
				printf("\n*** PIPE: Error when executing command after pipe.");
				exit(0);
			}
		}
		else
		{
			// parent executing, waiting for two children
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

	if (isPipe == 1)
	{
		//Execute if find pipe
		exec_w_Pipe(args);
	}
	else
	{
		//Single command
		if ((pid = fork()) < 0) // fork child process
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
			// Execute at parent process
			if (isConcurrence == 1)
			{
				// Found "&"
				//Execute concurrence, immediately go back to parent
				return;
			};

			// Not found "&", waiting for child
			while (pid != wait(&status))
			{
				printf("...\n");
			};
		}
	}
}

int main(void)
{

	char command[MAX_LENGTH];
	char prevCommand[MAX_LENGTH];
	char *args[MAX_LENGTH / 2 + 1]; // MAximum 40 argments
	prevCommand[0] = '\0';
	int should_run = 1;
	init_shell();
	while (should_run)
	{
		printf("ssh>> ");
		fflush(stdout);
		fgets(command, MAX_LENGTH, stdin);
		command[strlen(command) - 1] = '\0';
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
		else if (strcmp(args[0], "clear") == 0)
			clrscr();
		else
		{
			strncpy(prevCommand, command, strlen(command) + 1);
			execute(args);
		}
	}
	return 0;
}
