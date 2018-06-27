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

/**
  * functions that operate on cbt_node_t
  * 
  */


#include "lib.h"

 /**
  * int n -> cbt_node_t*
  * Creates a node according to the size n
  *
  */
static cbt_node_t* bt_create_node(int n)
{
	if(n < 3)
	{
		return NULL;
	}
	
	cbt_node_t* node = malloc(sizeof(cbt_node_t));
	if(node == NULL)
	{
		return NULL;
	}
	size_t entry_size = sizeof(cbt_entry_t*) * n;
	node->entry = calloc(1, entry_size);
	if(node->entry == NULL)
	{
		free(node);
		return NULL;
	}

	
	size_t children_size =  sizeof(cbt_node_t*) * (n+1);
	node->children = calloc(1, children_size);
	if(node->children == NULL)
	{
		free(node->entry);
		free(node);
		return NULL;
	}
	node->len = 0;
	return node;
}

/*
 * cbt_node_t*, int, int 
 * EFFECTS: calculates the index of the entry in the node entry array 
 * REQUIRES: the entries in the array to be packed together to the left and no of entries > 0
 * RETURNS: an index or -1 on error
 */
static int get_next_node_index(cbt_node_t* node, int key, int n)
{
	assert(node != NULL);

	
	if(node->entry[0] == NULL)
	{
		return -1;
	}
	
	if(node->entry[0]->key > key)
	{
		return 0;
	}
	for(int i = 0, nm1 = n-1; i < nm1; i++)
	{
		if(node->entry[i]->key <= key && (node->entry[i+1] == NULL || key < node->entry[i+1]->key))
		{
			return i + 1;
		}
	}
	
	return n;
}

/*
 * cbt_node_t*, int -> bool
 * return true if the node has no room for more entries
 */
static bool is_full_node(cbt_node_t* node, int n)
{
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] == NULL)
		{
			return false;
		}		
		assert(node->entry[i] != NULL);
	}
	return true;
}

/*
 * cbt_node_t*, int n
 * MODIFIES: cbt_node_t*
 * EFFECTS: shifts all the entries and children in the array to the right 
 *			starting from position i and i+1 respectively
 * REQUIRES: the node should not be full, otherwise the rightmost element will be orphaned "i < n - 1"
 * RETURNS: void
 *
 */
static void node_shift_right(cbt_node_t* node, int i,  int n)
{
	for(int j = n - 1; j > i; j--)
	{
		node->entry[j] = node->entry[j - 1];
		node->children[j+1] = node->children[j]; 
	}
	node->entry[i] = NULL;
	node->children[i+1] = NULL;
}

/*
 * cbt_node_t*, int n
 * MODIFIES: cbt_node_t*
 * EFFECTS: shifts all the entries in the array to the right
 * REQUIRES: the node should not be full, otherwise the rightmost element will be orphaned "i < n - 1"
 * RETURNS: void
 *
 */
static void node_shift_right_without_children(cbt_node_t* node, int i,  int n)
{
	for(int j = n - 1; j > i; j--)
	{
		node->entry[j] = node->entry[j - 1];
	}
	node->entry[i] = NULL;
}


/*
 * cbt_node_t*, int n
 * MODIFIES: cbt_node_t*
 * EFFECTS: shifts all the entries and children in the array to the left 
 *			starting from position i and i-1 respectively
 * REQUIRES:  position i be NULL , otherwise the the i'th element will be orphaned "0 =< i < n-1"
 * RETURNS: void
 *
 */
static void node_shift_left(cbt_node_t* node, int i,  int n)
{
	
	for(int j = i + 1; j < n; j++)
	{
		node->entry[j - 1] = node->entry[j];
		node->children[j-1] = node->children[j]; 
	}
	node->children[n-1] = node->children[n]; 
	node->entry[n-1] = NULL;
	node->children[n] = NULL;
	
}

 /*
 * cbt_node_t*, int n
 * MODIFIES: cbt_node_t*
 * EFFECTS: shifts all the entries in the array to the left 
 * REQUIRES:  position i be NULL , otherwise the the i'th element will be orphaned "0 =< i < n-1"
 * RETURNS: void
 *
 */
static void node_shift_left_without_children(cbt_node_t* node, int i,  int n)
{
	
	for(int j = i + 1; j < n; j++)
	{
		node->entry[j - 1] = node->entry[j];
	}
	node->entry[n-1] = NULL;
	
}


