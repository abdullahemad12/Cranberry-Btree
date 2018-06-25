<img src ="docs/cranberries_white.png" alt = "cranberries" width="10%">
# Cranberry Tree

A balanced binary search tree written in C with an easy to use interface.According to [wikipedia](https://en.wikipedia.org/wiki/B-tree), a B-tree is a self-balancing tree data structure that keeps data sorted and allows searches, sequential access, insertions, and deletions in logarithmic time. 


## Supported operations: 

The current version of Cranberry Tree supports only the following main operations: 

1. Creation: creating a binary tree object in memory (allocating memory for the tree).
2. Insertions: inserting an object, or a pointer to the data you want to store in memory with a specific unique key.
3. Deletions: remoeving an object from the binary tree
4. destroying: destroy all the memory associated with the tree, with the ability to destroy all the objects that still resist in the tree.

_**important notice**_: the current version does not scale very well with duplicates key and might crash or panic when deleting from a large tree that contains many duplicates. Future updates may support key duplications.

## interface: 
This section contains description of the interface

#### Struct: 

- struct cranbtree A.K.A: cranbtree_t: holds important metadata about the tree and will be used by the user to perform any operation on the tree.

#### functions

<b>1. cranbtree_t* cbt_create(int n);</b>

Description: given a number n, Allocates memory for the metadata of the binary.

Parameters: int n: specifies how many keys are allowed per node. It can be any number greater than 3. However, be alarmed that if your not expecting to store a lot of data it maybe wise to choose small n since having large n can with small trees will degrade the performance.

Return value: returns a pointer to the Cranberry Tree struct.


<b>2. void cbt_insert(cranbtree_t* bt, int key, void* object);</b>

Description: inserts an object into the tree with a search key "key".

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.
- void* object: pointer to the object stored in the tree.


<b>3. void* cbt_search(cranbtree_t* bt, int key);</b>

Description: search for an object in the tree with a search key "key".

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.

Return value: returns a pointer to the object that was inserted by the user, or NULL if the key was not found. .

<b>4. void* cbt_delete(cranbtree_t* bt, int key);</b>

Description: deletes an object from the tree with a search key "key".

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.

Return value: returns a pointer to the object that was inserted by the user, or NULL if the key was not found.

<b>5. void cbt_destroy(cranbtree_t* bt, void (* destroy_object)(void*));</b>

Description: Destroys cranberry datastructre

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- void (* destroy_object)(void*)): a pointer to a function that will be called on the object in the database to free it's memory, or destroy it. i.e pass `free`. 

<b>6. void printTree(cranbtree_t* bt);</b>

Description: Prints the tree to the screen. can be used for visualization and debugging.

parameters: cranbtree_t* bt: pointer to the cranbtree structure.


## Installation: 
This section will guide you on how to install the library in your computers depending on your machine, or use it as a portable library.


#### Linux and Unix-like Machines: 
1. Clone this repository or download it as zip and extract it.
2. open up the terminal and `cd` into the project's directory.
3. run: `make`
4. run: `sudo make install`

**compilation**: `clang  your_program.c -lcranbtree -o your_program`

where -lcranbtree tells the compiler where to find the code for the library 

#### portable library: 
1. Clone this repository or download it as zip and extract it.
2. open up the terminal and `cd` into the project's directory.
3. run: `make`
4. this creates an object file and library files which can be used and linked to your own project

**compilation**: `clang  your_program.c -L. -lcranbtree -o your_program`
where the .a library file should be placed in the compiling directory.


## Usage Example

```
#include <stdio.h>
#include <stdlib.h>
#include <cranberry/cranbtree.h>


int main(void)
{
	cranbtree_t* bt = cbt_create(3);

	int z = 0; 
	cbt_insert(bt, 3, &z); // inserts with key = 3
	printTree(bt);
	cbt_delete(bt, 3);	
	cbt_destroy(bt, NULL);

}
```

## Contact Me: 

For any inquiries please send me at: **abdullahem1997@hotmail.com**
