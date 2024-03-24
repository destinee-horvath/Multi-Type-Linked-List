#ifndef MTLL_H
#define MTLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>

#define MAX_INPUT 128

enum DataType {
    INT,
    FLOAT,
    STRING,      //nest considered string 
    CHAR
    // NEST 
};

enum ListType {
    LIST, 
    NESTED  
};

//node info
struct Node {
    void *data;            //store data (of any datatype)
    enum DataType type;    //store type of data 
    struct Node *next;   
}; 

//list info 
struct mtll {
    struct Node *head; 
    struct mtll *next;
    enum ListType type;  //type of list (list or nested)
    size_t id;           //give list a unique id 
};


//helper functions 
extern void *convertData(char *, enum DataType);

extern void printInvalidCommand(char *);

enum DataType checkType(char *); 

//Logic functions 
extern struct mtll *mtll_create();

extern void make_list(struct mtll *, size_t); 

extern void mtll_free(struct mtll *);

extern void mtll_view(struct mtll *);

extern void mtll_type(struct mtll *);

extern void mtll_view_all(struct mtll **, size_t);

extern void mtll_remove(struct mtll **, struct mtll *);

extern void mtll_insert(struct mtll *, ssize_t, char *); 

extern void mtll_delete(struct mtll *, ssize_t); 

#endif
