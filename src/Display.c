#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include "Directory.h"

const char * PROMPT_ENV_VAR = "PROMPT";
const char * PROMPT_DELIMITER = ":\0";
const int PROMPT_BUFFER_SIZE = 270;
char * prompt;

void DisplayUID() {
	printf("UID - %d\n", getuid());
}

void DisplayPID() {
	printf("PID - %d\n", getpid());
}

void DisplayPPID() {
	printf("PPID - %d\n", getppid());
}

void DisplayUsername() {
	printf("Username - %s\n", getpwuid(getuid())->pw_name);
}

void DisplayUserInfo() {
	DisplayUID();
	DisplayPID();
	DisplayPPID();
	DisplayUsername();
	DisplayWorkDir();
	DisplayHomeDir();
}

void DisplayWorkDir() {
	char * buf = GetCWD();
	
	// Print the CWd if we got it, else throw a fit
	if (buf != NULL)
		printf("Working Dir - %s\n", buf);
	else
		ThrowFit("Could not get the current working directory.");
}

void DisplayHomeDir() {
	printf("Home Directory - %s\n", getpwuid(getuid())->pw_dir);
}

void ClearScreen() {
	printf("\e[1;1H\e[2J");
}

void PrintPrompt() {
	// Print the current value of prompt with a space afterwards for readability
	if (prompt != NULL)
		printf("%s ", prompt);
}

void ChangePrompt(int promptId) {
	int alreadySet = 0;
	free(prompt);
	prompt = (char *)malloc(PROMPT_BUFFER_SIZE);
	prompt[0] = '\0';
	
	// 8
	if (promptId & USERNAME) {
		// Set the prompt to the username
		strcat(prompt, getpwuid(getuid())->pw_name);
		alreadySet = 1;
	}
	
	// 4
	if (promptId & HOSTNAME) {
		char * hostname = GetHostname();
		
		if (hostname != NULL) {
			// If the prompt was previously set, append the delimiter
			if (alreadySet != 0)
				strcat(prompt, PROMPT_DELIMITER);
			
			// Append the hostname
			strcat(prompt, hostname);
			
			alreadySet = 1;
			free(hostname);
		}
	}
	
	// 2
	if (promptId & BASENAME) {
		char * bname = GetBasename();
		
		// If the prompt was previously set, append the delimiter
		if (alreadySet != 0)
			strcat(prompt, PROMPT_DELIMITER);
			
		// Append the basename
		strcat(prompt, bname);
		
		alreadySet = 1;
	}
	
	// 1
	if (promptId & ENVVAR) {
		char * env = getenv(PROMPT_ENV_VAR);
		
		if (env != NULL) {
			// If the prompt was previously set, append the delimiter
			if (alreadySet != 0)
				strcat(prompt, PROMPT_DELIMITER);
			
			// Append the environment variable
			strcat(prompt, getenv(PROMPT_ENV_VAR));
			
			alreadySet = 1;
		}
	}
	
	// 0
	if (promptId == DEFAULT) {
		// Reset the current memory and set prompt to ">"
		free(prompt);
		prompt = (char *)malloc(2);
		prompt[0] = '>';
		prompt[1] = '\0';
	}
}