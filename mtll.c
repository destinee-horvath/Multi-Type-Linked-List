#include "mtll.h"


/**
 * - Checks the type of data passed through argument
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
 * - Determines the size of the list 
 * Parameters:
 *      - struct Node * : head
 * Returns: 
 *      - size          : int
*/
size_t sizeList(struct Node *head) {
    size_t size = 0;
    struct Node* current = head;

    while (current != NULL) {
        size++;
        current = current->next;
    }

    return size;
}

/**
 * - takes integer out of curly brackets or convert it to int 
 * Parameters: 
 *      char * : str
 * Returns: 
 *      int    : id of nested list 
*/
size_t upwrap_nest(char *str) {
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '{' && str[len - 1] == '}') {
        memcpy(str, str + 1, len - 2);
        str[len - 2] = '\0';
        
        if (checkType(str) == INT) {
            return atoi(str);
        }
    }
    else if (len == 1 && checkType(str) == INT) {
        return atoi(str);
    }
    return -1;
}

/**
 * Determines if a list exists - mainly used to determine if nested lists still exist 
 * Parameters: 
 *      struct mtll ** : lists
 *      int            : list_id 
 * Return:
 *      int            : 0 if list doesnt exist 
 *                     : 1 if list exists 
*/
size_t list_exists(struct mtll **lists, size_t list_id) {
    struct mtll *current = *lists; 

    while (current != NULL) {  
        if (current->id == list_id) {
            return 1;
        }
        current = current->next;

    }

    return 0; 

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
    memset(input, 0, sizeof(input));
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
    //if value is null, set it to whitespace
    if (value == NULL) {
        value = " ";
    }

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
                printInvalidCommand("INSERT");
                free(new_node);
                return;
            }
            new_node->data = new_node->type_string;
            if (strlen(value) + 12 >= MAX_INPUT) {
                clearBuff();
            }
            break;
    }

    new_node->next = NULL;

    //determine list size 
    struct Node *tmp = list->head;
    size_t list_size = sizeList(tmp);

    struct Node *current = list->head;

    //case indexing from back 
    if (index < 0) { 
        current = list->head;
        index = list_size + 1 + index;
        
        if (index < 0) { //exceeds size of list S
            printInvalidCommand("INSERT");
            free(new_node->data);
            free(new_node);
            return;
        }
    }

    //index larger than list 
    if (index > list_size) { 
        printInvalidCommand("INSERT");
        if ((new_node->type == STRING || new_node->type == REF) && new_node->data != NULL) {
            free(new_node->data);
        }
        free(new_node);
        return;
    }

    //insert at beginning 
    if (index == 0 || list->head == NULL) {
        new_node->next = list->head;
        list->head = new_node;
        printf("List %ld: ", list->id);
        mtll_view(list);
        return;
    }

    size_t i = 0;
    //insert at middle and end 
    while (current->next != NULL && i < index - 1) {
        if (i == index) {
            new_node->next = current->next;
            current->next = new_node;
            printf("List %ld: ", list->id);
            mtll_view(list);
            return;
        }
        current = current->next;
        i++;
    }

    //set pointers
    new_node->next = current->next;
    current->next = new_node;

    printf("List %ld: ", list->id);
    mtll_view(list);
    return;

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

    //determine list size 
    struct Node *tmp = list->head;
    size_t list_size = sizeList(tmp);
    
    //case negative index
    if (index < 0) { 
        index = list_size + index;
        if (index < 0) { 
            printInvalidCommand("DELETE");
            return;
        }
    }

    //index larger than list 
    if (index > list_size) { 
        printInvalidCommand("DELETE");
        return;
    }

    //case deleting head 
    if (index == 0) {
        struct Node *delete = list->head;
        list->head = list->head->next;

        if ((delete->type == STRING || delete->type == REF) && delete->data != NULL) {
            free(delete->data);
        }

        free(delete);

        printf("List %ld: ", list->id);
        mtll_view(list);
        return;
    }

    //set temporary pointers 
    struct Node *current = list->head;

    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }

    //node not found 
    if (current == NULL) {
        printInvalidCommand("DELETE");
        return;
    }

    struct Node *delete = current->next;
    current->next = current->next->next;
    //free if string since strdup() was used 
    if ((delete->type == STRING || delete->type == REF) && delete->data != NULL) {
        free(delete->data);
    }

    free(delete);

    printf("List %ld: ", list->id);
    mtll_view(list);

}