/**
  * cbt_node_t*, cbt_node_t* -> cbt_node_t*
  * EFFECTS: gets the right sibling of a node
  * RETURNS: the right sibling of a node, or NULL if doesn't have one
  */
static cbt_node_t* get_right_sibling(cbt_node_t* parent, cbt_node_t* node)
{
	if(parent->children[parent->len] == node)
	{
		return NULL;
	}
	
	for(int i = 0; i < parent->len; i++)
	{
		if(parent->children[i] == node)
		{
			return parent->children[i+1];
		}
	}
	return NULL;
}

/**
  * cbt_node_t*, cbt_node_t* -> cbt_node_t*
  * EFFECTS: gets the left sibling of a node
  * RETURNS: the left sibling of a node, or NULL if doesn't have one
  */
static cbt_node_t* get_left_sibling(cbt_node_t* parent, cbt_node_t* node)
{

	if(parent->children[0] == node)	
	{
		return NULL;
	}
	
	for(int i = 1, n1 = parent->len+1; i < n1 ; i++)
	{
		if(parent->children[i] == node)
		{
			return parent->children[i-1];
		}
	}
	return NULL;
}

/*
 * get_last_entry_index
 */
static int get_last_entry_index(cbt_node_t* node, int n)
{
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] == NULL)
		{
			return i - 1;
		}
	}
	return n - 1;
}

/*
 * cbt_node_t*, void (* done)(int) -> void
 * destroys all the data associated with a given node
 */
 
static void bt_destroy_node(cbt_node_t* node, int n, void (* done)(void*))
{
	assert(node != NULL);
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] != NULL)
		{
			bt_destroy_entry(node->entry[i], done);
			node->entry[i] = NULL;
		}
	}
	free(node->entry);
	free(node->children);
	free(node);
}




/*****************************
 *							 *
 *	cbt_entry_t node helpers  *
 *							 *
 *****************************/


/*
 * int key, void* object -> bvt_entry_t*
 * Given the key and the object, creates a node entry
 */
static cbt_entry_t* bt_create_entry(int key, void* object)
{
	assert(object != NULL);
	assert(key >= 0);
	cbt_entry_t* entry = calloc(1, sizeof(cbt_entry_t));
	if(entry == NULL)
	{
		return NULL;
	}
	entry->key = key;
	entry->object = object;
	return entry;
}


/*
 * cbt_node_t*, cbt_entry_t*, int -> int
 * MODIFIES: cbt_node_t* node
 * EFFECTS: inserts an entry in the node preserving the the order of the entries. i.e 
 * 			keeping the node sorted after the insertions
 * REQUIRES: the node should not be full
 * RETURNS: returns the index of the child corresponding to the new inserted entry on success, or
 * 			-1 on Failure
 * if bool shift is true then the children are shifted
 */
static int node_insert_entry(cbt_node_t* node, cbt_entry_t* entry, bool shift,  int n)
{
	assert(node != NULL);
	assert(entry != NULL);
	assert( n > 2);
	assert(!is_full_node(node, n));
	
	node->len++;
	for(int i = 0; i < n; i++)
	{
		/*reached the end of the entries list*/
		if(node->entry[i] == NULL)
		{
			node->entry[i] = entry;
			return i + 1;
		}
		/*found the location*/
		if(node->entry[i]->key > entry->key)
		{
			/*shift and insert*/
			if(shift)
				node_shift_right(node, i, n);
			else
				node_shift_right_without_children(node, i, n);
			node->entry[i] = entry; 
			return i + 1;
		}	
	}
	return -1;
}

/*
 * cbt_entry_t*, void (*)(int) -> void*
 * Frees entry  and calls the passed in function on the entry's object if it is not NULL
 */
static void* bt_destroy_entry(cbt_entry_t* entry, void (* done)(void*)) 
{
	if(done != NULL)
	{
		(*done)(entry->object);
		entry->object = NULL;
	}
	void* object = entry->object;
	free(entry);
	return object;
	
	
}

/**
  * cbt_node_t*, int -> int
  * EFFECTS: gets the index of an entry in an array of entries
  * RETURNS: the index of the entry if found, -1 otherwise
  */
static int get_entry_index(cbt_node_t* node, int key)
{
	for(int i = 0; i < node->len; i++)
	{
		if(node->entry[i]->key == key)
		{
			return i;
		}
	}
	return -1;
}  

/**
  * makes a copy of given entry
  */
