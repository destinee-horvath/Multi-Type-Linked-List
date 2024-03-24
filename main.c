#include "mtll.h"


/**
 * - Checks commands and inputs 
*/
int main(int argc, char** argv) {
    char input[MAX_INPUT];

    while (fgets(input, sizeof(input), stdin) != NULL) {      
        printf("%s\n", input);
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