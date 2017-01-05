#include "Execute.h"

#include "ProcList.h"
#include "Func.h"
#include "Directory.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char * DASH_PATH = "DASH_PATH";
ProcessList pl;

void RedirectTo(char * fromProc[], char * file) {
	int fd;
	int pid;
	
	pid = fork();
	
	// If this is the child
	if (pid == 0) {
		// Open the file as the standard output stream
		close(STDOUT_FILENO);
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH);
		if (STDOUT_FILENO != fd)
			dup2(fd, STDOUT_FILENO);
		
		// Exec the new process
		Execute(fromProc[0], fromProc, FORK, BLOCK, 0);
	}
}

void RedirectFrom(char * fromProc[], char * file) {
	int fd;
	int pid;
	
	pid = fork();
	
	// If this is the child
	if (pid == 0) {
		// Open the file as the standard input stream
		close(STDIN_FILENO);
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH);
		if (STDIN_FILENO != fd)
			dup2(fd, STDIN_FILENO);
		
		// Exec the new process
		Execute(fromProc[0], fromProc, FORK, BLOCK, 0);
	}
}

void Pipe(char * fromProc[], char * toProc[]) {	
	// Try to create the pipe
	char * buf = (char *)malloc(15);
	int pid;
	
	pid = fork();
	
	if (pid != 0) { // parent
		wait(NULL);
	} else { // child
		int pipeFD[2];
		
		// Create the pipe
		if (pipe(pipeFD) == -1) {
			ThrowFit("Error creating pipe.");
		} else {
			pid = fork();
			
			if (pid == 0) { // writer
				close(pipeFD[0]); // We dont need to read
				close(STDOUT_FILENO);
				dup2(pipeFD[1], STDOUT_FILENO);
				
				if (Execute(fromProc[0], toProc, NOFORK, NONBLOCK, 0) == -1) {
					exit(-1);
				}
			
			} else { // reader
				close(pipeFD[1]); // We do not need to write;
				close(STDIN_FILENO);
				dup2(pipeFD[0], STDIN_FILENO);
				
				if (Execute(toProc[0], toProc, NOFORK, NONBLOCK, 0) == -1) {
					kill(pid, 9); // parent needs to exit so dispose of the child
					exit(-1);
				}
			}
		}
	}
}

int Execute(char * file, char * argv[], ForkType shouldFork, WaitType waitType, int printError) {
	char * filepath = FindExecutable(file);
	int success = -1;
	
	
	// If the path is NULL
	if (filepath != NULL) {
		success = ExecutePath(filepath, argv, shouldFork, waitType, 0);
	} else {
		// If we should print errors
		if (printError != 0) {
			char * tmp = (char *)malloc(256);
			sprintf(tmp, "%s - '%s'", "Unable to execute file.", file);
			ThrowFit(tmp);
		}
	}
	
	return success;
}

int ExecutePath(char * path, char * argv[], ForkType shouldFork, WaitType waitType, int printError) {
	// Check if the path is an executable file
	int canExecute = access(path, X_OK);
	
	// If we found a executable
	if (canExecute == 0) {
		if (shouldFork == FORK) {
			// Run it
			int pid = fork();
			
			// If parent, wait if we need to
			if (pid != 0) {
				if (waitType == BLOCK) {
					waitpid(pid, NULL, 0);
				} else {
					Add(&pl, argv[0], pid);
				}
			// If child, execute the file at the path
			} else {
				if (execve(path, argv, NULL) == -1) {
					ThrowFit("Unable to execve after fork.");
				}

				canExecute = -1;
			}
		} else {
			// Run it
			if (execve(path, argv, NULL) == -1) {
				char * tmp = (char *)malloc(256);
				sprintf(tmp, "Unable to execve: '%s'", path);
				ThrowFit(tmp);
			}
			
			canExecute = -1;
		}
	} else {
		if (printError) {
			ThrowFit("Unable to execute file.");
		}
	}
	
	return canExecute;
}

int IsExecutableInCWD(char * file) {
	return access(strcat(GetCWD(), file), X_OK);
}

char * FindExecutable(char * file) {
	char * result = getenv(DASH_PATH),
		* path = NULL,
		** split;
	int i = 0, success = -1;

	if (result != NULL) {
		result = strdup(result);
		
		// Split the paths
		split = Split(result, ':');
		
		// While we haven't found an executable program
		while (split[i] != NULL && success != 0) {
			// Concatenate the path with the file
			path = (char *)malloc(strlen(split[i]) + strlen(file) + 1);
			sprintf(path, "%s/%s", split[i], file);
			
			// Find if the file is executable
			success = access(path, X_OK);
			
			// Make sure we clean up the path if it wasnt found
			if (success != 0) {
				free(path);
				path = NULL;
			}
			
			i++;
		}
		
		
		// Free any data
		free(result);
		free(split);
	} else {
		printf("Result of %s is NULL.\n", DASH_PATH);
	}
	
	return path;
}

void KillAll() {
	Process * cur = pl.head;
	
	// While the current element is not NULL
	while (cur != NULL) {
		// Try to kill the process
		if (kill(cur->pid, SIGKILL) == -1) {
			ThrowFit("Error during kill signal.");
		} else {
			// Try to wait on the killed process
			if (waitpid(cur->pid, NULL, 0) == -1) {
				ThrowFit("WaitPID error.");
			}
		}
		
		// Go to the next element
		cur = cur->next;
	}
	
	// Reset the Process List structure
	Reset(&pl);
}

void Kill(int pid) {
	Process * cur = pl.head;
	int found = 0;
	
	// While the current element is not NULL and we have not found the process
	while (cur != NULL && !found) {
		// If this is the process we are looking for
		if (cur->pid == pid) {
			// Try to kill the process
			if (kill(cur->pid, SIGKILL) == -1) {
				ThrowFit("Error during kill signal.");
			} else {
				// Try to wait on the killed process
				if (waitpid(cur->pid, NULL, 0) == -1) {
					ThrowFit("WaitPID error.");
				} else {
					Remove(&pl, pid);
				}
			}
			
			found = 1;
		} else {
			// Go to the next element
			cur = cur->next;
		}
	}
}

void PrintProcessList() {
	Print(&pl);
}

void ResetProcessList() {
	Reset(&pl);
}