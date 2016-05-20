#include "system.h"

/* Local command functions */
void parse_command(char buffer[], char cmd[], char args[]);
void exit_terminal();
void echo(char text[]);
void history();

struct command {
    char name[64];
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

#define CMD_HISTORY_LENGTH  4
char cmd_history[CMD_HISTORY_LENGTH][64];

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
    char cmd[64];
    char args[64];

    int size = (int) sizeof(command_list) / sizeof(struct command);
    while( !stop ) {
        puts("Enter a command: ");
        getline(buffer, 1024);
        parse_command(buffer, cmd, args);

        if(!strlen(buffer)) /* User didn't type anything */
            continue;

        int i = 0, found = 0;
        while(i < size) {
            if(!strcmp(cmd, command_list[i].name)) {
                strcpy(cmd_history[history_index], buffer);
                history_index++;

                if(history_index >= CMD_HISTORY_LENGTH) {
                    char current[64];
                    char temp[64];

                    strcpy(current, cmd_history[CMD_HISTORY_LENGTH - 1]);
                    strcpy(cmd_history[CMD_HISTORY_LENGTH-1], "\0");

                    int j;
                    for(j=CMD_HISTORY_LENGTH-1; j>0; j--) {
                        puts("looping\n");
                        strcpy(temp, cmd_history[j-1]);
                        strcpy(cmd_history[j-1], current);
                        strcpy(current, temp);
                    }

                    history_index--;
                }

                command_list[i].func(args);
                found = 1;
            }
            i++;
        }

        if(!found) {
            putch('\'');
            puts(cmd);
            puts("\' is an unrecognized command\n");
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


/* Commands local to the terminal */

void exit_terminal() {
    stop = 1;
}

void echo(char text[]) {
    puts(text);
    putch('\n');
}

void history() {
    int i;
    for(i=0; strlen(cmd_history[i]); i++) {
        puts(cmd_history[i]);
        putch('\n');
    }
}