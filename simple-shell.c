/**********************************************************************************************************************************************************
 * Simple UNIX Shell
 * @author: 
 * 
 **/


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 80 // The maximum length of the commands

int main(void) {

	char command[MAX_LENGTH];
	char prevCommand[MAX_LENGTH];
	char *args[MAX_LENGTH/2 + 1]; // MAximum 40 argments
	prevCommand[0] = '\0';
	int should_run = 1;

	while (should_run) {
		printf("ssh>>");
		fflush(stdout);
		fgets(command, MAX_LENGTH, stdin);
		command[strlen(command)-1] = '\0';
		if (strcmp(command, "exit") == 0)
		{
			break;
		}
		else if (strcmp(command, "") == 0)
		{
			continue;
		}
		else if (strcmp(command, "!!") == 0)
		{
			if (strcmp(prevCommand, "") == 0)
			{
				printf("No commands in history\n");
				continue;
			}
			strncpy(command, prevCommand, strlen(prevCommand) + 1);
		}
		strncpy(prevCommand, command, strlen(command) + 1);
		//Parse command and arguments.
		const char s[2] = " ";
		int count = 0;
		char *token;
		token = strtok(command, s);
		while(token != NULL)
		{
			args[count++] = token;
			token = strtok(NULL, s);
		}
		/*int c;
		for(c=0; c < count; c++)
			printf("%s\n", args[c]);
		*/	
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

