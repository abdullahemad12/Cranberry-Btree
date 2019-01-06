<h1><img src ="docs/Art/cranberries_white_small.png" alt = "cranberries" width="12%">Cranberry Tree</h1>

[![first-timers-only](https://img.shields.io/badge/first--timers--only-friendly-blue.svg?style=flat-square)](https://www.firsttimersonly.com/)
[![Join the chat at https://gitter.im/Cranberry-Tree](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Cranberry-Tree)

A balanced binary search tree written in C with simple interface.According to [wikipedia](https://en.wikipedia.org/wiki/B-tree), a B-tree is a self-balancing tree data structure that keeps data sorted and allows searches, sequential access, insertions, and deletions in logarithmic time. 


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

##### Functions Summary

<table width="100%">
	<tr>
		<th>Return</th>
		<th>Function and Description</th>
	</tr>
	<tr>
		<td>struct cranbtree*</td>
		<td>
			<b><a href="#create">cbt_create(int n)</a></b><br>
			creates a new cranbtree
		</td>
	</tr>
	<tr>
		<td>struct cranbtree*</td>
		<td>
			<b><a href="#clone">cbt_clone(cranbtree_t* cbt)</a></b><br>
			creates a clone of the given cranbtree
		</td>
	</tr>
	<tr>
		<td>struct cranbtree*</td>
		<td>
			<b><a href="#clone">cbt_clone(cranbtree_t* cbt)</a></b><br>
			creates a clone of the given cranbtree
		</td>
	</tr>
	<tr>
		<td>void</td>
		<td>
			<b><a href="#clonedetach">cbt_detach_clone(cranbtree_t* cbt, void* (* copy_object)(void*))</a></b><br>
			Marks a clone tree as a non-cloned tree. Any cloned tree passed to this function will be treated as a normal tree
		</td>
	</tr>
	<tr>
		<td>void</td>
		<td>
			<b><a href="#insert">cbt_insert(cranbtree_t* bt, int key, void* object)</a></b><br>
			inserts a new object in the tree
		</td>
	</tr>
	<tr>
		<td>void*</td>
		<td>
			<b><a href="#upate">cbt_update(cranbtree_t* bt, int key, void* object)</a></b><br>
			updates a specific entry in the tree with the given new object
		</td>
	</tr>
	<tr>
		<td>void*</td>
		<td>
			<b><a href="#search">cbt_search(cranbtree_t* bt, int key)</a></b><br>
			search for a specific object in the tree
		</td>
	</tr>
	<tr>
		<td>void</td>
		<td>
			<b><a href="#visit">cbt_visit_all(cranbtree_t* cbt, void (* visitor) (void *))</a></b><br>
			calls the visitor function on every object stored in the tree
		</td>
	</tr>
	<tr>
		<td>void*</td>
		<td>
			<b><a href="#navigate">cbt_navigation_search(cranbtree_t* cbt, void* key, int (*visitor)(void*, void*))</a></b><br>
			Search the tree according to the rules set by the navigation function
		</td>
	</tr>
	<tr>
		<td>void*</td>
		<td>
			<b><a href="#delete">cbt_delete(cranbtree_t* bt, int key)</a></b><br>
			deletes the object associated with the key from the tree
		</td>
	</tr>
	<tr>
		<td>void</td>
		<td>
			<b><a href="#destroy">cbt_destroy(cranbtree_t* bt, void (* destroy_object)(void*))</a></b><br>
			destroys the given cranbtree and frees all the memory associated with it
		</td>
	</tr>
	<tr>
		<td>void</td>
		<td>
			<b><a href="#print">printTree(cranbtree_t* bt)</a></b><br>
			prints the given tree structure to the terminal windows
		</td>
	</tr>
	<tr>
		<td>int</td>
		<td>
			<b><a href="#errno">cbt_errno(cranbtree_t * bt)</a></b><br>
			returns the error from the last operation (if any)
		</td>
	</tr>
	<tr>
		<td>const char*</td>
		<td>
			<b><a href="#errstr">cbt_errstr(cranbtree_t * bt)</a></b><br>
			returns the error message from the last operation (if any)
		</td>
	</tr>


</table>


<b><a id="create"></a>1. cranbtree_t* cbt_create(int n);</b>

Description: given a number n, Allocates memory for the metadata of the binary.

Parameters: int n: specifies how many keys are allowed per node. It can be any number greater than 3. However, be alarmed that if your not expecting to store a lot of data it maybe wise to choose small n since having large n can with small trees will degrade the performance.

Return value: returns a pointer to the Cranberry Tree struct.


<b><a id="clone"></a>2. cranbtree_t* cbt_clone(cranbtree_t* cbt);</b>

Description: creates an identical clone to the given cranbtree. It will create a new cranbtree with new nodes and new entries in memory that will have the same structure as the given cranbtree. The function will not deep copy the objects being stored in the cranbtree but rather will copy the pointers over from the original. As a result, you will have to take care when freeing or manipulating this objects.

Parameters: cranbtree_t* cbt: the cranbtree to be cloned.

Return value: returns a pointer to the clone of the Cranberry Tree struct.



<b><a id="clonedetach"></a>3. void cbt_detach_clone(cranbtree_t* cbt, void* (* copy_object)(void*));</b>

Description: Detaches a cloned cranbtree_t and use copy_object (If not NULL) to create new copies of the objects stored in the cranbtree and stores the new objects instead of the old ones. Note that, after calling this function, insert, update, delete, destroy will not treat this cranbtree as a clone any more and will be applied normally. It has no effect when called on an already detached cranbtree or a normal cranbtree.

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- void* (* copy_object)(void*): a pointer to a function that takes void* and returns void*. The function should expect an object stored in the tree as a paramter and return a void pointer pointing to a new copy of that object.

**Note**: that cbt_detach_clone will still be applied to the tree even if copy_object is NULL; However, it will not copy any objects and it becomes the responsibility of the user to make sure that destroy will not cause a double free memory corruption.




<b><a id="insert"></a>4. void cbt_insert(cranbtree_t* bt, int key, void* object);</b>

Description: inserts an object into the tree with a search key "key". It will fail if it was called on a cloned cranbtree unless the tree was detached.

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.
- void* object: pointer to the object stored in the tree.

<b><a id="update"></a>5. void* cbt_update(cranbtree_t* bt, int key, void* object);</b>

Description: updates an existing entry in the tree with a new object given the key of the entry. If no entry with such a key was found,
a new entry is inserted. It will fail if it was called on a cloned cranbtree unless the tree was detached. 

parameters: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key that identify the entry.
- void* object: pointer to the new object to replace the old one in the tree.

Return value: returns the pointer of the old object that was removed, or NULL if no such object was found.

NOTE: Do not use this function for insertions if you care about the performance. 

<b><a id="search"></a>6. void* cbt_search(cranbtree_t* bt, int key);</b>

Description: search for an object in the tree with a search key "key".

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.

Return value: returns a pointer to the object that was inserted by the user, or NULL if the key was not found. 

<b><a id="visit"></a>7.void cbt_visit_all(cranbtree_t* cbt, void (* visitor) (void *))</b>

Description: Calls the visitor function on every object pointer stored in the tree. Sets op_errno on errors. 

parameters: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- void (* visitor) (void *): pointer to a function that will be called on every object in the tree

<b><a id="navigate"></a>8. void* cbt_navigation_search(cranbtree_t* cbt, void* key, int (*visitor)(void*, void*));</b>

Description: traverse the tree searching for a specific object using the rules set by the visitor function. In other words, this function uses the visitor function to decide which node to visit next while searching for a specific object. This will be done by calling the function with the following arguments in the following order: key, current object (the object being traversed currently). The function is then expected to return a number and will make a decision based on this number under the following conditions:
1. if the return of visitor is positive the right node is traversed (the node with a greater key)
2. if the return of visitor is negative the left node is traversed (the node with a smaller key)
3. if the return of visitor is zero means the current node contains the target object and the function will return the current object.

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- void* key: search key, that will be used later to retrieve the object.
- int (*visitor)(void*, void*)): a function that will make the traversal decision,

Return value: returns a pointer to the object that was inserted by the user, or NULL if the key was not found. .

<b><a id="delete"></a>9. void* cbt_delete(cranbtree_t* bt, int key);</b>

Description: deletes an object from the tree with a search key "key". It will fail if it was called on a cloned cranbtree unless the tree was detached.

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- int key: search key, that will be used later to retrieve the object.

Return value: returns a pointer to the object that was inserted by the user, or NULL if the key was not found.

<b><a id="destroy"></a>10. void cbt_destroy(cranbtree_t* bt, void (* destroy_object)(void*));</b>

Description: Destroys the cranberry datastructre. Must be called when the cranbtree is no longer in use to avoid memory leaks

parameter: 
- cranbtree_t* bt: pointer to the cranbtree structure.
- void (* destroy_object)(void*)): a pointer to a function that will be called on the object in the database to free it's memory, or destroy it. i.e pass `free`. 

**Note**: the destroy_object function will not be applied on any object in a cranbtree that was created by the cbt_clone() function. If you want the function to be applied, you need to call cbt_detach_clone() first on the cloned cranbtree.

<b><a id="print"></a>11. void printTree(cranbtree_t* bt);</b>

Description: Prints the tree to the screen. can be used for visualization and debugging.

parameters: cranbtree_t* bt: pointer to the cranbtree structure.

<b><a id="errno"></a>12. int cbt_errno(cranbtree_t * bt); </b>

Description: returns an error code that describes the last failure. returns 0 if no failure has occured.

parameters: cranbtree_t* bt: pointer to the cranbtree structure.

<b><a id="errstr"></a>13. const char *cbt_errstr(cranbtree_t * bt); </b>

Description: returns a pointer a string describing the last failure on the cranbtree. returns NULL if no failure has occured.

parameters: cranbtree_t* bt: pointer to the cranbtree structure.

**Note:** Do not attempt to free this pointer.

#### Error codes: 

- No error: CBT_NO_ERROR 0
- Attempt to modify a cloned tree: CBT_CLONE_BAD_OP 1
- The key was not found: CBT_KEY_NOT_FOUND 2

## Installation: 
This section will guide you on how to install the library in your computers depending on your machine, or use it as a portable library.


#### Linux and Unix-like Machines: 
1. Clone this repository or download it as zip and extract it.
2. open up the terminal and `cd` into the project's directory.
3. run: `make`
4. run: `sudo make install`

**compilation**: `clang  your_program.c -lcranbtree -o your_program`

where -lcranbtree tells the compiler where to find the code for the library 

**Uninstall**
To uninstall the library run the following command in the terminal: 
`sudo make uninstall`

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

## Contribution
if you are planning to contribute, it is highly recommended that you read through the docs and go through the [CONTRIBUTING.md](CONTRIBUTING.md) guidlines

#### Opening an issue: 
For this project, there are three main types of issues that you can open: 

1. [Bug Report](.github/ISSUE_TEMPLATE/bug_report.md)
2. [Feature Request](.github/ISSUE_TEMPLATE/feature_request.md)
3. [New Task](.github/ISSUE_TEMPLATE/new-task.md) 

Please follow the format proposed by these templates before opening a new issue.

#### Creating a Pull request 
Please follow that pull request guidelines specified in The [pull Request template](PULL_REQUEST_TEMPLATE.md)


## Contact Me: 

For any inquiries please send me at: **abdullahem1997@hotmail.com**
