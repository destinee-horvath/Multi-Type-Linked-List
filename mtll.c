#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            converted = malloc(sizeof(int));
            *((int *)converted) = atoi(input);
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
    char *endptr;

    strtol(input, &endptr, 10);
    if (*endptr == '\0') {
        return INT; 
    }

    strtof(input, &endptr);
    if (*endptr == '\0') {
        return FLOAT; 
    }

    if (strlen(input) == 1 && strcmp(input, "\t") != 0 ) { //&& strcmp(input, " ") != 0 single whitespace is string 
        return CHAR; 
    }

    return STRING; 
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
    size_t enter_blank = 0;

    size_t i = 0;
    while (i < size) {
        if (fgets(input, sizeof(input), stdin) == NULL) { //user didn't enter anything
            strcpy(input, " ");
            enter_blank = 1;
        }
        
        if (strcmp(input, "\n") == 0) {
            strcpy(input, " ");
            enter_blank = 1;
        }
        
        input[strcspn(input, "\n")] = '\0'; //to remove newline character if it exists

        if (strlen(input) >= MAX_INPUT) {
            input[MAX_INPUT - 1] = '\n';

            //need to clear buffer to prevent exceeding characters from being read again 
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        //have case for nested list here 
        
        //create new node and store infos
        struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
        
        if (enter_blank != 1) { //user didnt entered nothing (single whitespace)
            new_node->type = checkType(input);            
        }

        new_node->next = NULL;
        new_node->data = convertData(input, new_node->type);        
        
        if (current == NULL) { //first node
            node_head->head = new_node;
            current = new_node;
        } 

        else {
            current->next = new_node;
            current = new_node;
        }
        
        i++;
    }
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
        free(node_tmp->data);
        free(node_tmp);
    }

    free(head); 
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
            printf(" ");
        }

        current = current->next;
        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("\n");
}

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
        
        }

        current = current->next;
        if (current != NULL) {
            printf(" -> ");
        }
    }

    printf("\n");
}

/**
 * Prints a list of all linked lists 
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
        printf("List %ld\n", (*tmp)->id);
        tmp = &((*tmp)->next);                 //point to the next pointer
    }

    //point to mtll head 
    //iterate to end of list 
    //mtll ONLY points to one list 
    //so need a way to store addresses of heads of lists
}



/**
 * Deletes a list 
 * Parameters:
 *      struct mtll ** : lists            
 *      size_t         : num_lists
 *      struct mtll *  : to_remove    //list to remove 
*/
void mtll_remove(struct mtll **lists, struct mtll *to_remove) {
    if (to_remove == NULL || *lists == NULL || lists == NULL) { 
        return;
    }

    struct mtll *curr = *lists;
    struct mtll *prev = NULL;

    while (curr != NULL && curr != to_remove) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {  //no node found 
        return;
    }

    if (prev != NULL) {  //link previous node to next node (skip curr)
        prev->next = curr->next;
    }
    else {  //curr is head 
        *lists = curr->next;
    }

    printf("List %ld has been removed.\n", curr->id);

    mtll_free(to_remove);

}