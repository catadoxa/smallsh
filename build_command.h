#ifndef BUILD_COMMAND_H
#define BUILD_COMMAND_H

#define COMM_SIZE 2048
#define NUM_COMMS 512

enum Built_In {NOT_BUILT_IN, EXIT, CD, STATUS};

struct Command {
	
	enum Built_In built_in;
	
	int background;
	
	char *in_file;
	
	char *out_file;
	
	char **comm_list;
	
	int comm_length;
	
	int *status;
};

//constructor
struct Command *Command_New(char **commands, int comms, int *status);

//destructor
void Command_Destroy(struct Command *command);

//testing function
void print_command(struct Command *command);

#endif