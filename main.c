#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mtll.h"

#define MAX_INPUT 128

/**
 * - Checks the type of data being entered into the linked list 
 * Parameters: 
 *      - char * : input
 * Returns: 
 *      - 0 if string
 *      - 1 if integer
 *      - 2 if float
 *      - 3 if character 
*/
int checkType(char *input) {
    char *endptr;

    //integer
    strtol(input, &endptr, 10);
    if (*endptr == '\0') {
        return 1; 
    }

    //float 
    endptr = NULL;
    strtof(input, &endptr);
    if (*endptr == '\0') {
        return 2; // Float
    }

    //character 
    if (strlen(input) == 1) {
        return 3; 
    }

    //string
    return 0; 
}

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
        long convert = strtol(argument, &endptr, 10);
        if (*endptr == '\0') {
            return 0;
        }
    }
    else if (num_args == 3) { //INSERT, DELETE : expects at least 2 integer arguments 
        char *arg1 = strtok(argument, " ");
        char *arg2 = strtok(NULL, " ");
        char *arg3 = strtok(NULL, " ");

        if (arg1 == NULL || arg2 == NULL || arg3 == NULL) { //no arguments
            return 1;
        }

        if (checkType(arg1) == 1 && checkType(arg2) == 1) { //two integer arguments 
            return 0; 
        }
    }

    return 1;
}

/**
 * - Checks commands and inputs 
*/
int main(int argc, char** argv) {
    char input[MAX_INPUT];
    char *arguments;
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        
        if (strncmp(input, "NEW ", 4) == 0) { 
            if (input[4] == ' ' || input[4] == '\n' || input[4] == '\0') {   //checks if there is something after whitespace to prevent core dump
                printf("INVALID COMMAND: NEW\n");
                continue;
            }

            arguments = strtok(input + 4, "\n");  

            if (checkArguments(input + 4, arguments, 1) != 0 || arguments == NULL) {
                printf("INVALID COMMAND: NEW\n");
                continue;
            } 
        }
        else if (strncmp(input, "VIEW ALL", 8) == 0 || strncmp(input, "VIEW ALL ", 9) == 0) { 
            arguments = strtok(input + 8, "\n");  

            if (arguments != NULL) {
                printf("INVALID COMMAND: VIEW ALL\n");
                continue;
            } 
        }
        else if (strncmp(input, "VIEW ", 5) == 0) { 
            if (input[5] == ' ' || input[5] == '\n' || input[5] == '\0') {
                printf("INVALID COMMAND: VIEW\n");
                continue;
            }
            arguments = strtok(input + 5, "\n");  

            if (checkArguments(input + 5, arguments, 1) != 0 || 
                                 arguments == NULL || input[6] == ' ') {
                printf("INVALID COMMAND: VIEW\n");
                continue;
            } 
        }
        else if (strncmp(input, "TYPE ", 5) == 0) { 
            if (input[5] == ' ' || input[5] == '\n' || input[5] == '\0') {
                printf("INVALID COMMAND: TYPE\n");
                continue;
            }
            arguments = strtok(input + 5, "\n");  

            if (checkArguments(input + 5, arguments, 1) != 0 || arguments == NULL) {
                printf("INVALID COMMAND: TYPE\n");
                continue;
            } 

        }
        else if (strncmp(input, "REMOVE ", 7) == 0) { 
            if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                printf("INVALID COMMAND: REMOVE\n");
                continue;
            }
            arguments = strtok(input + 7, "\n");  

            if (checkArguments(input + 7, arguments, 1) != 0 || arguments == NULL) {
                printf("INVALID COMMAND: REMOVE\n");
                continue;
            } 
        }
        else if (strncmp(input, "INSERT ", 7) == 0) {
            if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                printf("INVALID COMMAND: INSERT\n");
                continue;
            }
            arguments = strtok(input + 7, "\n");  

            if (checkArguments(input + 7, arguments, 3) != 0 || arguments == NULL) {
                printf("INVALID COMMAND: INSERT\n");
                continue;
            } 
        }   
        else if (strncmp(input, "DELETE ", 7) == 0) {
            if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                printf("INVALID COMMAND: DELETE\n");
                continue;
            }
            arguments = strtok(input + 7, "\n");  

            if (checkArguments(input + 7, arguments, 3) != 0 || arguments == NULL) {
                printf("INVALID COMMAND: DELETE\n");
                continue;
            } 
        }
        else { //includes commands like NEW1 (missing whitespace)
            printf("INVALID COMMAND: INPUT\n");
        }
    }

    //check brackets (if pair on same line with valid number between then valid, else invalid )
    return 0;
}


//strncmp(string1, string2, bytes to compare);
//strtok(string to token/when to start token, when to end token)
//strtol(string to convert, endpointer, base (e.g. base 10))