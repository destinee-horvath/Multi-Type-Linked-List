#include "mtll.h"


/**
 * - Checks the type of data being entered into the linked list 
 * Parameters: 
 *      - char * : input
 * Returns: 
 *      - enum Datatype
*/
enum DataType checkType(char *input) {
    if (input == NULL) {
        return STRING;
    }
    
    char *endptr;

    errno = 0;
    strtol(input, &endptr, 10);
    if (errno == 0 && endptr != input && *endptr == '\0') {
        return INT; 
    }

    errno = 0;
    strtof(input, &endptr);
    if (errno == 0 && *endptr == '\0') {
        return FLOAT; 
    }

    if (strlen(input) == 1 && isprint(input[0])) { // || strcmp(input, "\t") == 0 
        return CHAR; 
    }

    return STRING; 
}

/**
 * Frees allocated memory for linked list
 * Parameters: 
 *      struct mtll * : list
*/
void mtll_free(struct mtll *list) {
    //case no head 
    if (list == NULL) {
        return; 
    }

    struct Node *node_curr = list->head;

    //iterate through linked list 
    while (node_curr != NULL) {
        struct Node *node_tmp = node_curr;
        node_curr = node_curr->next;

        //if type string, need to free since strdup() used 
        if ((node_tmp->type == STRING || node_tmp->type == REF) && node_tmp->data != NULL) {
            free(node_tmp->data);
        }

        //free node 
        free(node_tmp);
    }

    //free head 
    free(list); 
}

/**
 * Creates a new head of linked list 
 * Returns:
 *      struct mtll : head
*/
struct mtll *mtll_create() {
    struct mtll *head = malloc(sizeof(struct mtll));

    if (head == NULL) { //memory allocation failed  
        return NULL;
    }

    head->head = NULL;
    head->next = NULL;
    head->type = LIST;
    head->id = -1;
    return head;
}

/**
 * Creates linked list based on used inputs
 * Parameters: 
 *      - struct mtll * : head
 *      - size_t        : size 
*/
void make_list(struct mtll *node_head, size_t size) {
    char input[MAX_INPUT];
    struct Node *current = node_head->head;

    //initialise input buffer
    memset(input, 0, sizeof(input));

    size_t i = 0;

    //loop to create list of size 
    while (i < size) {
        //user didn't enter anything
        if (fgets(input, sizeof(input), stdin) == NULL) { 
            strcpy(input, " ");
        }
        
        //user entered or tabbed
        if (strcmp(input, "\n") == 0 || strcmp(input, "\t") == 0) {
            strcpy(input, " ");
        }

        //remove newline character if it exists
        input[strcspn(input, "\n")] = '\0'; 

        //need to clear buffer to prevent exceeding characters from being read again 
        if (strlen(input) >= MAX_INPUT && input[MAX_INPUT - 1] != '\n') {
            input[MAX_INPUT - 1] = '\n';

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        
        //create new node and store infos
        struct Node *new_node = malloc(sizeof(struct Node));
        
        //determine input type
        new_node->type = checkType(input);  

        //case for nested lists
        if (input[0] == '{' && input[strlen(input) - 1] == '}') {
            node_head->type = NESTED;
            new_node->type = REF;
        }
        ///

        //set current node parameters
        new_node->next = NULL;
        
        //convert data 
        switch (new_node->type) {
            case INT: 
                new_node->data = &new_node->type_int; 
                sscanf(input, "%d", &new_node->type_int);
                break;

            case FLOAT:
                new_node->data = &new_node->type_float; 
                sscanf(input, "%f", &new_node->type_float);
                break;

            case CHAR:
                new_node->data = &new_node->type_char; 
                new_node->type_char = input[0];
                break;

            default: //anything else is just a string like STRING, REF
                new_node->type_string = strdup(input);
                if (new_node->type_string == NULL) {
                    free(new_node);
                }
                new_node->data = new_node->type_string;
                break;
        }


        if (new_node->data == NULL) { 
            printf("Conversion fail??\n");
        }    
        
        //case current is first node
        if (current == NULL) { 
            node_head->head = new_node;
            current = new_node;
        } 
        //case current is not first node 
        else {
            current->next = new_node;
            current = new_node;
        }
        
        i++;
        
    }
    
    //clear buffer 
    if (strlen(input) >= MAX_INPUT && input[MAX_INPUT - 1] != '\n') {
        input[MAX_INPUT - 1] = '\n';

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
}

/**
 * mtll_remove : Deletes an entire linked list 
 * Parameters:
 *      struct mtll ** : lists            
 *      size_t         : num_lists
 *      struct mtll *  : to_remove    //list to remove 
*/
void mtll_remove(struct mtll **lists, struct mtll *to_remove) {
    if (to_remove == NULL || *lists == NULL || lists == NULL) { 
        printInvalidCommand("REMOVE");
        return;
    }

    struct mtll *curr = *lists;
    struct mtll *prev = NULL;

    while (curr != NULL && curr != to_remove) {
        prev = curr;
        curr = curr->next;
    }

    //no node found 
    if (curr == NULL) {  
        return;
    }

    //link previous node to next node (skip curr)
    if (prev != NULL) {  
        prev->next = curr->next;
    }
    else {  //curr is head 
        *lists = curr->next;
    }

    printf("List %ld has been removed.\n", curr->id);

    mtll_free(to_remove);

}

/**
 * mtll_insert : Inserts an element into a linked list 
 * Parameters: 
 *      - struct mtl * : list
 *      - size_t       : index 
 *      - void *       : value
*/
void mtll_insert(struct mtll *list, ssize_t index, char *value) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        printInvalidCommand("INSERT");
        return;
    }

    //determine type of value 
    new_node->type = checkType(value);
    switch (new_node->type) {
        case INT: 
            new_node->data = &new_node->type_int; 
            sscanf(value, "%d", &new_node->type_int);
            break;

        case FLOAT:
            new_node->data = &new_node->type_float; 
            sscanf(value, "%f", &new_node->type_float);
            break;

        case CHAR:
            new_node->data = &new_node->type_char; 
            new_node->type_char = value[0];
            break;

        default: //anything else is just a string like STRING, REF
            new_node->type_string = strdup(value);
            if (new_node->type_string == NULL) {
                free(new_node);
            }
            new_node->data = new_node->type_string;
            break;
    }

    new_node->next = NULL;

    //determine list size 
    size_t list_size = 0;
    struct Node *tmp = list->head;
    while (tmp != NULL) {
        list_size++;
        tmp = tmp->next;
    }

    struct Node *current = list->head;
    size_t i = 0;

    //case indexing from back 
    if (index < 0) { 
        current = list->head;
        index = list_size + index + 1;
        
        if (index < 0) { //exceeds size of list 
            printInvalidCommand("INSERT");
            free(new_node->data);
            free(new_node);
            return;
        }
    }

    //insert at beginning 
    if (index == 0 || list->head == NULL) {
        new_node->next = list->head;
        list->head = new_node;
        printf("List %ld: ", list->id);
        mtll_view(list);
        return;
    }

    //insert middle  
    while (current->next != NULL && i < index - 1) {
        current = current->next;
        i++;
    }

    //index larger than list 
    if (i != index - 1) { 
        printInvalidCommand("INSERT");
        free(new_node->data);
        free(new_node);
        return;
    }

    //set pointers
    new_node->next = current->next;
    current->next = new_node;

    printf("List %ld: ", list->id);
    mtll_view(list);

}

