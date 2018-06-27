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

/*library includes*/
#include "lib/insert.c"
#include "lib/search.c"
#include "lib/delete.c"
#include "lib/node.c"
#include "lib/statistics.c"
#include "lib/helpers.c"


/*prototypes*/
static void destroy_bt_helper(cbt_node_t* root, int n, void (* done)(void*));


/*
 * int -> bptree_t* 
 * Given the maximum number of entries in a node
 * returns a pointer to an empty B-tree
 */
cranbtree_t* cbt_create(int n)
{
	if(n < 2)
	{ 
		return NULL;
	}
	/*must be odd (reacts internally without informing the user)*/
	if(n % 2 == 0)
	{
		n++;
	}
	
	cranbtree_t* bt = malloc(sizeof(cranbtree_t));
	if(bt == NULL)
	{
		return NULL;
	}
	bt->root = NULL;
	bt->length = 0;
	bt->max_key = -1;
	bt->n = n;
	return bt;
}
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
void cbt_insert(cranbtree_t* bt, int key, void* object)
{
	assert(bt != NULL);
	assert(object != NULL);
	
	cbt_entry_t* entry = bt_create_entry(key, object);
	bt_insert_helper(bt, bt->root, entry);
	
	/*updates the min and max if needed*/
	bt->max_key = bt->max_key < key ? key : bt->max_key;
	bt->min_key = bt->min_key > key ? key : bt->min_key;
	bt->length++;
}


/**
  * cranbtree_t*, void* -> int
  * REQUIRES: the object to be already inserted in the tree
  * EFFECTS: Figures out the key of a given object in the tree
  * RETURNS: the key of the object, or -1 if the object was not found
  *
  */
int cbt_key_search(cranbtree_t* cbt, void* object)
{
	return 0;
}

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: search for an object given the search key
  * RETURN: pointer to the object or NULL if the object was 
  *		    not found
  */
void* cbt_search(cranbtree_t* bt, int key)
{
	assert(bt != NULL);
	return bt_search_helper(bt->root, key, bt->n);
}

/**
  * cranbtree_t*, int -> void*
  * EFFECTS: Given a key, removes an object from the tree
  * MODIFIES: cranbtree_t*
  * RETURNS: returns the object or NULL if no such a key was found
  */
void* cbt_delete(cranbtree_t* bt, int key)
{
	assert(bt != NULL);
	void* object = bt_delete_helper(bt, NULL, bt->root, key);
	
	if(object != NULL)
	{
		bt->max_key = bt->max_key == key ? cbt_calculate_max_key(bt->root) : bt->max_key;
		bt->min_key = bt->min_key == key ? cbt_calculate_min_key(bt->root) : bt->min_key;
		bt->length--;
	}
	
	return object;
}


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the maximum key in the Tree
  * RETURNS: the maximum key 
  */
int cbt_get_max_key(cranbtree_t* cbt)
{
	return 0;
}


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the minimum key in the Tree
  * RETURNS: the minimum key 
  */
int cbt_get_min_key(cranbtree_t* cbt)
{
	return 0;
}


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the length of the B-tree
  * RETURNS: the length of the B-tree
  */
int cbt_get_length(cranbtree_t* c)
{
	return 0;
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
void cbt_destroy(cranbtree_t* bt, void (* destroy_object)(void*))
{
	destroy_bt_helper(bt->root, bt->n, destroy_object);
	free(bt);
}
 
 
void printTree(cranbtree_t* bt)
{
	printf("\n\n");
	for(int i = 0, n = calculate_depth(bt->root); i < n; i++)
	{
			print_level(bt->root, bt->n, i, 0);
			printf("\n\n");
	}


}						

/*******************************************
 *								 		   *
 *		 Static private Functions          *
 *								 		   *
 *******************************************/


static void destroy_bt_helper(cbt_node_t* root, int n, void (* done)(void*))
{
	if(root == NULL)
	{
		return;
	}
	for(int i = 0, n1 = n + 1; i < n1; i++)
	{
		destroy_bt_helper(root->children[i], n, done);
	}
	bt_destroy_node(root, n, done);
}
