#include "run_command.h"
#include "helper_funcs.h"

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

//determine type of command, and run appropriate function
int run_command(struct Command *command) {
	
	switch(command->built_in) {
		
		case EXIT:
			return run_exit();
		case CD:
			return run_cd(command);
		case STATUS:
			return run_status(*(command->status));
		default:
			return run_not_built_in(command);
	}
}

int run_exit() {
	
	int procs[255];
	//get list of child processes
	int total = child_procs(procs);
	int i;
	//kill the children!
	for(i = 0; i < total; i++)
		kill(procs[i], SIGKILL);
	
	return 1;
}

//change working directory
int run_cd(struct Command *command) {
	
	char wd[PATH_MAX];
	
	if(command->comm_length == 1)
		chdir(getenv("HOME"));
	else if(command->comm_list[1][0] == '/')
		chdir(command->comm_list[1]);
	else
		chdir(strcat(strcat(getcwd(wd, PATH_MAX), "/"),  command->comm_list[1]));
	
	return 0;

}

//print a status message
int run_status(int status) {

	char num[10];
	char *exit = "exit value ";
	char *term = "terminated by signal ";
	char *new = "\n";
	
	if(WIFEXITED(status)) {
		int_to_str(WEXITSTATUS(status), num, 10);
		print_text(exit);
		print_text(num);
		print_text(new);
	} else {
		int_to_str(WTERMSIG(status), num, 10);
		print_text(term);
		print_text(num);
		print_text(new);
	}
	return 0;
}

//private function run after forking child process
int _run_child(struct Command *command);

//fork a child process and call _run_child(). print information and wait as appropriate
int run_not_built_in(struct Command *command) {
	
	char *bg_pid = "background pid is ";
	char num[10];
	char *new = "\n";
	
	int pid = fork();
	
	if(pid == 0)
		
		_run_child(command);
	
	else if(pid == -1)
		
		perror("fork");
	
	else if(!command->background)
		wait(command->status);
	else {
		//here the command must be running in background, so print out pid
		//print the pid
		int_to_str(pid, num, 10);
		print_text(bg_pid);
		print_text(num);
		print_text(new);
		
	}
		
	return 0;
}

int _run_child(struct Command *command) {
	
	int fd;

	//set background process stdin/out to /dev/null
	if(command->background) {
				
		fd = open("/dev/null", O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		
		fd = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
	} else
		//let foreground child process use default action for interrupt signal
		signal(SIGINT, SIG_DFL);
	
	//set stdin/out to files if indicated
	if(command->in_file) {
		
		fd = open(command->in_file, O_RDONLY);
		if(fd == -1) {
			perror("open");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if(command->out_file) {
		
		fd = open(command->out_file, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	//execute command, or print error message
	execvp(command->comm_list[0], command->comm_list);
	perror("execvp");
	exit(1);
	
	return 0;
}
