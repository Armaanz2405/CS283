#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "dshlib.h"

// Compressed binary dragon data
unsigned char dragon_gz[] = {
  0x1f, 0x8b, 0x08, 0x08, 0x38, 0xed, 0xab, 0x67, 0x00, 0x03, 0x64, 0x72,
  0x61, 0x67, 0x6f, 0x6e, 0x2e, 0x74, 0x78, 0x74, 0x00, 0xb5, 0xd6, 0x41,
  0xa2, 0x83, 0x20, 0x0c, 0x04, 0xd0, 0x3d, 0xa7, 0x60, 0x93, 0x7b, 0xe4,
  0xfe, 0xa7, 0xea, 0xaf, 0x4a, 0x0a, 0xc9, 0x4c, 0x82, 0xad, 0x9f, 0x45,
  0x6b, 0x21, 0xf8, 0x48, 0x44, 0x6d, 0xef, 0xcf, 0x34, 0x95, 0xbf, 0x46,
  0xc6, 0xda, 0x23, 0x82, 0x48, 0x42, 0x3c, 0x84, 0x14, 0xc6, 0x03, 0x88,
  0x5c, 0xc6, 0x8c, 0xe8, 0xc3, 0x88, 0x58, 0x73, 0x3d, 0x5f, 0x20, 0x36,
  0xf3, 0xfd, 0xa9, 0xbe, 0x5b, 0xac, 0x77, 0x84, 0x2d, 0xfb, 0x60, 0x03,
  0x11, 0xd8, 0xb2, 0x31, 0x9f, 0x4a, 0x89, 0xa4, 0x27, 0x59, 0x7e, 0x52,
  0xa3, 0x44, 0xb2, 0xa5, 0x86, 0xf3, 0xda, 0xb9, 0x57, 0xa3, 0x40, 0x32,
  0x41, 0xcf, 0xba, 0xe3, 0xe5, 0xdf, 0x40, 0xd2, 0x34, 0x58, 0x2a, 0x12,
  0xef, 0x98, 0xdf, 0x10, 0xae, 0xff, 0x27, 0x72, 0x55, 0x50, 0x21, 0x62,
  0x41, 0x09, 0x51, 0xd3, 0x63, 0x1c, 0x96, 0x0b, 0x67, 0x6a, 0xeb, 0x0a,
  0x15, 0x98, 0x7b, 0x63, 0x88, 0x22, 0x64, 0x09, 0xa2, 0x2b, 0xde, 0x29,
  0x23, 0x2b, 0x7b, 0xeb, 0x3e, 0x64, 0x4e, 0x85, 0x9d, 0x60, 0x1e, 0x50,
  0x16, 0x34, 0x21, 0x9f, 0x60, 0x3f, 0x24, 0x5c, 0x01, 0x77, 0x48, 0xc6,
  0x18, 0x12, 0x8c, 0xe4, 0x9a, 0x24, 0x08, 0x74, 0x06, 0x12, 0x8d, 0x50,
  0x7a, 0x3e, 0x52, 0x41, 0x8d, 0x0d, 0xa4, 0x4a, 0x89, 0xcc, 0x4f, 0xe9,
  0xf4, 0x9a, 0xc4, 0xb2, 0x40, 0x44, 0xcf, 0x33, 0x21, 0x65, 0x1c, 0xde,
  0xc9, 0x84, 0xec, 0x6e, 0x96, 0xdc, 0xa7, 0x6b, 0xca, 0x04, 0x6d, 0x30,
  0xd7, 0x14, 0xf6, 0xa7, 0x15, 0x7c, 0x8f, 0xb4, 0xee, 0x82, 0x57, 0xc4,
  0xcd, 0x55, 0xac, 0xa7, 0x57, 0xe9, 0x40, 0xdc, 0x83, 0x61, 0xce, 0x06,
  0xad, 0x6b, 0xfb, 0xca, 0x5b, 0x1e, 0x72, 0x3c, 0xbb, 0x3c, 0x7c, 0xdc,
  0xf5, 0xd7, 0x05, 0xc5, 0xc6, 0xa6, 0x72, 0x05, 0x82, 0x07, 0xe4, 0xa0,
  0xd8, 0x2c, 0xbc, 0xf1, 0x18, 0x20, 0x86, 0xec, 0x66, 0x5f, 0xec, 0x3d,
  0x10, 0x79, 0x1e, 0xb6, 0xbd, 0x75, 0x45, 0x22, 0x65, 0x5c, 0x5c, 0x2b,
  0x27, 0x50, 0x82, 0xcc, 0x02, 0x7f, 0x21, 0x5b, 0x3e, 0x23, 0x15, 0xd0,
  0xa4, 0xb9, 0x28, 0x10, 0x21, 0xce, 0x34, 0xac, 0x12, 0x5f, 0x7b, 0xb0,
  0xd6, 0x7d, 0x79, 0xfd, 0x81, 0x3f, 0x12, 0xc4, 0xc8, 0xf3, 0x02, 0x93,
  0x68, 0x26, 0x7b, 0x1b, 0x0d, 0x29, 0xd9, 0xeb, 0xfa, 0x06, 0xd2, 0xed,
  0x71, 0x5b, 0x21, 0xc7, 0xb1, 0x26, 0x08, 0x93, 0xba, 0x7d, 0x61, 0x22,
  0x4c, 0x2b, 0x91, 0x5f, 0x9a, 0x33, 0xda, 0x0b, 0x4f, 0xe4, 0xd2, 0x5c,
  0xfa, 0x0e, 0x00, 0x00
};
unsigned int dragon_gz_len = 388;

void print_compressed_dragon(void) {
    #define DRAGON_MAX_SIZE 8192
    unsigned char decompressed[DRAGON_MAX_SIZE];

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = dragon_gz_len;
    strm.next_in  = dragon_gz;

    int ret = inflateInit2(&strm, 16 + MAX_WBITS);
    if (ret != Z_OK) {
        fprintf(stderr, "inflateInit2 error: %d\n", ret);
        return;
    }

    strm.avail_out = DRAGON_MAX_SIZE;
    strm.next_out  = decompressed;

    ret = inflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        fprintf(stderr, "Error decompressing dragon art (zlib error %d)\n", ret);
        inflateEnd(&strm);
        return;
    }
    inflateEnd(&strm);

    size_t decompressed_len = DRAGON_MAX_SIZE - strm.avail_out;
    if (decompressed_len < DRAGON_MAX_SIZE)
        decompressed[decompressed_len] = '\0';
    else
        decompressed[DRAGON_MAX_SIZE - 1] = '\0';

    // Print the decompressed ASCII art.
    printf("%s", decompressed);
}

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    command_list_t clist;

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            exit(0);
        }

        if (strcmp(cmd_buff, "dragon") == 0) {
            print_compressed_dragon();
            continue;
        }

        rc = build_cmd_list(cmd_buff, &clist);
        if (rc == WARN_NO_CMDS) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("error: command or arguments exceed maximum allowed length\n");
            continue;
        } else if (rc != OK) {
            continue;
        }

        printf(CMD_OK_HEADER, clist.num);
        for (int i = 0; i < clist.num; i++) {
            printf("<%d> %s", i + 1, clist.commands[i].exe);
            if (strlen(clist.commands[i].args) > 0) {
                printf(" [%s]", clist.commands[i].args);
            }
            printf("\n");
        }
    }

    return 0;
}