/**
 * mtll_delete : Deletes a node at a position 
 * Parameters: 
 *      - struct mtl * : list
 *      - size_t       : index 
*/
void mtll_delete(struct mtll *list, ssize_t index) {
    if (list == NULL || list->head == NULL) {
        printInvalidCommand("DELETE");
        return; 
    }

    //set temporary pointers 
    struct Node *current = list->head;
    struct Node *prev = NULL;
    size_t i = 0;

    
    //case negative index - delete end  
    if (index < 0) { 
        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }
    }
    else {
        while (current != NULL && i < index) {
            prev = current;
            current = current->next;
            i++;
        }
    }
    

    //node not found 
    if (current == NULL) {
        printInvalidCommand("DELETE");
        return;
    }

    //delete
    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }


    //free if string since strdup() was used 
    if ((current->type == STRING || current->type == REF) && current->data != NULL) {
        free(current->data);
    }

    //free node 
    free(current);

    printf("List %ld: ", list->id);
    mtll_view(list);

}

/**
 * mtll_view : Prints all the elements in the specified linked list 
 * Parameters: 
 *     struct mtll * : node
*/
void mtll_view(struct mtll *node) {
    if (node == NULL) {
        return;
    }

    struct Node *current = node->head;
    // printf("List %ld: ", node->id);

    while (current != NULL) {
        switch (current->type) {

            case INT:
                printf("%d", *((int *)current->data));
                break;

            case FLOAT:
                printf("%.2f", *((float *)current->data));
                break;

            case STRING:
                printf("%s", (char *)current->data);
                break;

            case CHAR:
                printf("%c", *((char *)current->data)); 
                break;

            default:
                break;

            
        }

        current = current->next;
        if (current != NULL) {
            printf(" ->");
        }

        if (current != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}

/**
 * mtll_type : Prints types of elements in list
 * Parameters:
 *      struct mtll * : Node 
*/
void mtll_type(struct mtll *node) {
    if (node == NULL) {
        return;
    }

    struct Node *current = node->head;

    while (current != NULL) {
        switch (current->type) {

            case INT:
                printf("int");
                break;

            case FLOAT:
                printf("float");
                break;

            case STRING:
                printf("string");
                break;

            case CHAR:
                printf("char");
                break;
            
            case REF: 
                printf("reference");
                break;
        }

        current = current->next;
        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("\n");
}


/**
 * mtll_view_all : Prints a list of all linked lists 
 * Parameters:
 *      struct mtll ** : lists            //pointer to heads of all lists 
 *      size_t         : num_lists
*/
void mtll_view_all(struct mtll **lists, size_t num_lists) {
    printf("Number of lists: %zu\n", num_lists);

    struct mtll **tmp = lists;

    if (num_lists == 0 || lists == NULL) {
        return;
    }

    while ((*tmp) != NULL) {    
        
        //print
        switch((*tmp)->type) {
            case LIST:
                printf("List %ld\n", (*tmp)->id);
                break;

            case NESTED:
                printf("Nested %ld\n", (*tmp)->id);
                break;
        }

        //point to next pointer 
        tmp = &((*tmp)->next);                 
    }
}