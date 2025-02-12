#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

// Helper function to trim leading and trailing whitespace.
static char *trim_whitespace(char *str) {
    char *end;

    while(isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end+1) = '\0';

    return str;
}

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;

    char *line = trim_whitespace(cmd_line);
    if(strlen(line) == 0) {
        return WARN_NO_CMDS;
    }

    char *token;
    char *rest = line;
    while ((token = strtok_r(rest, PIPE_STRING, &rest))) {

        char *cmd_str = trim_whitespace(token);
        if (strlen(cmd_str) == 0) {
            continue;
        }

        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        char *cmd_copy = strdup(cmd_str);
        if (!cmd_copy) {
            perror("strdup");
            exit(1);
        }

        char *word;
        char *saveptr;
        word = strtok_r(cmd_copy, " ", &saveptr);
        if (word == NULL) {
            free(cmd_copy);
            continue;
        }

        if (strlen(word) >= EXE_MAX) {
            free(cmd_copy);
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(clist->commands[clist->num].exe, word);

        char args_buffer[ARG_MAX];
        args_buffer[0] = '\0';

        while ((word = strtok_r(NULL, " ", &saveptr))) {
            if (strlen(args_buffer) > 0) {
                strcat(args_buffer, " ");
            }
            if (strlen(args_buffer) + strlen(word) + 1 >= ARG_MAX) {
                free(cmd_copy);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcat(args_buffer, word);
        }
        strcpy(clist->commands[clist->num].args, args_buffer);

        clist->num++;
        free(cmd_copy);
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}
