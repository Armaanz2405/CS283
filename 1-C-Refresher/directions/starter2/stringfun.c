#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int);
void word_print(char *, int);
int replace_substring(char *, int, char *, char *);

int setup_buff(char *buff, char *user_str, int len) {
    int i = 0, j = 0;
    char prev_char = ' '; // Tracks consecutive spaces

    while (user_str[i] != '\0') {
        if (j >= len) {
            return -1; // User string exceeds buffer size
        }

        if (isspace(user_str[i])) {
            if (prev_char != ' ') {
                *(buff + j++) = ' ';
            }
        } else {
            *(buff + j++) = *(user_str + i);
        }
        prev_char = *(user_str + i);
        i++;
    }

    // Remove trailing space if present
    if (j > 0 && *(buff + j - 1) == ' ') {
        j--; // Step back to overwrite the trailing space
    }

    // Fill remaining buffer with dots
    while (j < len) {
        *(buff + j++) = '.';
    }

    return i; // Return the length of the original processed user string
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    (void)len; // Mark len as unused to suppress the warning
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') { // Ignore spaces and dots
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else if (*(buff + i) == ' ') {
            in_word = 0;
        }
    }

    return word_count;
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int str_len) {
    int start = 0, end = str_len - 1;

    // Reverse only the significant portion of the buffer
    while (start < end) {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }

    // Fill trailing portion with dots
    for (int i = str_len; i < BUFFER_SZ; i++) {
        buff[i] = '.';
    }
}

void word_print(char *buff, int str_len) {
    printf("Word Print\n----------\n");
    int word_start = 0, word_len = 0, word_num = 1;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ' || i == str_len - 1) {
            if (i == str_len - 1 && buff[i] != ' ') {
                word_len++; // Include the last character if it's part of a word
            }
            if (word_len > 0) {
                printf("%d. ", word_num++);
                for (int j = 0; j < word_len; j++) {
                    putchar(buff[word_start + j]);
                }
                printf("(%d)\n", word_len);
                word_len = 0;
            }
        } else if (word_len == 0) {
            word_start = i;
            word_len = 1;
        } else {
            word_len++;
        }
    }
    printf("\nNumber of words returned: %d\n", word_num - 1);
}

int replace_substring(char *buff, int len, char *target, char *replacement) {
    char *read_ptr = buff;        // Pointer to traverse the original buffer
    char temp_buffer[len];        // Temporary buffer for safe replacement
    char *write_ptr = temp_buffer; // Pointer to construct the new buffer
    char *target_ptr;             // Pointer to match the target substring
    char *replacement_ptr;        // Pointer for replacement string
    int target_len = 0;           // Length of target substring
    int replacement_len = 0;      // Length of replacement substring
    int original_len = 0;         // Length of meaningful content in buffer
    int found = 0;                // Flag to track if a match was found

    // Compute lengths of target and replacement substrings
    target_ptr = target;
    while (*target_ptr++) {
        target_len++;
    }
    replacement_ptr = replacement;
    while (*replacement_ptr++) {
        replacement_len++;
    }

    // Calculate the length of the original string in the buffer
    while (original_len < len && *(buff + original_len) != '.') {
        original_len++;
    }

    // Traverse the buffer to process the replacement
    while ((read_ptr - buff) < original_len) {
        char *match_start = read_ptr;
        target_ptr = target;

        // Check for target match
        while (*target_ptr && *read_ptr && *target_ptr == *read_ptr) {
            target_ptr++;
            read_ptr++;
        }

        if (!*target_ptr) { // Match found
            found = 1; // Mark match as found

            // Replace target with replacement
            replacement_ptr = replacement;
            while (*replacement_ptr) {
                if ((write_ptr - temp_buffer) >= len) {
                    return -1; // Overflow, replacement too long
                }
                *write_ptr++ = *replacement_ptr++;
            }
        } else {
            // No match, copy the character
            *write_ptr++ = *match_start;
            read_ptr = match_start + 1;
        }
    }

    if (!found) {
        printf("Not Implemented!\n");
        return 3; // Exit with error code 3 (service error)
    }

    // Fill the rest of the buffer with dots
    while ((write_ptr - temp_buffer) < len) {
        *write_ptr++ = '.';
    }

    // Copy the updated buffer back into the original
    char *temp_ptr = temp_buffer;
    for (int i = 0; i < len; i++) {
        *(buff + i) = *temp_ptr++;
    }

    return 0; // Replacement successful
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      This is safe because argc ensures at least 2 arguments exist before accessing argv[1].
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      Ensures that the user provided a string for options other than 'h'.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (!buff) {
        printf("Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            word_print(buff, user_str_len);
            break;
        case 'x':
                if (argc < 5) {
                        printf("Error: Missing arguments for search and replace.\n");
                        free(buff);
                        exit(1);
                }
                char *target = argv[3];
                char *replacement = argv[4];
                rc = replace_substring(buff, BUFFER_SZ, target, replacement);
                if (rc < 0) {
                        printf("Buffer overflow occurred during replacement.\n");
                        free(buff);
                        exit(2);
                }
                break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that
//          the buff variable will have exactly 50 bytes?
//
//          PLACE YOUR ANSWER HERE
//          Providing both ensures safety and clarity. The length avoids potential
//          overruns and allows functions to operate only on the relevant portion
//          of the buffter, even if the buffer is larger.
