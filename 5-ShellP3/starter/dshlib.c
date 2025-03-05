#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "dshlib.h"

static int last_exit_code = 0;

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = NULL;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    return clear_cmd_buff(cmd_buff);
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int close_cmd_buff(cmd_buff_t *cmd_buff) {
	return free_cmd_buff(cmd_buff);
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    int argc = 0;
    char *ptr = cmd_buff->_cmd_buffer;

    while (*ptr != '\0') {
        while (*ptr == ' ') {
            ptr++;
        }
        if (*ptr == '\0')
            break;

        char *token = NULL;
        if (*ptr == '\"') {
            ptr++;
            token = ptr;
            char *end_quote = strchr(ptr, '\"');
            if (end_quote == NULL) {
                end_quote = ptr + strlen(ptr);
            }
            *end_quote = '\0';
            ptr = end_quote + 1;
        } else {
            token = ptr;
            while (*ptr != '\0' && *ptr != ' ') {
                ptr++;
            }
            if (*ptr != '\0') {
                *ptr = '\0';
                ptr++;
            }
        }
        if (argc < CMD_ARGV_MAX) {
            cmd_buff->argv[argc] = token;
            argc++;
        } else {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }
    cmd_buff->argc = argc;
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *token;
    char *saveptr;
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    while (token != NULL) {
        while (*token == ' ') {
            token++;
        }
        if (strlen(token) == 0) {
            token = strtok_r(NULL, PIPE_STRING, &saveptr);
            continue;
        }
        if (clist->num >= CMD_MAX) {
            fprintf(stderr, CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }
        int rc = build_cmd_buff(token, &clist->commands[clist->num]);
        if (rc != OK) {
            return rc;
        }
        clist->num++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    for (int i = 0; i < cmd_lst->num; i++) {
        clear_cmd_buff(&cmd_lst->commands[i]);
    }
    cmd_lst->num = 0;
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0)
        return BI_CMD_EXIT;
    else if (strcmp(input, "dragon") == 0)
        return BI_CMD_DRAGON;
    else if (strcmp(input, "cd") == 0)
        return BI_CMD_CD;
    else if (strcmp(input, "rc") == 0)
        return BI_RC;
    else
        return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds type = match_command(cmd->argv[0]);
    switch (type) {
        case BI_CMD_EXIT:
            exit(0);
            break;
        case BI_CMD_DRAGON:
            extern void print_dragon(void);
            print_dragon();
            break;
        case BI_CMD_CD:
            if (cmd->argc == 1) {
            } else if (cmd->argc == 2) {
                if (chdir(cmd->argv[1]) != 0) {
                    perror("cd");
                }
            } else {
                fprintf(stderr, "cd: too many arguments\n");
            }
            break;
        case BI_RC:
            printf("%d\n", last_exit_code);
            break;
        default:
            break;
    }
    return type;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return ERR_EXEC_CMD;
    }
    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(errno);
    } else {
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return ERR_EXEC_CMD;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return ERR_EXEC_CMD;
        }
    }
    return ERR_EXEC_CMD;
}

int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int i;
    int prev_fd = -1;
    int pipefd[2];
    pid_t pid;
    int status = 0;

    for (i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1) {
            if (pipe(pipefd) < 0) {
                perror("pipe");
                return ERR_EXEC_CMD;
            }
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            return ERR_EXEC_CMD;
        } else if (pid == 0) {
            if (i > 0) {
                if (dup2(prev_fd, STDIN_FILENO) < 0) {
                    perror("dup2");
                    exit(errno);
                }
                close(prev_fd);
            }
            if (i < num_cmds - 1) {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    exit(errno);
                }
                close(pipefd[0]);
                close(pipefd[1]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(errno);
        } else {
            if (i > 0 && prev_fd != -1) {
                close(prev_fd);
            }
            if (i < num_cmds - 1) {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
        }
    }

    while (wait(&status) > 0)
        ;

    return OK;
}


/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int exec_local_cmd_loop() {
    char line[SH_CMD_MAX];
    int rc = OK;
    cmd_buff_t cmd;
    alloc_cmd_buff(&cmd);
    command_list_t clist;
    memset(&clist, 0, sizeof(clist));
    int non_tty = !isatty(STDIN_FILENO);
    int first_iteration = 1;
    while (1) {
        if (!non_tty || !first_iteration) {
            printf("%s", SH_PROMPT);
            fflush(stdout);
        }
        first_iteration = 0;
        if (fgets(line, SH_CMD_MAX, stdin) == NULL) {
            if (non_tty) {
                printf("%s", SH_PROMPT);
            }
            printf("\n");
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) {
            if (!non_tty || isatty(STDIN_FILENO)) {
                printf(CMD_WARN_NO_CMD);
            }
            continue;
        }
        if (strchr(line, PIPE_CHAR) != NULL) {
            char *line_copy = strdup(line);
            if (!line_copy) {
                fprintf(stderr, "Memory allocation error\n");
                continue;
            }
            rc = build_cmd_list(line_copy, &clist);
            free(line_copy);
            if (rc != OK) {
                fprintf(stderr, "Error parsing piped command.\n");
                free_cmd_list(&clist);
                continue;
            }
            rc = execute_pipeline(&clist);
            free_cmd_list(&clist);
        } else {
            rc = build_cmd_buff(line, &cmd);
            if (rc != OK) {
                fprintf(stderr, "Error parsing command.\n");
                clear_cmd_buff(&cmd);
                continue;
            }
            Built_In_Cmds bi = match_command(cmd.argv[0]);
            if (bi != BI_NOT_BI) {
                exec_built_in_cmd(&cmd);
                clear_cmd_buff(&cmd);
                continue;
            }
            rc = exec_cmd(&cmd);
            last_exit_code = rc;
            if (rc != 0) {
                fprintf(stderr, "Error executing command.\n");
            }
            clear_cmd_buff(&cmd);
        }
    }
    return last_exit_code;
}

