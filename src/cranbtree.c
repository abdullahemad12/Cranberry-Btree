/** Cranberry Tree Copyright (C) 2018  Abdullah Emad
  * This program comes with ABSOLUTELY NO WARRANTY. 
  * This is free software, and you are welcome to redistribute it
  * under certain conditions.
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
	return bt_delete_helper(bt, NULL, bt->root, key);
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
