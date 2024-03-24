#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        strtol(argument, &endptr, 10);

        if (*endptr == '\0') {
            return 0;
        }
    }
    else if (num_args == 3) { //INSERT, DELETE : expects at least 1 integer arguments 
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
    char *arguments;
    
    struct mtll *all_lists = NULL; //store mtll lists

    size_t index_lists = 0;
    size_t size = 0;

    while (fgets(input, sizeof(input), stdin) != NULL) {    
        printf("INPUT: %s\n", input);
        
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

            //ADD SOME LOGIC TO DETERMINE 
            //IF NESTED 

            struct mtll *new_node = mtll_create(); 
            new_node->id = index_lists;
            make_list(new_node, atoi(arguments));
            printf("List %ld: ", new_node->id);
            mtll_view(new_node);

            if (all_lists == NULL) {      //first node 
                all_lists = new_node;     //make first node head of all_lists
            }
            else { 
                struct mtll *current_list = all_lists;

                while (current_list->next != NULL) {          //traverse to end 
                    current_list = current_list->next;
                }
                current_list->next = new_node;                //add new node
            }
                        
            index_lists++;                                 //increment id 
            size++;                                        //increment size 
        }

        else if (strncmp(input, "VIEW ALL", 8) == 0 || strncmp(input, "VIEW ALL ", 9) == 0) { 
            arguments = strtok(input + 8, "\n");  

            if (arguments != NULL) {
                printInvalidCommand("VIEW ALL");
                continue;
            } 

            mtll_view_all(&all_lists, size);

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

            if (all_lists == NULL) {
                printInvalidCommand("VIEW");
                continue;
            }

            struct mtll *tmp = all_lists;
            size_t count = 0;

            while (tmp != NULL) {    
                if ((tmp)->id == atoi(arguments)) {
                    mtll_view((tmp));
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size) {
                printInvalidCommand("VIEW");
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

            if (all_lists == NULL) {
                printInvalidCommand("TYPE");
                continue;
            }

            struct mtll *tmp = all_lists; 
            size_t count = 0;

            while (tmp != NULL) {    
                if (tmp->id == atoi(arguments)) {
                    mtll_type(tmp);
                    break;
                }
                tmp = tmp->next;
                count++;
            }

            if (count == size) {
                printf("List doesn't exist\n");
            }
        }
        
        else if (strncmp(input, "REMOVE ", 7) == 0) { 
            if (input[7] == ' ' || input[7] == '\n' || input[7] == '\0') {
                printInvalidCommand("REMOVE");
                continue;
            }
            arguments = strtok(input + 7, "\n");  

            if (checkArguments(input + 7, arguments, 1) != 0 || arguments == NULL) {
                printInvalidCommand("REMOVE");
                continue;
            } 

            size_t list_id = atoi(arguments);
            struct mtll *to_remove = all_lists;

            while (to_remove != NULL && to_remove->id != list_id) {
                to_remove = to_remove->next;
            }

            if (to_remove != NULL) {
                mtll_remove(&all_lists, to_remove);
                printf("\n");
                size--;
                mtll_view_all(&all_lists, size);
                continue;
            }
            else {
                printInvalidCommand("REMOVE");
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

            if (arguments == NULL || input_list == NULL || input_pos == NULL) {
                printInvalidCommand("INSERT");
                continue;
            }

            struct mtll *tmp = all_lists;
            size_t count = 0;
            
            while (tmp != NULL) {    
                if ((tmp)->id == atoi(arguments)) {
                    mtll_insert(tmp, atoi(input_pos), input_element);
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size) {
                printInvalidCommand("INSERT");
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

            if (arguments == NULL || input_list == NULL || input_pos == NULL) {
                printInvalidCommand("DELETE");
                continue;
            } 
            
            struct mtll *tmp = all_lists;
            size_t count = 0;
            
            while (tmp != NULL) {    
                if ((tmp)->id == atoi(arguments)) {
                    mtll_delete(tmp, atoi(input_pos));
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size) {
                printInvalidCommand("DELETE");
            }

        }

        else if (strcmp(input, "\n") == 0) { //input blank 
            continue;
        }

        else { //includes commands like NEW1 (missing whitespace)
            printInvalidCommand("INPUT");
        }
    }

    //check brackets (if pair on same line with valid number between then valid, else invalid )
    
    if (all_lists != NULL) {
         struct mtll *curr_list = all_lists;
        while (curr_list != NULL) {
            struct mtll *next_list = curr_list->next;
            mtll_free(curr_list); 
            curr_list = next_list; 
        }

        // free(all_lists);
    }

    return 0;
}


//strncmp(string1, string2, bytes to compare);
//strtok(string to token/when to start token, when to end token)
//strtol(string to convert, endpointer, base (e.g. base 10))


/**
 * Linking Concept: 
 *      O -> O -> O
 *      |
 *      V
 *      O -> O
 *      |
 *      V
 *      O -> O -> O -> O
*/