%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

#include "Func.h"
#include "Execute.h"
#include "Display.h"

FILE * tmpFile;
int insideFile = 0, processRsc = 0;

void ProcessResourceFile();
void ProcessFile(char * file);
void OnExit();
void ToggleDebug();

const char * RESOURCE_FOLDER = "/.dash";
const char * S_DEBUG = "-d";
const char * L_DEBUG = "--debug";
const char * S_FDEBUG = "-f";
const char * L_FDEBUG = "--fdebug";
const char * S_START = "-s";
const char * L_START = "--start";
const char * S_LOG = "-l";
const char * L_LOG = "--log";

%}

%union {
	char * strVal;
	int intVal;
}

%token <intVal> NUMBER
%token <strVal> STRING
%token UID PID PPID NAME WD HD INFO EXIT HOME UP PROMPT RUN DOWN GO SET CLEAR DEBUG TASKS HOSE
%token AMPERSAND LESSTHAN GREATERTHAN PIPE SEMICOLON
%token EOL UNKNOWN EOFF

%type <strVal> cmdargs process

%%

commandset:
 | commandset commandline { }
;

commandline: lineend { }
 | command lineend { }
;

lineend:
   SEMICOLON
 | EOL { if (!insideFile) 
			PrintPrompt(); }
 | EOFF { }
;

command:
   process { char ** argv = Split($1, ':');
			 Execute(argv[0], argv, 1, 1, 1);
			 free(argv); }
 | process GREATERTHAN STRING { RedirectTo(Split($1, ':'), $3); }
 | process LESSTHAN STRING { RedirectFrom(Split($1, ':'), $3); }
 | process PIPE process { Pipe(Split($1, ':'), Split($3, ':')); }
 | process AMPERSAND {  char ** argv = Split($1, ':');
						Execute(argv[0], argv, FORK, NONBLOCK, 1);
						free(argv); }
 | builtin 
;

process:
   STRING cmdargs { char * tmp = (char *)malloc(strlen($1) + strlen($2) + 1);
					if (strcmp($2, "") == 0)
						tmp = $1;
					else
						sprintf(tmp, "%s:%s", $1, $2);
					$$ = tmp; }
;

builtin:
   DEBUG { ToggleDebug(); }
 | UID { DisplayUID(); }
 | PID { DisplayPID(); }
 | PPID { DisplayPPID(); }
 | NAME { DisplayUsername(); }
 | WD { DisplayWorkDir(); }
 | HD { DisplayHomeDir(); }
 | INFO { DisplayUserInfo(); }
 | HOME { GoHome(); }
 | UP { GoUp(); }
 | CLEAR { ClearScreen(); }
 | EXIT {	if (IsKeepingLog())
				WriteToTempFile(1, "EXIT\n");
			
			if (IsVerbose())
				printf("Exiting the program.\n");
				
			exit(0); }
 | PROMPT NUMBER { ChangePrompt($2); }
 | GO STRING { GoToPath($2); }
 | SET STRING STRING { SetEnv($2, $3); }
 | RUN STRING { ProcessFile($2); }
 | DOWN STRING { GoDown($2); }
 | TASKS { PrintProcessList();}
 | HOSE { KillAll(); }
 | HOSE NUMBER { Kill($2); }
;

cmdargs: { $$ = ""; }
 | STRING cmdargs { char * tmp = (char *)malloc(strlen($1) + strlen($2) + 1);
					if (strcmp($2, "") == 0)
						tmp = $1;
					else
						sprintf(tmp, "%s:%s", $1, $2);
					$$ = tmp; }
 | NUMBER cmdargs { // Convert the number to a string
					char num[32];
					sprintf(num, "%d", $1);
					
					char * tmp = (char *)malloc(strlen(num) + strlen($2) + 1);
					if (strcmp($2, "") == 0)
						tmp = num;
					else
						sprintf(tmp, "%s:%s", num, $2);
					$$ = tmp; }
;

%%
extern FILE * yyin;

main(int argc, char** argv) {
	int i;
	
	ResetProcessList();
	
	// Make sure we have the default prompt
	ChangePrompt(0);
	
	// On exit function to save the temporary file
	if (atexit(OnExit) == -1)
		ThrowFit("Unable to register exit function");
	
	// Make sure the resource folder exists
	CheckResourceFolder();
	
	// Handle command line arguments
	for (i = 1; i < argc; i++) {
		// Check for log keeping
		if (strcmp(S_LOG, argv[i]) == 0 || strcmp(L_LOG, argv[i]) == 0) {
			TurnOnLogKeeping();
		}
		
		// Check for verbose mode
		if (strcmp(S_DEBUG, argv[i]) == 0 || strcmp(L_DEBUG, argv[i]) == 0) {
			TurnOnVerbose();
		}
		
		// Check for flex and bison debug mode
		if (strcmp(S_FDEBUG, argv[i]) == 0 || strcmp(L_FDEBUG, argv[i]) == 0) {
			yydebug = 1;
		} 
		
		// Check if we should run the resource file
		if (strcmp(S_START, argv[i]) == 0 || strcmp(L_START, argv[i]) == 0) {
			processRsc = 1;
		}
	}
	
	// If we need to keep a log, create a temporary file
	if (IsKeepingLog())
		MakeTemporaryFile();
	
	// If we need to run the resource file, process it
	if (processRsc != 0)
		ProcessResourceFile();
	
	// Start processing
	PrintPrompt();
	yyparse();
}

int yywrap() {
	// Print the prompt and reset our stream to STD in
	PrintPrompt();
	yyrestart(stdin);
	
	// We are not in a file so make sure no file is open
	insideFile = 0;
	
	if (tmpFile != NULL && fclose(tmpFile))
		free(tmpFile);
	
	// return 0 so we continue to get input
	return 1;
}

yyerror(char * s) {
	fprintf(stderr, "Bison Error: %s\n", s);
}

void ProcessResourceFile() {
	// Process the resource file
	ProcessFile(strcat(strcat(getpwuid(getuid())->pw_dir, RESOURCE_FOLDER), "/rc"));
}

void ProcessFile(char * file) {
	// Open the file
	tmpFile = fopen(file, "r");
	insideFile = 1;
	
	// If we are keeping a log, write to the temporary file
	if (IsKeepingLog())
		WriteToTempFile(3, "RUN ", file, "\n");
	
	// If we are in verbose mode, print what we are doing
	if (IsVerbose())
		printf("Going to directory: %s\n", file);
	
	// Process the files content
	if (tmpFile) {
		yyin = tmpFile;
		
		yyparse();
	} else {
		ThrowFit("Unable to open the file.");
	}
	
	// We are no longer in a file
	insideFile = 0;
}

void ToggleDebug() {
	// If we are keeping a log, write to the temporary file
	if (IsKeepingLog())
		WriteToTempFile(1, "DEBUG\n");
	
	// If we are in verbose mode, print what we are doing
	if (IsVerbose())
		printf("Toggling Flex/Bison debug mode.\n");
	
	// Toggle flex/bison debug mode
	if (!yydebug)
		yydebug = 1;
	else
		yydebug = 0;
}