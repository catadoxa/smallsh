#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

#include "build_command.h"

int run_command(struct Command *command);

int run_exit();

int run_cd(struct Command *command);

int run_status(int status);

int run_not_built_in(struct Command *command);

#endif