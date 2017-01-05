#include "Directory.h"

#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>

const int BUFFER_SIZE = 256;

char * GetCWD() {
	// Try to get the current wokring directory
	char * buf = (char *)malloc(BUFFER_SIZE);
	getcwd(buf, BUFFER_SIZE);
	
	return buf;
}

char * GetBasename() {
	return basename(GetCWD());
}

char * GetHostname() {
	char * hostname = (char *)malloc(BUFFER_SIZE);
	gethostname(hostname, BUFFER_SIZE);
	
	return hostname;
}

int GoUp() {
	int result = 0;
	
	// Try to go up a directory level
	if (chdir("..") == -1) {
		ThrowFit("Unable to go up a directory level.");
		result = -1;
	}
	
	return result;
}

int GoDown(const char * dir) {
	int result = 0;
	
	// Try to go down to the directory
	if (chdir(dir) == -1) {
		ThrowFit("Unable to go down a directory level.");
		result = -1;
	}
	
	return result;
}

int GoToPath(const char * path) {
	int result = 0;
	
	// Try to go to the path
	if (chdir(path) == -1) {
		ThrowFit("Unable to change to the given directory.");
		result = -1;
	}
	
	return result;
}

int GoHome() {
	int result = 0;
	
	// Try to go to the home directory
	if (chdir(getpwuid(getuid())->pw_dir) == -1) {
		ThrowFit("Unable to change to the given directory.");
		result = -1;
	}
	
	return result;
}