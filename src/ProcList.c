#include "ProcList.h"

#include <stdio.h>
#include <stdlib.h>

void Reset(ProcessList * pl) {
	Process * cur = pl->head, * next;
	
	// For every element, free its memory
	while (cur != NULL) {
		next = cur->next;
		
		free(cur);
		
		cur = next;
	}
	
	// Reset all attributes
	pl->head = NULL;
	pl->count = 0;
}

void Add(ProcessList * pl, char * name, int pid) {
	Process * newProc = (Process *)malloc(sizeof(Process));
	
	// Set the new processes attributes
	newProc->name = name;
	newProc->pid = pid;
	newProc->next = NULL;
	
	// If the list is not empty
	if (pl->head != NULL) {
		Process * current = pl->head;
		
		// Find the end of the list
		while (current->next != NULL) {
			current = current->next;
		}
		
		// Add it to the end
		current->next = newProc;
	} else {
		// Make this process the beginning of the list
		pl->head = newProc;
	}
	
	pl->count++;
}

void Remove(ProcessList * pl, int pid) {
	Process * current = pl->head, 
		 * last = NULL;
	int removed = 0;
	
	// While we have removed the process and there is still a list to search
	while (current != NULL && !removed) {
		if (pid == current->pid) {
			// If this is the head of the list
			if (last == NULL) {
				pl->head = current->next;
				
			// Else if we are inside the list
			} else {
				last->next = current->next;
			}
			
			// Free the memory and update the process list
			free(current);
			pl->count--;
			removed = 1;
		} else {
			// Go to the next element
			last = current;
			current = current->next;
		}
	}
}

void Print(ProcessList * pl) {
	int i = 0;
	Process * current = pl->head;
	
	// Print the count of the processes
	printf(" -- Process Count: %d --\n", pl->count);
	
	// Print each element
	while (current != NULL) {
		printf("%d - %d - %s\n", i + 1, current->pid, current->name);
		
		current = current->next;
		i++;
	}
}