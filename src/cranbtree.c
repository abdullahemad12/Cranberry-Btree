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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <cranbtree.h>

/*
 * library includes
 */
#include "lib/insert.c"
#include "lib/search.c"
#include "lib/delete.c"
#include "lib/node.c"
#include "lib/statistics.c"
#include "lib/helpers.c"
#include "lib/update.c"
#include "lib/clone.c"
#include "lib/visit.c"
/*
 * prototypes
 */
static void destroy_bt_helper(cbt_node_t * root, int n, void (*done) (void *));

/*
 * cranbtree_t* -> int
 * Return last detected error code/number
 */
int cbt_errno(cranbtree_t * bt)
{
	assert(bt != NULL);

	return bt->op_errno;
}

/*
 * cranbtree_t* -> const char*
 * Return pointer to string describing last error. NULL if no error recorded.
 */
const char *cbt_errstr(cranbtree_t * bt)
{
	assert(bt != NULL);

	return errorMessages[bt->op_errno];
}

/*
 * int -> bptree_t* 
 * Given the maximum number of entries in a node
 * returns a pointer to an empty B-tree
 */
cranbtree_t *cbt_create(int n)
{
	if (n < 2)
	{
		return NULL;
	}
	/*
	 * must be odd (reacts internally without informing the user)
	 */
	if (n % 2 == 0)
	{
		n++;
	}

	cranbtree_t *bt = malloc(sizeof(cranbtree_t));

	if (bt == NULL)
	{
		return NULL;
	}
	bt->root = NULL;
	bt->length = 0;
	bt->min_key = -1;
	bt->max_key = -1;
	bt->n = n;
	bt->is_clone = false;
	bt->op_errno = CBT_NO_ERROR;

	return bt;
}

/**
  * cranbtree_t*, void* (*)(void*) -> cranbtree_t*
  * EFFECTS: clones the given cranbtree_t and clones the user objects if the
  *                     the passed function pointer is not NULL
  * RETURNS: a pointer to the clone, or NULL if the given cbt is not valid
  * NOTE: if the function is NULL, clone will copy the original object pointers to the new 
  *                  clone . This you must be careful in this case when manipulating the objects and destroying
  *                  them. Otherwise, the tree will store copies of the objects
  */
cranbtree_t *cbt_clone(cranbtree_t * cbt)
{
	if (cbt == NULL)
	{
		return NULL;
	}
	assert(cbt->n > 2);
	assert(cbt->n % 2 != 0);
	cranbtree_t *clone = malloc(sizeof(cranbtree_t));

	if (clone == NULL)
	{
		return NULL;
	}
	cbt_copy_metadata(cbt, clone);
	clone->root = cbt_copy_nodes(cbt->root, cbt->n);
	return clone;
}

/**
  * cranbtree_t*, int, void* -> void
  * EFFECTS: Inserts an object in the btree in respect with the search key.
  * 		 Unless the tree is a clone, in which case it return immediately.
  * 		 Sets op_errno on any error.
  * MODIFIES: cranbtree_t* bt
  * RETURNS: Pointer to the inserted object or NULL on any error
  *
  * cranbtree_t* bt: Tree structs that will hold the object
  * int key: search key (choosen by the user)
  * void* object: pointer to the object to be inserted
  */
void *cbt_insert(cranbtree_t * bt, int key, void *object)
{
	assert(bt != NULL);
	assert(object != NULL);

	if (bt->is_clone)
	{
		bt->op_errno = CBT_CLONE_BAD_OP;
		return (cranbtree_t *) NULL;
	}

	cbt_entry_t *entry = bt_create_entry(key, object);

	bt_insert_helper(bt, bt->root, entry);

	/*
	 * updates the min and max if needed
	 */
	if (bt->length == 0)
	{			/* first insertion */
		bt->max_key = key;
		bt->min_key = key;
	}
	else
	{
		bt->max_key = bt->max_key < key ? key : bt->max_key;
		bt->min_key = bt->min_key > key ? key : bt->min_key;
	}
	bt->length++;

	return object;
}

/**
  * cranbtree_t*, int, void* -> void
  * MODIFIES: cranbtree_t* 
  * EFFECTS: Updates the object of the entry that has the specified key with the new pointer. If 
  * 		 no such entry was found a new entry is inserted. Returns immediately if the tree is
  * 		 a clone. Sets op_errno on any error.
  * RETURNS: Pointer to the old object, NULL if it was not found.
  * PARAMETERS: 
  * - cranbtree_t* cbt, pointer to the BTree struct
  * - int key: the key of the entry to be updated
  * - void* object: pointer to the object
  */
void *cbt_update(cranbtree_t * bt, int key, void *object)
{
	assert(bt != NULL);

	if (bt->is_clone)
	{
		bt->op_errno = CBT_CLONE_BAD_OP;
		return (void *)NULL;
	}

	void *old_object = cbt_update_helper(bt->root, key, object, bt->n);

	if (old_object == NULL)
	{
		bt->op_errno = CBT_KEY_NOT_FOUND;
		cbt_insert(bt, key, object);
	}
	return old_object;
}

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
void *cbt_update_if_exists(cranbtree_t * bt, int key, void *object)
{
	return NULL;
}

/**
  * cranbtree_t*, void* -> int
  * REQUIRES: the object to be already inserted in the tree
  * EFFECTS: Figures out the key of a given object in the tree
  * RETURNS: the key of the object, or -1 if the object was not found
  *
  */
