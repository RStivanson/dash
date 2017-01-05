#ifndef PROC_LIST_H
#define PROC_LIST_H

typedef struct Process Process;
typedef struct ProcessList ProcessList;

struct Process {
	char * name;
	int pid;
	Process * next;
};

struct ProcessList {
	Process * head;
	int count;
};

void Reset(ProcessList * pl);
void Add(ProcessList * pl, char * name, int pid);
void Print(ProcessList * pl);
void Remove(ProcessList * pl, int pid);

#endif