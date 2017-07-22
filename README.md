Program: smallsh
Author: Joshua Kluthe
Email: kluthej@oregonstate.edu
Date: 2016.11.16
Files:
	smallsh.c
	build_command.c
	build_command.h
	helper_funcs.c
	helper_funcs.h
	run_command.c
	run_command.h
	makefile
	readme.txt
	
Compilation:
	The included makefile should perform all compilation tasks necessary, so simply use the
	command 'make' to compile and link the executable, which should be called 'smallsh'.
	
	Failing this, running the command 
	'gcc smallsh.c helper_funcs.c build_command.c run_command.c -o smallsh'
	should produce the same result.
	
	Additionally, to remove the .o object files and the smallsh executable, you may use the
	command 'make clean'.

Available Commands:
    This shell has built in commands for
    
    "exit": exits the shell
    "cd": changes directory
    "status": displays the exit code of the last command to execute

    Other non-built-in commands can also be run. In this case, the shell spawns a child process that executes the command and returns the exit code.
