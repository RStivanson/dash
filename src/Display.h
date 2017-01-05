#ifndef DISPLAY_H
#define DISPLAY_H

enum PROMPT_FLAG {
	DEFAULT = 0,
	ENVVAR   = 1,
	BASENAME = 2,
	HOSTNAME = 4,
	USERNAME = 8
};

void DisplayUID();
void DisplayPID();
void DisplayPPID();
void DisplayUsername();
void DisplayWorkDir();
void DisplayHomeDir();
void DisplayUserInfo();
void DisplayWorkDir();
void DisplayHomeDir();

void ClearScreen();
void PrintPrompt();
void ChangePrompt(int promptId);

#endif