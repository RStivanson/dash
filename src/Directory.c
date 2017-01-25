#include "Directory.h"

#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>

const int BUFFER_SIZE = 256;

/// <summary>
/// Gets the current working directory
/// </summary>
char * GetCWD() {
	// Try to get the current wokring directory
	char * buf = (char *)malloc(BUFFER_SIZE);
	getcwd(buf, BUFFER_SIZE);
	
	return buf;
}

/// <summary>
/// Gets the base directory name of the current working directory
/// </summary>
char * GetBasename() {
	return basename(GetCWD());
}

/// <summary>
/// Gets the hostname of the local programmer
/// </summary>
char * GetHostname() {
	char * hostname = (char *)malloc(BUFFER_SIZE);
	gethostname(hostname, BUFFER_SIZE);
	
	return hostname;
}

/// <summary>
/// Go up to the parent directory
/// </summary>
int GoUp() {
	int result = 0;
	
	// Try to go up a directory level
	if (chdir("..") == -1) {
		ThrowFit("Unable to go up a directory level.");
		result = -1;
	}
	
	return result;
}

/// <summary>
/// Go down to the given directory, if it exists
/// </summary>
/// <param name="dir">Name of directory</param>
int GoDown(const char * dir) {
	int result = 0;
	
	// Try to go down to the directory
	if (chdir(dir) == -1) {
		ThrowFit("Unable to go down a directory level.");
		result = -1;
	}
	
	return result;
}

/// <summary>
/// Navigate to a path in the file system
/// </summary>
/// <param name="path">The path to navigate to in the file system</param>
int GoToPath(const char * path) {
	int result = 0;
	
	// Try to go to the path
	if (chdir(path) == -1) {
		ThrowFit("Unable to change to the given directory.");
		result = -1;
	}
	
	return result;
}

/// <summary>
/// Go to the local users home directory
/// </summary>
int GoHome() {
	int result = 0;
	
	// Try to go to the home directory
	if (chdir(getpwuid(getuid())->pw_dir) == -1) {
		ThrowFit("Unable to change to the given directory.");
		result = -1;
	}
	
	return result;
}
