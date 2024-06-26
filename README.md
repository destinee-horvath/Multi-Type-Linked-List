# About Project
To create a linked lists that can store elements of multiple data types while preserving their actual data types. The program takes in commands from stdin to create and manage these multi-type linked lists. 

# Commands
### NEW
To create a new list with a specific number of initial elements:

`NEW <number of elements>`

For example, to create a list: 

Input: 
```
NEW 3
1
donuts
c
```

Output: 
```
LIST 0: 1 -> donuts -> c
```

Additionally, to create a nested list: 

Input: 
```
NEW 3
first
{0}
last
```

Output: 
```
List 1: first -> {List 0} -> last
```

### VIEW 
To view a list by its index: 

`VIEW <list index>`

For example, to view the list with index 0: 

Input:
```
VIEW 0
```

Output: 
```
1 -> donuts -> c
```

### TYPE
To view the data types of each element in a list of index: 

`TYPE <list index>`

Identifiable data types include: 
- `int` : integer
- `float` : float
- `char` : character
- `string` : string

For example, to see the data types of elements in list 0 with index 0:

Input: 
```
TYPE 0
```

Output: 
```
int -> string -> char
```

### VIEW ALL 
To view all the lists in order of creation: 

`VIEW ALL`

For example: 

Input: 
```
VIEW ALL
```

Output: 
```
List 0: 1 -> donuts -> c
List 1: List 1: first -> {List 0} -> last
```

### VIEW-NESTED
To view the sublists of a nested list: 

`VIEW-NESTED <list index>`

For example: 

Input: 
```
VIEW-NESTED 1
```

Output:
```
first -> {1 -> donuts -> c} -> last
```

### REMOVE
To remove a list with index: 

`REMOVE <list index>`

For example to remove a list with index 1:

Input: 
```
REMOVE 1
```

Output: 
```
List 1 has been removed.

Number of Lists: 1
List 0
```

### INSERT
To insert an element into an existing list: 

`INSERT <list id> <index> <value>`

For example, to insert "3.14" into list 0:

Input: 
```
INSERT 0 1 3.14
```

Output: 
```
List 0: 1 -> 3.14 -> donuts -> c
```

### DELETE 
To delete an element of index from an existing list:

`DELETE <list id> <index>`

For example, to delete "3.14" from list 0:

Input: 
```
DELETE 0 1
```

Output:
```
List 0: 1 -> donuts -> c
```

Additionally, if the index is negative, indexing will begin at the end of the list. For example if list 1 was `1 -> 2 -> 3 -> 4`

Input: 
```
DELETE 1 -1
```

Output: 
```
List 1: 1 -> 2 -> 3
```

### Invalid Commands 
For any command that is invalid: 
Output: `INVALID COMMAND: <command entered>`