static cbt_entry_t* cpy_entry(cbt_entry_t* entry_original)
{
	assert(entry_original != NULL);
	return bt_create_entry(entry_original->key, entry_original->object);
}


static void node_entry_set_null(cbt_node_t* node, int entry_index, int n)
{
	node->entry[entry_index] = NULL;
	if(entry_index < n-1)
	{
		node_shift_left(node, entry_index,  n);
	}
	node->len--;
}


/*****************************
 *							 *
 *	 node children helpers   *
 *							 *
 *****************************/


static void children_shift_right(cbt_node_t* nodes[], int n)
{
	for(int i = n; i > 0; i--)
	{
		nodes[i] = nodes[i-1];
	}
}



/**
  * cbt_node_t*, int -> int
  * EFFECTS: gets the index of an child in an array of children
  * RETURNS: the index of the child if found, -1 otherwise
  */
static int get_child_index(cbt_node_t* node, cbt_node_t* child)
{
	for(int i = 0, n1 = node->len+1; i < n1; i++)
	{
		if(node->children[i] == child)
		{
			return i;
		}
	}
	return -1;
}  


static void children_shift_left_i(cbt_node_t* node, int i, int n)
{
	for(int j = i; j < n; j++)
	{
		node->children[j] = node->children[j+1];
	}
	node->children[n] = NULL;
}

static void children_shift_right_i(cbt_node_t* nodes[], int j, int n)
{ 
	for(int i = n; i > j; i--)
	{
		nodes[i] = nodes[i-1];
	}
	nodes[j] = NULL;
}


/**
  * Fixes pointers gaps if any
  */
static void fix_pointers_gaps(cbt_node_t* node, int n)
{
	for(int i = 0; i < n; i++)
	{
		if(node->children[i] == NULL)
		{
			children_shift_left_i(node, i, n);
		}
	}
	
}
/**
  * cbt_node_t*, cbt_node_t* -> void
  * EFFECTS: adds a childs to the appropriate position in a list of children
  * MODIFIES: cbt_node_t* parent
  * REQUIRES: children not to be full
  */
static void insert_child(cbt_node_t* parent, cbt_node_t* child, int n)
{
	if(child == NULL)
	{
		return;
	}
	if(is_leaf(parent->children[0]))
	{
		return;
	}

	for(int i = 0, n1 = n+1; i < n1; i++)
	{
		assert(parent->children[i] != child);
		if(parent->children[i] == NULL)
		{
				parent->children[i] = child; 
				return;
		}
		assert(parent->children[i]->entry[0] != NULL);
		assert(child->entry[child->len-1] != NULL);
		if(parent->children[i]->entry[0]->key > child->entry[child->len-1]->key)
		{
			children_shift_right_i(parent->children, i, n);
			parent->children[i] = child; 
			return;
		}
	}
}

/**
  * cbt_node_t*, cbt_node_t* -> void
  * EFFECTS: adds a childs to the appropriate position in a list of children
  * MODIFIES: cbt_node_t* parent
  * REQUIRES: children not to be full
  */
static void delete_child(cbt_node_t* parent, cbt_node_t* child, int n)
{
	if(child == NULL)
	{
		return;
	}
	for(int i = 0, n1 = n+1; i < n1; i++)
	{
		if(parent->children[i] == NULL)
		{
				return;
		}
		if(parent->children[i] == child)
		{
			parent->children[i] = NULL;
			children_shift_left_i(parent, i, n);
			return;
		}
	}
}
/**
  * cbt_node_t**, int -> cbt_node_t*
  * EFFECTS: shifts all the the children in the cbt_node_t array on step to the left
  * MODIFIES: cbt_node_t**
  * RETURNS: the leftmost element
  */
static cbt_node_t* children_shift_left(cbt_node_t* nodes[], int n)
{
	cbt_node_t* ret = nodes[0];
	for(int i = 0; i < n; i++)
	{
		nodes[i] = nodes[i+1]; 
	}
	nodes[n] = NULL;
	return ret;
}

/**
  * cbt_node_t** -> cbt_node_t*
  * EFFECTS: removes the last from the children array
  * MODIFIES: cbt_node_t**
  * RETURNS: the last child
  */
 static cbt_node_t* remove_last_child(cbt_node_t* nodes[], int len)
 {
 
 	cbt_node_t* last = nodes[len];
 	nodes[len] = NULL;
 	return last;
 
 }
 
 
