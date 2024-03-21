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
            return converted;

        case FLOAT: 
            converted = malloc(sizeof(float));
            *((float *)converted) = atof(input);
            return converted;
        
        case CHAR: 
            converted = malloc(sizeof(char));
            *((char *)converted) = input[0]; 
            return converted;
        
        //anything else is considered string 
        default: 
            converted = strdup(input);  //prevent changing original string
            return converted;
    }

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

    if (strlen(input) == 1) {
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
    head->type = STRING;
    head->id = -1;
    return head;
}

/**
 * Creates linked list based on used inputs
 * Parameters: 
 *      - struct mtll * : head
 *      - size_t        : size 
*/
void make_list(struct mtll *head, size_t size) {
    char input[MAX_INPUT];
    struct Node *current = head->head;

    size_t i = 0;
    while (i < size) {
        if (fgets(input, sizeof(input), stdin) == NULL) { //user didn't enter anything
            strcpy(input, " ");  
        }

        //have case for nested list here 
        
        //create new node and store infos
        struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
        
        new_node->type = checkType(input);            
        new_node->next = NULL;
        new_node->data = convertData(input, new_node->type);
        
        if (current == NULL) { //first node
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
    struct Node *current = head->head;

    while (current != NULL) {
        struct Node *tmp = current;
        current = current->next;
        free(tmp->data);
        free(tmp);
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

    while (current->next != NULL) {
        switch (current->type) {

        case INT:
            printf("%d ", *((int *)current->data));
            break;

        case FLOAT:
            printf("%.2f ", *((float *)current->data));
            break;

        case STRING:
            printf("%s ", (char *)current->data);
            break;

        case CHAR:
            printf("%c ", *((char *)current->data));
            break;

        // case NEST: 
        //     printf("%s ", *((char *)current->data))
        //     break;
        // }

        current = current->next;
        if (current != NULL) {
            printf("->");
        }
    }
    printf("\n");
    }
}

/**
 * Prints a list of all linked lists 
 * Parameters:
 *      struct mtll ** : lists            //pointer to all lists 
 *      size_t         : num_lists
*/
void mtll_view_all(struct mtll **lists, size_t num_lists) {
    printf("Number of lists: %zu\n", num_lists);

    if (lists != NULL) {
        // struct mtll current_list = (*lists)->head;

        for (size_t i = 0; i < num_lists; i++) {
            if (lists[i] != NULL) {
                printf("List %zu: %zu\n", i, lists[i]->id);
            }
            printf("oh no\n"); 
        }
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
void mtll_remove(struct mtll **lists, size_t num_lists, struct mtll *to_remove) {
    if (to_remove == NULL) { 
        return;
    }

    size_t i = 0;
    while (i < num_lists && lists[i] != to_remove) {
        i++;
    }

    if (i == num_lists) { //list doesnt exist 
        return;
    }

    mtll_free(to_remove);
    num_lists--;
    mtll_view_all(lists, num_lists);

}