int cbt_key_search(cranbtree_t * cbt, void *object)
{
	return 0;
}

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: search for an object given the search key
  * RETURN: pointer to the object or NULL if the object was 
  *		    not found
  */
void *cbt_search(cranbtree_t * bt, int key)
{
	assert(bt != NULL);
	void *object = bt_search_helper(bt->root, key, bt->n);

	if (object == NULL)
	{
		bt->op_errno = CBT_KEY_NOT_FOUND;
	}
	return object;
}

/**
  * cranbtree_t*, void* key,  int (*)(void*) -> void*
  * EFFECTS: will navigate the tree searching for a specific item using the rules set by the visitor
  *                    function
  * REQUIRES: the function "visitor" to return an integer according to the following rules
  *                      1. positive: means the node to look at next is the right one.
  *                      2. negative: means the node to look at next is the left one.
  *                      3. zero: means the item was found, no further traversal is needed.
  *                      the visitor will be passed the arguments in the following order: key, current object
  * RETURNS: the item we are looking for, or NULL if the item was not found
  * PARAMETERS: 
  * - cranbtree_t* cbt: cranberry tree to be traversed
  * - void* key: a key that will be passed to visitor to make a decision
  * - int (*visitor)(void*, void*): visitor function that will be invoked on every object in the tree
  */
void *cbt_navigation_search(cranbtree_t * cbt, void *key,
			    int (*visitor) (void *, void *))
{
	assert(cbt != NULL);
	assert(visitor != NULL);
	return cbt_navigation_search_helper(cbt->root, key, visitor);
}

/**
  * crabtree_t* , void (*) (void *)  -> void
  * EFFECTS: Calls the visitor function on every object pointer stored in the tree
  * MODIFIES: cranbtree.objects
  * REQUIRES: The  visitor function to take a void* as an argument and to return nothing 
  * PARAMETERS: 
  * - cranbtree_t* cbt: the tree to be traversed
  * - void (* visitor) (void *): the function that will be invoked on every object in the tree  
  */
void cbt_visit_all(cranbtree_t * cbt, void (*visitor) (void *))
{
	assert(cbt != NULL);
	cbt_visit_all_helper(cbt->root, visitor);
}

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: Given a key, removes an object from the tree. Sets errno on any error.
  * MODIFIES: cranbtree_t*
  * RETURNS: returns the object or NULL if no such a key was found.
  */
void *cbt_delete(cranbtree_t * bt, int key)
{
	assert(bt != NULL);

	if (bt->is_clone)
	{
		bt->op_errno = CBT_CLONE_BAD_OP;
		return (void *)NULL;
	}

	void *object = bt_delete_helper(bt, bt->root, key);

	if (object != NULL)
	{
		bt->max_key =
		    bt->max_key ==
		    key ? cbt_calculate_max_key(bt->root) : bt->max_key;
		bt->min_key =
		    bt->min_key ==
		    key ? cbt_calculate_min_key(bt->root) : bt->min_key;
		bt->length--;
	}
	else
	{
		bt->op_errno = CBT_KEY_NOT_FOUND;
	}

	return object;
}

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the maximum key in the Tree
  * RETURNS: the maximum key 
  */
int cbt_get_max_key(cranbtree_t * cbt)
{
	return cbt->max_key;
}

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the minimum key in the Tree
  * RETURNS: the minimum key 
  */
int cbt_get_min_key(cranbtree_t * cbt)
{
	return cbt->min_key;
}

/**
  * cranbtree_t* -> int
  * EFFECTS: gets the length of the B-tree
  * RETURNS: the length of the B-tree
  */
unsigned int cbt_get_length(cranbtree_t * cbt)
{
	return cbt->length;
}

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
void cbt_detach_clone(cranbtree_t * cbt, void *(*copy_object) (void *))
{
	/* if there is no tree do nothing */
	if (cbt == NULL)
	{
		return;
	}

	/* this tree is no longer a clone, mark it as such */
	cbt->is_clone = false;

	/* copy the objects starting from the root node recursively */
	cbt_copy_objects(cbt->root, cbt->n, copy_object);
}

/**
  * cranbtree_t*, (void) destroy_object(void*) -> void
  * MODIFIES: cranbtree_t* bt
  * EFFECTS: Frees all the memory associated with the B tree
  * 
  * (void) destroy_object(void*): a pointer to a function that will be invoked on every object 
  * 				  i.e: object_destroy(object). In case the pointer is NULL, nothing is done 
  *						on the object and it becomes the user's responsibility to free it
  */
void cbt_destroy(cranbtree_t * bt, void (*destroy_object) (void *))
{
	if (bt->is_clone)
	{
		destroy_object = NULL;
	}
	destroy_bt_helper(bt->root, bt->n, destroy_object);
	free(bt);
}

void printTree(cranbtree_t * bt)
{
	printf("\n\n");
	for (int i = 0, n = calculate_depth(bt->root); i < n; i++)
	{
		print_level(bt->root, bt->n, i, 0);
		printf("\n\n");
	}

}

/*******************************************
 *                                         *
 *         Static private Functions        * 
 *                                         *
 *******************************************/

static void destroy_bt_helper(cbt_node_t * root, int n, void (*done) (void *))
{
	if (root == NULL)
	{
		return;
	}
	for (int i = 0, n1 = n + 1; i < n1; i++)
	{
		destroy_bt_helper(root->children[i], n, done);
	}
	bt_destroy_node(root, n, done);
}
