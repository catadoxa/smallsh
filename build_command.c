#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "build_command.h"

//constructor
struct Command *Command_New(char **commands, int comms, int *status) {
	
	//keeps track of how many background and file redirect indicators are in commands
	int i = 0;
	//malloc space for the command
	struct Command *command = malloc(sizeof(struct Command));
	
	//all commands point to same status integer
	command->status = status;
	
	//assign built_in field
	if(strcmp(commands[0], "exit") == 0)
		command->built_in = EXIT;
	else if(strcmp(commands[0], "cd") == 0)
		command->built_in = CD;
	else if(strcmp(commands[0], "status") == 0)
		command->built_in = STATUS;
	else
		command->built_in = NOT_BUILT_IN;
	
	//determine if background
	if(commands[comms - 1][0] == '&') {
		command->background = 1;
		i++;
	} else
		command->background = 0;
	
	//determine out_file and in_file
	//probably a much more elegant method for doing this
	command->out_file = NULL;
	command->in_file = NULL;
	if(comms - i - 2 > 0) {
		if(commands[comms - i - 2][0] == '>') {
			command->out_file = commands[comms - i - 1];
			i += 2;
		} else if(commands[comms - i - 2][0] == '<') {
			command->in_file = commands[comms - i - 1];
			i += 2;
		}
	}
	if(comms - i - 2 > 0) {
		if(commands[comms - i - 2][0] == '>') {
			command->out_file = commands[comms - i - 1];
			i += 2;
		} else if(commands[comms - i - 2][0] == '<') {
			command->in_file = commands[comms - i - 1];
			i += 2;
		}
	}
	
	//build list of commands minus the background and file redirect indicators
	command->comm_length = comms - i;
	command->comm_list = malloc(sizeof(char*) * (command->comm_length + 1));
	for(i = 0; i < command->comm_length; i++)
		command->comm_list[i] = commands[i];
	//append a last null element to meet requirements of execv
	command->comm_list[command->comm_length] = NULL;
	
	return command;
}

//print out command values for testing
void print_command(struct Command *command) {
	
	int i;
	for(i = 0; i < command->comm_length; i++)
		printf("%s\n", command->comm_list[i]);
	printf("infile: %s, outfile: %s, background: %d, type: %d\n", 
	command->in_file, command->out_file, 
	command->background, (int) command->built_in);
	
}

//destructor
void Command_Destroy(struct Command *command) {
	
	free(command->comm_list);
	command->comm_list = NULL;
	free(command);
	command = NULL;
	
}