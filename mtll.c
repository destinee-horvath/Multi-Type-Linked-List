#include <stdio.h>
#include <stdlib.h>
#include "mtll.h"


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
    head->type = STRING;
    // head->id = 0;
    return head;
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

    for (size_t i = 0; i < num_lists; i++) {
        printf("List %ld: ", lists[i]->id);
        mtll_view(lists[i]);
    }
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