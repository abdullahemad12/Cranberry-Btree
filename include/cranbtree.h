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
	void* object;
}cbt_entry_t;

/*
 * Struct for the B+ Tree nodes 
 * The maximum Number of entries n is determined by the users when creating the tree
 * A B+ Tree with n entries can have n+1 pointers
 */

typedef struct cbt_node
{
	cbt_entry_t** entry;
	struct cbt_node** children;
	unsigned int len;
}cbt_node_t;



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
	cbt_node_t* root;
	unsigned int length;
	int max_key;
	int min_key;
	unsigned int n;
}cranbtree_t;



/*interface*/



/*
 * int -> bptree_t* 
 * Given the maximum number of entries in a node
 * returns a pointer to an empty B-tree
 */
cranbtree_t* cbt_create(int n);



/**
  * cranbtree_t*, int, void* -> void
  * EFFECTS: inserts an object in the btree in respect with the search key
  * MODIFIES: cranbtree_t* bt
  * RETURNS: void
  *
  * cranbtree_t* bt: Tree structs that will hold the object
  * int key: search key (choosen by the user)
  * void* object: pointer to the object to be inserted
  */
void cbt_insert(cranbtree_t* bt, int key, void* object);

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
void* cbt_update(cranbtree_t* bt, int key, void* object);


/**
  * cranbtree_t*, int -> void*
  * EFFECTS: search for an object given the search key
  * RETURN: pointer to the object or NULL if the object was 
  *		    not found
  */
void* cbt_search(cranbtree_t* bt, int key);


/**
  * cranbtree_t*, void* -> int
  * REQUIRES: the object to be already inserted in the tree
  * EFFECTS: Figures out the key of a given object in the tree
  * RETURNS: the key of the object, or -1 if the object was not found
  *
  */
int cbt_key_search(cranbtree_t* cbt, void* object);


/**
  * cranbtree_t*, int -> void*
  * EFFECTS: Given a key, removes an object from the tree
  * MODIFIES: cranbtree_t*
  * RETURNS: returns the object or NULL if no such a key was found
  */
void* cbt_delete(cranbtree_t* bt, int key);


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the maximum key in the Tree
  * RETURNS: the maximum key 
  */
int cbt_get_max_key(cranbtree_t* cbt);


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the minimum key in the Tree
  * RETURNS: the minimum key 
  */
int cbt_get_min_key(cranbtree_t* cbt);


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the length of the B-tree
  * RETURNS: the length of the B-tree
  */
int cbt_get_length(cranbtree_t* cbt);


/**
  * cranbtree_t*, (void) destroy_object(void*) -> void
  * MODIFIES: cranbtree_t* bt
  * EFFECTS: Frees all the memory associated with the B tree
  * 
  * (void) destroy_object(void*): a pointer to a function that will be invoked on every object 
  * 				  i.e: object_destroy(object). In case the pointer is NULL, nothing is done 
  *						on the object and it becomes the user's responsibility to free it
  */
void cbt_destroy(cranbtree_t* bt, void (* destroy_object)(void*));
 
 /**
   * cranbtree_t* -> void
   * EFFECTS: prints the nodes of the tree to the screen for visualization
   */
void printTree(cranbtree_t* bt);
 

 

 #endif /* _BTREE_H_ */
