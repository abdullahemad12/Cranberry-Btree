---
name: New Task
about: Description for a new task or a new feature to be implemented

---

# Task 
A description of the task or the feature implemented. ** Be precise, stick to the point and be clear.**

# Method 
Describe How to implement this feature and how some ways to implemented are better than the others.

# Prototype
place a prototype for the interface function here. This will be added to include/cranbtree.h as well.

```
/**
  * arg1, arg2,arg3 , .... , argn -> return Type
  * EFFECTS: what effects will this function have on the tree (avoid specifying the how)
  * MODIFIES: cranbtree  (Only include if the function will modify the tree)
  * REQUIRES: any state that the cranbtree should be in before calling this function. 
  *                      (avoid including obvious states like 'the cranbtree should not be NULL')
  * RETURNS: what does this function return (do not include if the function does not return)
  * PARAMETERS: 
  * - DataType arg1: description
  * - DataType arg2: description
  * - ......
  * Include if you need to specifiy something special about the paramaters
  * NOTES: any additional notes or pointers when using this function
  */
function prototype e.g:
void cbt_insert(cranbtree* cbt, int key,  void* object)
```
# Where to code
This section will include the files where the code should go.
For example: 
- the interface function will go to src/cranbtree.c 
- any helpers will go to src/lib/insert.c

# Tests 

Specify any set of tests the needs to be implemented in order to test the new functionality, or any set of tests already implemented and the new functionalities needs to pass.
