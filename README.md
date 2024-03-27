-- TESTCASE DESCRIPTIONS --

TEST INPUT COMMANDS: 
    errorDelete
    errorInsert
    errorNew
    errorRemove
    errorType
    errorViewAll
    errorView 
    errorViewNested
    
    - tests various invalid input commands
        - no arguments entered
        - wrong number of arguments entered 
        - command not uppercase 
        - negative arguments 

TEST PART A: 
    test1     : valid inputs to create a list                 
    test2     : creating strings exceeding 128 buffer limit    
                punctuation and strange characters
    test3     : removing lists after they have been removed 
    test4     : inputs with newline characters, tab characters, scientific notation 
              : viewing list after deletion 

    test5     : creating of lists with only whitespaces
              : scientific notation 

TEST PART B: 
    test1     : valid inputs to create list, add and delete elements from it
    test2     : creating empty list inserting and deleting 
    test3     : deleting everything from list and reinserting different elements 
    test4     : inserting and deleting from a list which was removed 
              : deleting from an empty list 
              : inserting into an empty list 
    test5     : making many lists
              : removing from many lists twice 
              : inserting into removed lists

TEST PART C: 
    test1     : valid inputs to create a nested list 
    test2     : adding multiple nested lists in a list
    test3     : removal and insertion of nested lists 
              : printing lists (not nested lists) with VIEW-NESTED command
    test4     : creating nested in nested in nested list (multiple nested list)
    test5     : creating nested in nested
              : then deleting a nested 