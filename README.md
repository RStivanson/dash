# Flex-And-Bison-Bash
This is a very small implementation of Bash using Flex and Bison in C. This implementation has nowhere near the amount of features as the original Bash but it does have a few of the key features.

# Run Flags
1. -d or --debug ---- Activates verbose mode
2. -f or --fdebug --- Activates Flex debugging
3. -s or --start ---- Processes the resource file at the beginning of execution
4. -l or --log ------ Enables log keeping of commands entered

# Commands
1. UID ----- Get the user's ID
  Syntax UID
2. PID ----- Get the current processes' ID
  Syntax PID
3. PPID ---- Get the parent of the current processes' ID
  Syntax PPID
4. NAME ---- Get the user's username
  Syntax NAME
5. WD ------ Get the current working direction
  Syntax WD
6. HD ------ Get the home directory
  Syntax HD
7. INFO ---- Displays the user's information
  Syntax INFO
8. EXIT ---- Exit the application
  Syntax EXIT
9. HOME ---- Go to the home directory
  Syntax HOME
10. UP ----- Go up in the directory heirarchy
  Syntax UP
11. PROMPT - Change the diplayed prompt
  Syntax PROMPT {PROMPT FLAG}
12. RUN ---- Run a command file
  Syntax RUN {FILE NAME}
13. DOWN --- Go down to a directory within the current directory
  Syntax DOWN {DIRECTORY NAME}
14. CLEAR -- Clear the terminal screen
  Syntax CLEAR
15. GO ----- Go to a directory
  Syntax GO {DIRECTORY PATH}
16. SET ---- Set the value of an environment variable
  Syntax SET {ENV. VARIABLE NAME} {VALUE}
17. DEBUG -- Toggle Debug Mode
  Syntax DEBUG
18. TASKS -- Display a list of all child processes
  Syntax TASKS
19. HOSE --- Kill all children processes or a specific process if the ID is given
  Syntax  HOSE
  Syntax  HOSE {PROCESS ID}

A process can be executed simply by entering it's name.

# Command Modifiers
The following is a list of modifiers that can be applied to either commands or files

1. > ----- Redirect all output from the first into the second
  Syntax  {PROCESS} > {FILE}
2. < ----- Redirect all input from the second into the first
  Syntax  {PROCESS} < {FILE}
3. | ----- All output from the first process is directed into the second process
  Syntax  {PROCESS} | {PROCESS}
4. & ----- Forces the process to run in the background
  Syntax  &{PROCESS}
  
# Prompt Flags
The following values are flags. Any value below the highest value will produce a combination of these prompts. The prompts are appended from this list starting from top to bottom.
  
0. Default
1. Environment Variable
2. Basename
4. Hostname
8. Username

# How To Use
!IMPORTANT!
This program will only compile on Linux due to its Fork function calls. This section of the code will need to be changed in order for it to run on a Windows machine.

The provided Makefile will compile the program correctly.

After compilation, start the program. A terminal will appear and from here you will be able to enter commands and proccesses.
