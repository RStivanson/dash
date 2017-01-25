#ifndef DIRECTORY_H
#define DIRECTORY_H

/// <summary>
/// Gets the current working directory
/// </summary>
char * GetCWD();

/// <summary>
/// Gets the local hostname
/// </summary>
char * GetHostname();

/// <summary>
/// Gets the basename of the current working directory
/// </summary>
char * GetBasename();

/// <summary>
/// Go up to the parent directory
/// </summary>
int GoUp();

/// <summary>
/// Navigateto a child directory
/// </summary>
/// <param name="dir">The path to navigate to in the file system</param>
int GoDown(const char * dir);

/// <summary>
/// Navigate to a path in the file system
/// </summary>
/// <param name="path">The path to navigate to in the file system</param>
int GoToPath(const char * path);

/// <summary>
/// Go to the home directory
/// </summary>
int GoHome();

#endif
