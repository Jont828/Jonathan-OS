#include "system.h"

#define MAX_CMD_LENGTH  256
#define MAX_CMD_DESC_LENGTH 256
#define CMD_HISTORY_LENGTH  4

int get_command(char *buffer, int lim);

/* Local command functions */
void parse_command(char buffer[], char cmd[], char args[]);

void help();
void exit_terminal();
void echo(char text[]);
void history();
void who(char text[]);
void date();

void shift_history();

struct command {
    char name[MAX_CMD_LENGTH];
    char desc[MAX_CMD_DESC_LENGTH];
    void (*func)(char *);
};

struct command command_list[] = 
{ 
    { "help",
        "lists all commands and their descriptions",
        help },

    { "exit", 
        "exits the system",
        exit_terminal }, 
    { "logout", 
        "exits the system",
        exit_terminal }, 

    { "clear", 
        "clears the screen",
        cls }, 

    { "edit", 
        "opens a simple text editor",
        editor },

    {"echo", 
        "prints the arguments given",
        echo},

    {"history", 
        "displays command history",
        history},

    {"date",
        "displays current time and date [NOT FINISHED]",
        date},

    {"whoami", 
        "displays username of current user", who},
    {"who", 
        "[NOT FINISHED] displays information on users currently logged in",
        who}
};

int stop = 0;

char cmd_history[CMD_HISTORY_LENGTH][MAX_CMD_LENGTH];
// char username[MAX_CMD_LENGTH];
char username[] = "jtong";

/* start_terminal() is in system.h */
void start_terminal()
{
    int size = (int) sizeof(command_list) / sizeof(struct command);
    struct command temp;

    int i, j;
    for(i=0; i<size; i++) {;
        for(j=0; j<size - i - 1; j++) {
            if(strcmp(command_list[j].name, command_list[j+1].name) > 0) {
                strcpy(temp.name, command_list[j].name);
                strcpy(temp.desc, command_list[j].desc);
                temp.func = command_list[j].func;

                strcpy(command_list[j].name, command_list[j+1].name);
                strcpy(command_list[j].desc, command_list[j+1].desc);
                command_list[j].func = command_list[j+1].func;

                strcpy(command_list[j+1].name, temp.name);
                strcpy(command_list[j+1].desc, temp.desc);
                command_list[j+1].func = temp.func;
            }
        }
    }



    /* Clears the command history for this terminal session */
    for(i=0; i<CMD_HISTORY_LENGTH; i++) {
        cmd_history[i][0] = '\0';
    }

    int history_index = 0;

    // puts("Enter your username: ");
    // get_command(username, MAX_CMD_LENGTH);

    char buffer[1024];
    char cmd[MAX_CMD_LENGTH];
    char args[MAX_CMD_LENGTH];

    while( !stop ) {
        puts(username);
        puts("@jonathan-os$ ");
        // puts("Enter a command: ");
        get_command(buffer, 1024);
        parse_command(buffer, cmd, args);

        if(strlen(buffer)) {
            /* If user typed a command, log it into history */
            if(history_index < CMD_HISTORY_LENGTH) {
                strcpy(cmd_history[history_index], buffer);
                history_index++;
            } else {
                shift_history();
                strcpy(cmd_history[CMD_HISTORY_LENGTH - 1], buffer);
            }
        } else {
            /* Continue to next iteration if nothing was entered, skipping the table search */
            continue;
        }

        /* Search command list and run it */
        i = 0;
        int found = 0;
        while(i < size) {
            if(!strcmp(cmd, command_list[i].name)) {
                command_list[i].func(args);
                found = 1;
            }
            i++;
        }

        /* If command entered is not in table */
        if(!found) {
            putch('\'');
            puts(cmd);
            puts("\' is an unrecognized command\n");
        } else {

        }
    }
}

/* Read characters into buffer until user presses '\n' and return length.
* Handles backspaces as well, will add tab completion and arrow keys for cmd history */
int get_command(char *buffer, int lim)
{
    int i=0;
    while(i < (lim-1) && (buffer[i] = getchar()) != '\n') {
        if(buffer[i] == '\b') {
            if(i != 0) {
                buffer[i-1] = '\0';
                buffer[i] = '\0';
                i = i-1;
            }
        } else {
            i++;
        }
    }

    buffer[i] = '\0';

    return i;
}

/* Separates the command from its arguments and puts them into cmd[] and args[], respectively */
void parse_command(char buffer[], char cmd[], char args[]) {
    int i=0, j=0;
    while(buffer[i] != ' ' && i < strlen(buffer)) {
        cmd[i] = buffer[i];
        i++;
    }
    cmd[i] = '\0';
    i++; /* Skip over the space encountered */

    while(i < strlen(buffer)) {
        args[j] = buffer[i];
        i++;
        j++;
    }
    args[j] = '\0';
}

/* Shifts elements of history array over one when it's full, allowing a new element to be put in */
void shift_history() {
    char current[MAX_CMD_LENGTH];
    char temp[MAX_CMD_LENGTH];

    strcpy(current, cmd_history[CMD_HISTORY_LENGTH - 1]);
    strcpy(cmd_history[CMD_HISTORY_LENGTH - 1], "\0");

    int j;
    for(j=CMD_HISTORY_LENGTH-1; j>0; j--) {
        // puts("looping\n");
        strcpy(temp, cmd_history[j-1]);
        strcpy(cmd_history[j-1], current);
        strcpy(current, temp);
    }
}

/* Commands local to the terminal are below */

void help() {
    int size = (int) sizeof(command_list) / sizeof(struct command);

    puts("Jonathan's OS with basic CLI\n\n");
    puts("Supported commands:\n");
    int i;
    for(i=0; i<size; i++) {
        puts(" - ");
        puts(command_list[i].name);
        puts(": ");
        puts(command_list[i].desc);
        puts("\n");
    }
}

void exit_terminal() {
    stop = 1;
}

void echo(char text[]) {
    int i=0;
    while(text[i]) {
        if( text[i] != '\"' )
            putch(text[i]);
        i++;
    }
    putch('\n');
}

void history() {
    int i;
    for(i=0; strlen(cmd_history[i]); i++) {
        puts(cmd_history[i]);
        putch('\n');
    }
}

void who(char text[]) {
    if(strlen(text)) {
        if(!strcmp(text, "am i")) {
            puts("You're ");
            puts(username);
            puts(", at least that's what you told me, anyway\n");
        } else {
            puts("I'm only as smart as you made me buddy\n");
        }
    } else {
        /* User didn't type anything or entered whoami
        * Will improve this implementation later to list all users like on Linux*/
        puts("You're ");
        puts(username);
        puts(", at least that's what you told me, anyway\n");
    }
}

void date() {
    extern time_since_boot;
    puts("System has been active for ");
    putint(time_since_boot);
    puts(" seconds\n");
}