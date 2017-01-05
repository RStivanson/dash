%{
	#include "bison.tab.h"
%}
%x SET_COMMAND
%%
 /* Builtin Commands */
"UID"		{ return UID; }
"PID"		{ return PID; }
"PPID"		{ return PPID; }
"NAME"		{ return NAME; }
"WD"		{ return WD; }
"HD"		{ return HD; }
"INFO"		{ return INFO; }
"EXIT"		{ return EXIT; }
"HOME"		{ return HOME; }
"UP"		{ return UP; }
"PROMPT"	{ return PROMPT; }
"RUN"		{ return RUN; }
"DOWN"		{ return DOWN; }
"CLEAR"		{ return CLEAR; }
"GO"		{ return GO; }
"SET"		{ BEGIN(SET_COMMAND); return SET; }
"DEBUG"		{ return DEBUG; }
"TASKS"		{ return TASKS; }
"HOSE"		{ return HOSE; }


 /* Rules to handle command manipulators */
"|"		{ return PIPE; }
"&"		{ return AMPERSAND; }
">"		{ return GREATERTHAN; }
"<"		{ return LESSTHAN; }
<*>";"		{ return SEMICOLON; }

 /* Special conditions for commands */
<SET_COMMAND>[^ \t\r\n;]+ { yylval.strVal = strdup(yytext); return STRING; }

 /* Default rules for datatypes */
[0-9]+ { yylval.intVal = atoi(yytext); return NUMBER; }
[a-zA-Z0-9._/-]+ {  yylval.strVal = strdup(yytext); return STRING; }

 /* Rules to handle whitespace and any unhandled characters */
<*>[ \t\r]	{ }
<*>\n		{ BEGIN(INITIAL); return EOL; }
<*><<EOF>>	{ return EOFF; }
<*>.		{ }


%%