#pragma once

#define LINE_BUFFER_SIZE 128
#define MAX_COMMAND_SIZE 10

typedef struct COMMANDS ShellCommands;
struct COMMANDS{
    const char *command;
    void (*handler)(const char *argument);
};

void shell_put_char(char c);
void shell_backspace(void);
void shell_enter(void);

void shell_command_clear(const char *argument);
void shell_command_list(const char *argument);
void shell_command_create_file(const char *argument);
void shell_command_shutdown(const char *argument);