/**
 * mtll_view : Prints all the elements in the specified linked list 
 *           : Doesnt print contents of nested list 
 * Parameters: 
 *     struct mtll * : node
*/
void mtll_view(struct mtll *node) {
    if (node == NULL) {
        return;
    }

    struct Node *current = node->head;
    // printf("List %ld: ", node->id);

    if (current == NULL) {
        printf("\n");
        return;
    }

    //first node is whitespace char
    if (current->type == CHAR && *((char *)current->data) == ' ') {
        printf("  -> ");
        current = current->next;
    }

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
                //check if last node is whitespace
                if (current->next == NULL && *((char *)current->data) == ' ') {
                    break;
                }
                printf("%c", *((char *)current->data)); 
                break;

            case REF:                
                printf("{List %ld}", upwrap_nest((char *)current->data));
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

/**
 * mtll_view_nested  :  prints contents including nested list 
 * Parameters:
 *      struct mtll ** : lists
 *      struct mtll *  : node         list with index user entered 
*/
void mtll_view_nested(struct mtll **lists, struct mtll *node, size_t recurse) {
    if (node == NULL) {
        return;
    }

    struct Node *current = node->head;

    if (current == NULL) {
        printf("\n");
        return;
    }

    //not a nested list (recurse 0 times)
    else if (node->type == LIST && recurse == 0) {
        mtll_view(node);
        return;
    }

    //nested list is within list 
    else if (node->type == LIST && recurse > 0) {
        nested_view(node);
        return;
    }

    //find nested list
    struct mtll *tmp = *lists; 
    size_t count = 0;
    size_t exists = 0;
    

    //iterate to check if nested list exists 
    while (tmp != NULL) {

        struct Node *current = tmp->head;

        while (current != NULL) {

            if (current->type == REF) { //type ref exists
                count++;
                if (list_exists(lists, tmp->id) == 1) {  //nested list exists
                    exists++;
                    break;
                }
            } 

            current = current->next;
        }

        tmp = tmp->next;
    }

    //no nested list 
    if (count == 0) {
        mtll_view(node);
        return;
    }
    
    //case where a list was deleted 
    else if (count != exists) { 
        printInvalidCommand("VIEW-NESTED");
        return;
    }
    

    //first node is whitespace char
    if (current->type == CHAR && *((char *)current->data) == ' ') {
        printf("  -> ");
        current = current->next;
    }

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
                //check if last node is whitespace
                if (current->next == NULL && *((char *)current->data) == ' ') {
                    break;
                }
                printf("%c", *((char *)current->data)); 
                break;

            case REF:      
                printf("{"); 
                // mtll_view_nested(lists, lists[upwrap_nest((char *)current->data)], ++recurse); //recursive
                printf("}");
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


void nested_view(struct mtll *node) {
    if (node == NULL) {
        return;
    }

    struct Node *current = node->head;
    // printf("List %ld: ", node->id);

    if (current == NULL) {
        printf("\n");
        return;
    }

    //first node is whitespace char
    if (current->type == CHAR && *((char *)current->data) == ' ') {
        printf("  -> ");
        current = current->next;
    }

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
                //check if last node is whitespace
                if (current->next == NULL && *((char *)current->data) == ' ') {
                    break;
                }
                printf("%c", *((char *)current->data)); 
                break;

            case REF:                
                printf("{List %ld}", upwrap_nest((char *)current->data));
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

}