#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

#include "build_command.h"
#include "run_command.h"
#include "helper_funcs.h"

int parse_input(char **commands, char *input);

int get_input(char *input);

int check_children();

int main() {
		
	int comms, exit;
	int status = 0;
	char input[COMM_SIZE];
	char *comm_list[NUM_COMMS];
	struct Command *command;
	char start_dir[PATH_MAX];
	//save starting directory in buffer
	getcwd(start_dir, PATH_MAX);
	//set signal handler to ignore SIGINT
	signal(SIGINT, SIG_IGN);
	
	do {
		//prevents leftover input from being read in case of signal interrupts
		*input = '\0';
		//prompt for input
		get_input(input);
		//parse input string into array of discrete strings
		comms = parse_input(comm_list, input);
		
		if(comms) {
			//build a struct Command from array
			command = Command_New(comm_list, comms, &status);
			
			//print_command(command);
			//run the command
			exit = run_command(command);
			//free the alloc'd memory
			Command_Destroy(command);
		}
	} while(!exit);
	
	//change back to the starting directory
	chdir(start_dir);
	
	return 0;
}

//check for any terminated child processes
int check_children() {
	
	int pid;
	int status;
	char *pre = "background pid ";
	char *post = " is done: ";
	char num[10];
	int children[255];
	//get list of child process pids
	int total = child_procs(children);
	int i;
	
	for(i = 0; i < total; i++) {
		
		//check for terminated procs
		if((pid = waitpid(children[i], &status, WNOHANG)) > 0) {
			
			//print info
			int_to_str(pid, num, 10);
			print_text(pre);
			print_text(num);
			print_text(post);
			run_status(status);
			
		}
	}	
	return 0;
}

//replaces spaces in input string with nulls, then points **commands elements
//to the address of what are now discrete strings
int parse_input(char **commands, char *input) {
	
	int comms = 0;
	int in_len = strlen(input);
	
	//if input is blank or comment, return 0
	if(!in_len || *input == '#')
		return 0;
	
	//replace spaces with nulls
	while(*input != '\0') {
		
		//add starting address of word to commands
		*commands = input;
		commands++;
		comms++;
		
		//advance address pointer to next space
		while(*input != '\0' && *input != ' ')
			input++;
		
		//replace space with null char and advance to next address, or exit loop if null char
		if(*input == ' ') {
			*input = '\0';
			input++;
		}	
	}
	return comms;
}

//read input from terminal. Uses read() system call to maintain reentrancy
int get_input(char *input) {
	int retval = 0;
	
	size_t len = COMM_SIZE;
	
	//print prompt
	char *prompt = ": ";
	
	//check for terminated child processes
	check_children();
	
	//output prompt
	print_text(prompt);
	
	//read input
	retval = getline(&input, &len, stdin);
	
	//replace newline with null
	if(retval > 0)
		input[retval - 1] = '\0';
	
	return retval;
}