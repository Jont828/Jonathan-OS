#include "system.h"

#define MAX_CMD_LENGTH 256
#define CMD_HISTORY_LENGTH  4

/* Local command functions */
void parse_command(char buffer[], char cmd[], char args[]);
void exit_terminal();
void echo(char text[]);
void history();

void shift_history();

struct command {
    char name[MAX_CMD_LENGTH];
    void (*func)(char *);
};

struct command command_list[] = 
{ 
    { "exit", &exit_terminal }, 
    { "logout", &exit_terminal }, 
    { "clear", &cls }, 
    { "edit", &editor },
    {"echo", &echo},
    {"history", &history}
};

int stop = 0;

char cmd_history[CMD_HISTORY_LENGTH][MAX_CMD_LENGTH];

/* start_terminal() is in system.h */
void start_terminal()
{
    /* Clears the command history for this terminal session */
    int i;
    for(i=0; i<CMD_HISTORY_LENGTH; i++) {
        cmd_history[i][0] = '\0';
    }

    int history_index = 0;

    char buffer[1024];
    char cmd[MAX_CMD_LENGTH];
    char args[MAX_CMD_LENGTH];

    int size = (int) sizeof(command_list) / sizeof(struct command);
    while( !stop ) {
        puts("Enter a command: ");
        getline(buffer, 1024);
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
        int i = 0, found = 0;
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
