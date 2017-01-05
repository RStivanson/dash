#ifndef FUNC_H
#define FUNC_H

#include <pwd.h>

void CheckResourceFolder();
void GoToResourceFolder();
void GoToLogsFolder();
void WriteToTempFile(int args, ...);
void MakeTemporaryFile();
void ThrowFit(char * message);
void SetEnv(char * env, char * value);
void OnExit();
void TurnOnVerbose();
int IsVerbose();
void TurnOnLogKeeping();
int IsKeepingLog();

char ** Split(char * a_str, const char a_delim);

#endif