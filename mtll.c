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

