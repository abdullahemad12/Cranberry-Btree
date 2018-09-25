/** MIT License
  *
  * Copyright (c) 2018 Abdullah Emad
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  */

#ifndef _BTREE_H_
#define _BTREE_H_
#include <stdbool.h>

/*
 * error numbers
 */
#define CBT_NO_ERROR        0
#define CBT_CLONE_BAD_OP    1
#define CBT_KEY_NOT_FOUND   2

/*
 * error messages
 */

static char *errorMessages[] = {
	NULL,
	"Cannot perform this operation on a cloned tree",
};

/*
 * Struct for the B-tree node's entries;
 * All the nodes in the left subtree must have value >= this.key
 * All the nodes in the right subtree must have value < this.key
 *
 * Keys for the B-tree are assumed to be positive, otherwise the entry is treated as invalid
 */
typedef struct cbt_entry
{
	int key;
	void *object;
} cbt_entry_t;

/*
 * Struct for the B+ Tree nodes 
 * The maximum Number of entries n is determined by the users when creating the tree
 * A B+ Tree with n entries can have n+1 pointers
 */

typedef struct cbt_node
{
	cbt_entry_t **entry;
	struct cbt_node **children;
	unsigned int len;
} cbt_node_t;

/*
 * struct for the Btree
 * 
 * bpt_node_t* root: pointer to the root of the tree
 * unsigned int length: number of nodes in the btree
 * int max_key: the maximum key in the BTree
 * unsinged int n: the maximum number of entries in a node
 */

typedef struct cranbtree
{
	cbt_node_t *root;
	unsigned int length;
	int max_key;
	int min_key;
	unsigned int n;
	bool is_clone;
	int op_errno;
} cranbtree_t;

/*interface*/

/*
 * cranbtree_t* -> int
 * Return pointer to string describing last error. NULL if no error recorded.
 */
int cbt_errno(cranbtree_t * bt);

/*
 * cranbtree_t* -> const char* 
 * Return pointer to string describing last error. NULL if no error recorded.
 */
const char *cbt_errstr(cranbtree_t * bt);

/*
 * int -> bptree_t* 
 * Given the maximum number of entries in a node
 * returns a pointer to an empty B-tree
 */
cranbtree_t *cbt_create(int n);

/**
  * cranbtree_t* -> cranbtree_t*
  * EFFECTS: clones the given cranbtree_t into a new cranbtree_t with new nodes and entries.
  * 		 However, the old object pointers are copied over with no modifications to them.
  * RETURNS: a pointer to the clone, or NULL if the given cbt is not valid
  *
  */
cranbtree_t *cbt_clone(cranbtree_t * cbt);

/**
  * cranbtree_t*, int, void* -> void
  * EFFECTS: Inserts an object in the btree in respect with the search key.
  *          Unless the tree is a clone, in which case it return immediately.
  *          Sets op_errno on any error.
  * MODIFIES: cranbtree_t* bt
  * RETURNS: Pointer to the inserted object or NULL on any error
  *
  * cranbtree_t* bt: Tree structs that will hold the object
  * int key: search key (choosen by the user)
  * void* object: pointer to the object to be inserted
  */
void *cbt_insert(cranbtree_t * bt, int key, void *object);

/**
  * cranbtree_t*, int, void* -> void
  * MODIFIES: cranbtree_t* 
  * EFFECTS: Updates the object of the entry that has the specified key with the new pointer. If 
  * 		 no such entry was found a new entry is inserted.
  * RETURNS: pointer to the old object, NULL if it was not found
  * PARAMETERS: 
  * - cranbtree_t* cbt, pointer to the BTree struct
  * - int key: the key of the entry to be updated
  * - void* object: pointer to the object
  */
void *cbt_update(cranbtree_t * bt, int key, void *object);

/**
  * cranbtree_t*, int, void* -> void
  * MODIFIES: cranbtree_t* 
  * EFFECTS: Updates the object of the entry that has the specified key with the new pointer only if it 
  * exists. 
  * PARAMETERS: 
  * - cranbtree_t* cbt, pointer to the BTree struct
  * - int key: the key of the entry to be updated
  * - void* object: pointer to the object
  * RETURNS: pointer to the old object, or NULL if it does not exist
  */
void *cbt_update_if_exists(cranbtree_t * bt, int key, void *object);

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: search for an object given the search key
  * RETURN: pointer to the object or NULL if the object was 
  *		    not found
  */
void *cbt_search(cranbtree_t * bt, int key);

/**
  * cranbtree_t*, void* -> int
  * REQUIRES: the object to be already inserted in the tree
  * EFFECTS: Figures out the key of a given object in the tree
  * RETURNS: the key of the object, or -1 if the object was not found
  *
  */
int cbt_key_search(cranbtree_t * cbt, void *object);

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: Given a key, removes an object from the tree
  * MODIFIES: cranbtree_t*
  * RETURNS: returns the object or NULL if no such a key was found
  */
void *cbt_delete(cranbtree_t * bt, int key);

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the maximum key in the Tree
  * RETURNS: the maximum key 
  */
int cbt_get_max_key(cranbtree_t * cbt);

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the minimum key in the Tree
  * RETURNS: the minimum key 
  */
int cbt_get_min_key(cranbtree_t * cbt);

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the length of the B-tree
  * RETURNS: the length of the B-tree
  */
unsigned int cbt_get_length(cranbtree_t * cbt);

/**
  * cranbtree_t*, void* (* copy_object)(void*) -> void
  * EFFECTS: detaches a cloned cranbtree_t from its parent. If copy_object is not NULL, cbt_detach_clone 
  * will use it to make a new copy of every object in the tree.
  * MODIFIES: cranbtree_t* cbt
  * PARAMETERS: 
  * - cranbtree_t* cbt: a pointer to a cloned cranbtree
  * - void* (* copy_object)(void*): a pointer to a function (void* -> void*) that will take the old object as a parameter
  *									and returns a pointer to a copy of the object. This will be invoked on every object in the cranbtree 
  *
  * NOTE: This should generally be used when your cloned cranbtree will start storing objects different 
  * 	   then those of the original; However, when using it, it becomes your responsibility to free 
  *		   and manipulate objects correctly
  */
void cbt_detach_clone(cranbtree_t * cbt, void *(*copy_object) (void *));

/**
  * cranbtree_t*, (void) destroy_object(void*) -> void
  * MODIFIES: cranbtree_t* bt
  * EFFECTS: Frees all the memory associated with the B tree
  * 
  * (void) destroy_object(void*): a pointer to a function that will be invoked on every object 
  * 				  i.e: object_destroy(object). In case the pointer is NULL, nothing is done 
  *						on the object and it becomes the user's responsibility to free it
  */
void cbt_destroy(cranbtree_t * bt, void (*destroy_object) (void *));

 /**
   * cranbtree_t* -> void
   * EFFECTS: prints the nodes of the tree to the screen for visualization
   */
void printTree(cranbtree_t * bt);

#endif				/* _BTREE_H_ */
