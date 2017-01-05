#include "Func.h"
#include "Directory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdarg.h>
#include <libgen.h>

const char * RSC_FOLDER_PATH = ".dash";
const char * LOG_FOLDER_PATH = ".dash/logs";

int tempLogFD = -1, keepLog = 0, verbose = 0;

void CheckResourceFolder() {
	// Capture where we currently are
	char * cwd = GetCWD();
	
	// Go to our home directory
	GoHome();
	
	// If we cannot move to the .dash folder
	if (chdir(".dash") == -1) {
		// make the directory
		if (mkdir(".dash", S_IRWXU | S_IRGRP | S_IROTH) == -1) {
			ThrowFit("Unable to create the DASH resource folder");
		} else {
			// If we made the .dash directory, make the logs directory
			if (mkdir("logs", S_IRWXU | S_IRGRP | S_IROTH) == -1) {
				ThrowFit("Unable to create the DASH logs folder");
			}
		}
	} else {
		// If we were able to move to .dash, try to move to the logs directory
		if (chdir("logs") == -1) {
			// If we couldnt move to logs, then make the logs directory
			if (mkdir("logs", S_IRWXU | S_IRGRP | S_IROTH) == -1) {
				ThrowFit("Unable to create the DASH logs folder");
			}
		}
	}
	
	// Go back to where we started
	GoToPath(cwd);
}

void WriteToTempFile(int args, ...) {
	// If the temporary file was made and we have atleast 1 argument
	if (tempLogFD != -1 && args > 0) {
		char * content;
		va_list vl;
		int i;
		
		// Get the start of the variable list
		va_start(vl, args);
		
		// Get the first argument
		content = va_arg(vl, char *);
		
		// For every argument in the list
		for (i = 0; i < args; i++) {
			// Write to the file
			write(tempLogFD, content, strlen(content));
			
			// Get the next argument
			content = va_arg(vl, char *);
		}
		
		// End the variable list
		va_end(vl);
	}
}

void MakeTemporaryFile() {
	char format[] = "formatXXXXXX";
	
	// Create a temporary file
	tempLogFD = mkstemp(format);
	
	// If we were unable to create the file, throw a fit
	if (tempLogFD == -1)
		ThrowFit("Unable to create a temporary log file.");
	else
		// Unlink it so noone can access it
		unlink(format);
}

void ThrowFit(char * message) {
	// Print our message plus whatever error the system currently has
	printf("%s\n", message);
	perror("P-Error");
}

void SetEnv(char * env, char * value) {
	// If we are keeping a log, write to the temporary file
	if (keepLog != 0)
		WriteToTempFile(5, "SET ", env, " ", value, "\n");
	
	// If we are in verbose mode, print what we are doing
	if (verbose != 0)
		printf("Set the environment variable '%s' to '%s'\n", env, value);
	
	// Try to set the environment variable
	if (setenv(env, value, 1) == -1)
		ThrowFit("Unable to set the environment variable.");
	
	// Free the values
	free(env);
	free(value);
}

void OnExit() {
	int sizeRead, logFile;
	char buffer[1], logBuffer[] = "logXXXXXX";
	
	keepLog = 0;
	
	if (tempLogFD != -1) {
		// Traverse to the logs folder
		GoToLogsFolder();
		
		// Open the file to hold the log
		logFile = mkstemp(logBuffer);
		
		// Goto the beginning of the temporary file
		lseek(tempLogFD, 0, SEEK_SET);
		
		// If we read a btye
		sizeRead = read(tempLogFD, buffer, sizeof(char));
		
		// Transfer the bytes from the temporary file to the log file
		while (sizeRead != 0) {
			write(logFile, buffer, sizeof(char));
			sizeRead = read(tempLogFD, buffer, sizeof(char));
		}
		
		// Close both files
		close(logFile);
		close(tempLogFD);
	}
	
	return;
}

// Traverse to the resource folder
void GoToResourceFolder() {
	GoHome();
	GoDown(RSC_FOLDER_PATH);
}

// Traverse to the logs folder
void GoToLogsFolder() {
	GoHome();
	GoDown(LOG_FOLDER_PATH);
}

void TurnOnVerbose() {
	verbose = 1;
}

int IsVerbose() {
	return verbose;
}

void TurnOnLogKeeping() {
	keepLog = 1;
}

int IsKeepingLog() {
	return keepLog;
}

// I copied this from Stack Overflow to save myself some time
// Creates a NULL terminated array of c-strings split by a delimier
char ** Split(char * a_str, const char a_delim) {
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}
	
	/* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);

	/* Add space for terminating null string so caller
		knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}