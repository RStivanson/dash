#ifndef EXECUTE_H
#define EXECUTE_H

typedef enum ForkType ForkType;
typedef enum WaitType WaitType;

enum ForkType {
	NOFORK = 0,
	FORK
};

enum WaitType {
	BLOCK = 0,
	NONBLOCK
};

void RedirectTo(char * fromProc[], char * file);
void RedirectFrom(char * fromProc[], char * file);
void Pipe(char * fromProc[], char * toProc[]);

int Execute(char * file, char * argv[], ForkType shouldFork, WaitType waitType, int printError);
int ExecutePath(char * path, char * argv[], ForkType shouldFork, WaitType waitType, int printError);

int IsExecutableInCWD(char * file);
char * FindExecutable(char * file);

void KillAll();
void Kill(int pid);

void PrintProcessList();
void ResetProcessList();

#endif