#ifndef DIRECTORY_H
#define DIRECTORY_H

char * GetCWD();
char * GetHostname();
char * GetBasename();

int GoUp();
int GoDown(const char * dir);
int GoToPath(const char * path);
int GoHome();

#endif