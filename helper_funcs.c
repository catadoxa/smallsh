#include "helper_funcs.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

//write output to terminal. Uses write() system call to maintain reentrancy
int print_text(char *output) {
	int outlength = strlen(output);
	int retval;
	retval = write(STDOUT_FILENO, output, outlength);
	fflush(stdout);
	return retval;
}

//turn a positive int into a string. Does not handle negatives, but that is not necessary
//in this case.
int int_to_str(int num, char *buf, int buf_len) {
	
	int i = 0;
	int len;
	char temp;
	
	//check if zero, else convert digits to chars and add to buf in reverse order
	if(num == 0) {
		buf[i] = '0';
		i++;
	} else	
		while(num != 0) {
			buf[i] = num % 10 + 48;
			num = num / 10;
			i++;
		}
	//append null char to make buf a string
	buf[i] = '\0';
	//rereverse the digits in buf
	len = strlen(buf);
	for(i = 0; i < len/2; i++) {
		temp = buf[i];
		buf[i] = buf[len - i - 1];
		buf[len - i - 1] = temp;
	}
	return 0;
}

//searches through /proc for all child processes of the current process and
//adds them to parameter int procs[]. Returns number of child processes found
int child_procs(int procs[]) {
	
	char *temp;
	char file[255] = "/proc";
	int cppid, cpid;
	int ppid = getpid();
	FILE *fp;
	DIR *dp;
	struct dirent *entry;
	int i = 0;
	
	dp = opendir(file);
	if(dp != NULL)
		while((entry = readdir(dp))) {
			//build up file name to search for parent pid in
			temp = malloc(sizeof(char) * 255);
			strcat(strcat(strcat(strcpy(temp, file), "/"), entry->d_name), "/stat");
			//open file and pull out the values of the pid and the parent pid
			fp = fopen(temp, "r");
			if(fp) {
				fscanf(fp, "%d %*s %*s %d", &cpid, &cppid);
				
				//check processes parent pid against the parent pid we are concerned with
				if(cppid == ppid) {
					//if equal, add the child pid to procs
					procs[i] = cpid;
					i++;
				}
				fclose(fp);
			}
			free(temp);
			temp = NULL;
		}
	closedir(dp);
	
	return i;
}
