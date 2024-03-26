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
        else if (atoi(argument) < 0) {
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
 * - clears buffer to prevent overflow  
 * 
*/
void clearBuff() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * - Checks commands and inputs 
*/
int main(int argc, char** argv) {
    char input[MAX_INPUT];
    char *arguments;

    struct mtll *all_lists = NULL; //store mtll lists

    size_t index_lists = 0;

    while (fgets(input, sizeof(input), stdin) != NULL) {  
        if (strncmp(input, "NEW ", 4) == 0) { 
            if (input[4] == ' ' || input[4] == '\n' || input[4] == '\0') {   //checks if there is something after whitespace to prevent core dump
                printInvalidCommand("NEW");
                continue;
            }

            arguments = strtok(input + 4, "\n");  

            if (checkArguments(input + 4, arguments, 1) != 0 || 
                    arguments == NULL || checkType(arguments) != 0 || arguments <= 0) {
                printInvalidCommand("NEW");
                continue;
            } 

            //create new head node 
            struct mtll *new_list = mtll_create(); 
            new_list->id = index_lists;

            //make new list
            make_list(new_list, atoi(arguments));

            //if there are no heads in all_lists, new_node must be the first head 
            if (all_lists == NULL) {
                all_lists = new_list;
            }
            else {
                struct mtll *current_list = all_lists;

                //traverse to end of all_lists
                while (current_list->next != NULL) {
                    current_list = current_list->next;
                }

                //add new_list
                current_list->next = new_list;
            }

            if (feof(stdin)) { 
                break; 
            }

            //to print results 
            switch(new_list->type) {
                case LIST:
                    printf("List %ld: ", new_list->id);
                    break;

                case NESTED:
                    printf("Nested %ld: ", new_list->id);
                    break;
            }

            mtll_view(new_list);

            //increment sizes
            index_lists++;
            
            memset(input, 0, sizeof(input)); //clear buffer
        }   

        else if (strncmp(input, "VIEW ALL", 8) == 0 || strncmp(input, "VIEW ALL ", 9) == 0) { 
            arguments = strtok(input + 8, "\n");  

            if (arguments != NULL) {
                printInvalidCommand("VIEW ALL");
                continue;
            } 

            mtll_view_all(&all_lists, size_all_lists(&all_lists));
        }

        else if (strncmp(input, "VIEW-NESTED ", 12) == 0) { 
            if (input[12] == ' ' || input[12] == '\n' || input[12] == '\0') {
                printInvalidCommand("VIEW-NESTED");
                continue;
            }
            arguments = strtok(input + 12, "\n");  

            if (checkArguments(input + 12, arguments, 1) != 0 || arguments == NULL || input[13] == ' ') {
                printInvalidCommand("VIEW-NESTED");
                continue;
            }   

            struct mtll *tmp = all_lists;
            size_t count = 0;

            //traverse to find list with id 
            while (tmp != NULL) {    
                if (tmp->id == atoi(arguments)) {
                    mtll_view_nested(&all_lists, tmp, 0);
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size_all_lists(&all_lists)) {
                printInvalidCommand("VIEW-NESTED");
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

            struct mtll *tmp = all_lists;
            size_t count = 0;

            //traverse to find list with input index 
            while (tmp != NULL) {    
                if ((tmp)->id == atoi(arguments)) {
                    mtll_view((tmp));
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size_all_lists(&all_lists)) {
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

            //traverse to find list with input index
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

            if (count == size_all_lists(&all_lists)) {
                printInvalidCommand("TYPE");
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

            size_t list_id = atoi(arguments);
            struct mtll *to_remove = all_lists;

            //traverse to find list with input index
            while (to_remove != NULL && to_remove->id != list_id) {
                to_remove = to_remove->next;
            }
            
            //if the list exists remove 
            if (to_remove != NULL) {
                size_t tmp = mtll_remove(&all_lists, to_remove);
                
                //removed successfully 
                if (tmp == 0) { 
                    printf("\n");
                    mtll_view_all(&all_lists, size_all_lists(&all_lists));
                }
                
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

            //seperate arguments 
            char *input_list = strtok(arguments, " ");
            char *input_pos = strtok(NULL, " ");
            char *input_element = strtok(NULL, ""); //if empty set as single whitespace character

            if (arguments == NULL || 
                input_list == NULL || checkType(input_list) != INT ||
                input_pos == NULL || checkType(input_pos) != INT) {
        
                printInvalidCommand("INSERT");
                continue;
            }

            if (atoi(input_list) < 0) {
                printInvalidCommand("INSERT");
                continue;
            }

            struct mtll *tmp = all_lists;
            size_t count = 0;
            
            //traverse to find list to insert element into 
            while (tmp != NULL) {    
                if (tmp->id == atoi(arguments)) {
                    mtll_insert(tmp, atoi(input_pos), input_element);
                    break;
                }
                tmp = tmp->next;
                count++;
            }

            if (count == size_all_lists(&all_lists)) {
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
            char *input_value = strtok(NULL, "");

            if (arguments == NULL || 
                input_list == NULL || checkType(input_list) != INT ||
                input_pos == NULL || checkType(input_pos) != INT || input_value!=NULL) {

                printInvalidCommand("DELETE");
                continue;
            } 

            if (atoi(input_list) < 0) {
                printInvalidCommand("DELETE");
                continue;
            }

            struct mtll *tmp = all_lists;
            size_t count = 0;
            
            //traverse to find list 
            while (tmp != NULL) {    
                if ((tmp)->id == atoi(arguments)) {
                    mtll_delete(tmp, atoi(input_pos));
                    break;
                }
                tmp = (tmp)->next;
                count++;
            }

            if (count == size_all_lists(&all_lists)) {
                printInvalidCommand("DELETE");
            }

        }

        else if (strcmp(input, "\0") == 0) {
            continue;
        }

        else if (strcmp(input, "\n") == 0) { //input blank 
            printInvalidCommand("INPUT");
            continue;
        }


        //input cases where user didnt add arguments 
        else if (strncmp(input, "NEW", 3) == 0) { 
            printInvalidCommand("NEW");
            continue;
        }
        else if (strncmp(input, "VIEW-NESTED", 11) == 0) { 
            printInvalidCommand("VIEW-NESTED");
            continue;
        }
        else if (strncmp(input, "VIEW", 4) == 0) { 
            printInvalidCommand("VIEW");
            continue;
        }
        else if (strncmp(input, "TYPE", 4) == 0) { 
            printInvalidCommand("TYPE");
            continue;
        }
        else if (strncmp(input, "REMOVE", 6) == 0) { 
            printInvalidCommand("REMOVE");
            continue;
        }
        else if (strncmp(input, "INSERT", 6) == 0) { 
            printInvalidCommand("INSERT");
            continue;
        }
        else if (strncmp(input, "DELETE", 6) == 0) { 
            printInvalidCommand("DELETE");
            continue;
        }

        else { //includes commands like NEW1 (missing whitespace)
            printInvalidCommand("INPUT");
        }

    }

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
