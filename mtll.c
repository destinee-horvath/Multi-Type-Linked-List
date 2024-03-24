#include "mtll.h"

/**
 * - Converts user input to correct datatype 
 * Parameters: 
 *      - char * : input
 *      - enum : datatype
 * Returns: 
 *      - void * : converted
*/
void *convertData(char *input, enum DataType datatype) {
    void *converted = NULL; 

    switch(datatype) {
        case INT:
            converted = malloc(sizeof(size_t));
            *((size_t *)converted) = (size_t)atof(input); 
            break;

        case FLOAT: 
            converted = malloc(sizeof(float));
            *((float *)converted) = atof(input);
            break;
        
        case CHAR: 
            converted = malloc(sizeof(char));
            *((char *)converted) = input[0]; 
            break;
        
        //anything else is considered string 
        default: 
            converted = strdup(input);  //prevent changing original string
            break;
    }

    return converted;

    //note freed when free(node->data)
}


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


    if (strlen(input) == 1 || strcmp(input, "\t") == 0 || isprint(input[0]) == 0) { //&& strcmp(input, " ") != 0 single whitespace is string 
        return CHAR; 
    }

    return STRING; 
}

/**
 * Frees allocated memory for linked list
 * Parameters: 
 *      struct mtll * : head
*/
void mtll_free(struct mtll *head) {
    if (head == NULL) {
        return; 
    }

    struct Node *node_curr = head->head;

    while (node_curr != NULL) {
        struct Node *node_tmp = node_curr;
        node_curr = node_curr->next;
        if (node_tmp->data != NULL) {
            free(node_tmp->data);
        }
        free(node_tmp);
    }

    free(head); 
}

/**
 * Creates a new linked list 
 * Returns:
 *      struct mtll : head
*/
struct mtll *mtll_create() {
    struct mtll *head = (struct mtll *)malloc(sizeof(struct mtll));

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
        if (strlen(input) >= MAX_INPUT) {
            input[MAX_INPUT - 1] = '\n';

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        
        //create new node and store infos
        struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
        
        new_node->type = checkType(input);     


        //case for nested lists
        if (input[0] == '{' && input[strlen(input) - 1] == '}') {
            node_head->type = NESTED;
            new_node->type = REF;
        }
        //

        //set current node parameters
        new_node->next = NULL;
        new_node->data = convertData(input, new_node->type);    
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
}


/**
 * Prints all the elements in the specified linked list 
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

            case REF:
                printf("%s", (char *)current->data);
                break;

            printf(" ");
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