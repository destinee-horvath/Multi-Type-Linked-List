#include "mtll.h"


/**
 * - Checks arguments of the command (if valid datatypes in correct order)
 * Parameters: 
 *      - char * : command
 *      - char * : argument
 *      - int : num_args
 * Returns: 
 *      - 0 if valid 
 *      - 1 if not valid 
*/
int checkArguments(char *command, char *argument, int num_args) {
    if (num_args == 1) { //NEW, VIEW, TYPE, REMOVE : expects integer argument
        char *endptr;

        if (checkType(argument) != INT) {
            return 1;
        }

        strtol(argument, &endptr, 10);

        if (*endptr == '\0') {
            return 0;
        }
    }
    else if (num_args == 3) { //INSERT, DELETE : expects at least 2 integer arguments 
        char *arg1 = strtok(argument, " ");
        char *arg2 = strtok(NULL, " ");
        // char *arg3 = strtok(NULL, " ");

        if (arg1 == NULL || arg2 == NULL) { //no arguments
            return 1;
        }

        if (checkType(arg1) == 1 && checkType(arg2) == 1) { //two integer arguments 
            return 0; 
        }
    }

    return 0;
}

/**
 * - print message
 * Parameters: 
 *      - char * : command 
*/
void printInvalidCommand(char *command) {
    printf("INVALID COMMAND: %s\n", command);
    return;
}

/**
 * - Checks commands and inputs 
*/
int main(int argc, char** argv) {
    char input[MAX_INPUT];

    while (fgets(input, sizeof(input), stdin) != NULL) {  
        if (strncmp(input, "NEW ", 4) == 0) { 
                if (input[4] == ' ' || input[4] == '\n' || input[4] == '\0') {   //checks if there is something after whitespace to prevent core dump
                    printInvalidCommand("NEW");
                    continue;
                }

                arguments = strtok(input + 4, "\n");  

                if (checkArguments(input + 4, arguments, 1) != 0 || 
                        arguments == NULL || checkType(arguments) != 0) {
                    printInvalidCommand("NEW");
                    continue;
                } 

                if (arguments <= 0) {
                    printInvalidCommand("NEW");
                    continue;
                }
            }

            else if (strncmp(input, "VIEW ALL", 8) == 0 || strncmp(input, "VIEW ALL ", 9) == 0) { 
                arguments = strtok(input + 8, "\n");  

                if (arguments != NULL) {
                    printInvalidCommand("VIEW ALL");
                    continue;
                } 
            }

            else if (strncmp(input, "VIEW ", 5) == 0) { 
                if (input[5] == ' ' || input[5] == '\n' || input[5] == '\0') {
                    printInvalidCommand("VIEW");
                    continue;
                }
                arguments = strtok(input + 5, "\n");  

                if (checkArguments(input + 5, arguments, 1) != 0 || 
                                    arguments == NULL || input[6] == ' ') {
                    printInvalidCommand("VIEW");
                    continue;
                }   
            
            }

            else if (strncmp(input, "TYPE ", 5) == 0) { 
                if (input[5] == ' ' || input[5] == '\n' || input[5] == '\0') {
                    printInvalidCommand("TYPE");
                    continue;
                }
                arguments = strtok(input + 5, "\n");  

                if (checkArguments(input + 5, arguments, 1) != 0 || arguments == NULL) {
                    printInvalidCommand("TYPE");
                    continue;
                } 

            }
            
            else if (strncmp(input, "REMOVE ", 7) == 0) { 
                if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                    printInvalidCommand("REMOVE");
                    continue;
                }
                arguments = strtok(input + 7, "\n");  

                if (checkArguments(input + 7, arguments, 1) != 0 || arguments == NULL || checkType(arguments) != INT) {
                    printInvalidCommand("REMOVE");
                    continue;
                } 

            }

            else if (strncmp(input, "INSERT ", 7) == 0) {
                if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                    printInvalidCommand("INSERT");
                    continue;
                }
                arguments = strtok(input + 7, "\n");  

                char *input_list = strtok(arguments, " ");
                char *input_pos = strtok(NULL, " ");
                char *input_element = strtok(NULL, "");

                if (arguments == NULL || 
                    input_list == NULL || checkType(input_list) != INT ||
                    input_pos == NULL || checkType(input_pos) != INT) {
            
                    printInvalidCommand("INSERT");
                    continue;
                }

            }   
            else if (strncmp(input, "DELETE ", 7) == 0) {
                if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                    printInvalidCommand("DELETE");
                    continue;
                }
                arguments = strtok(input + 7, "\n");  
                char *input_list = strtok(arguments, " ");
                char *input_pos = strtok(NULL, " ");

                if (arguments == NULL || 
                    input_list == NULL || checkType(input_list) != INT ||
                    input_pos == NULL || checkType(input_pos) != INT) {

                    printInvalidCommand("DELETE");
                    continue;
                } 

            }

            else if (strcmp(input, "\n") == 0) { //input blank 
                continue;
            }

            else { //includes commands like NEW1 (missing whitespace)
                printInvalidCommand("INPUT");
            }
    }
    
    return 0;
}


//strncmp(string1, string2, bytes to compare);
//strtok(string to token/when to start token, when to end token)
//strtol(string to convert, endpointer, base (e.g. base 10))

