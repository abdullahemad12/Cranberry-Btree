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
#include <btree.h>

/*protoypes*/
static cbt_node_t* bt_create_node(int n);
static cbt_entry_t* bt_create_entry(int key, void* object);
static void bt_destroy_node(cbt_node_t* node, int n, void (* done)(void*));
static void* bt_destroy_entry(cbt_entry_t* entry, void (* done)(void*));
static int ceil_fn(double n);
static bool is_full_node(cbt_node_t* node, int n);
static void print_node(cbt_node_t* node, int n);
static bool print_level(cbt_node_t* root, int n, int level, int currentLevel);
static int node_insert_entry(cbt_node_t* node, cbt_entry_t* entry, bool shift,  int n);
static cbt_entry_t* bt_split_child(cbt_node_t* node, cbt_node_t** splitted_node, int n);
static int get_next_node_index(cbt_node_t* node, int key, int n);
static void node_shift_right(cbt_node_t* node, int i,  int n);
static void node_shift_left(cbt_node_t* node, int i,  int n);
static void node_shift_left_without_children(cbt_node_t* node, int i,  int n);
static cbt_node_t* split_full_root(cbt_node_t* old_root, int n);
static bool is_leaf(cbt_node_t* node);
static void* bt_node_search_helper(cbt_entry_t* entries[], int key, int min, int max);
static void* bt_search_helper(cbt_node_t* node, int key, int n);
static void bt_insert_helper(cranbtree_t* bt ,cbt_node_t* root, cbt_entry_t* entry);
static void destroy_bt_helper(cbt_node_t* root, int n, void (* done)(void*));
static bool is_root(cranbtree_t* bt, cbt_node_t* node);
static void* bt_delete_entry_helper(cbt_node_t* node, int key, int n);
static int get_last_entry_index(cbt_node_t* node, int n);
static int min(int x, int y);
static cbt_node_t* merge_leaf_nodes(cbt_node_t* node1, cbt_node_t* node2, int n);
static void node_entry_set_null(cbt_node_t* node, int entry_index, int n);
static void children_shift_right(cbt_node_t* nodes[], int n);
static cbt_node_t* children_shift_left(cbt_node_t* nodes[], int n);
static cbt_entry_t* cpy_entry(cbt_entry_t* entry_original);
static int get_entry_index(cbt_node_t* node, int key);
static void* bt_delete_int_case(cranbtree_t* bt, cbt_node_t* node, int key);
static void fix_pointers_gaps(cbt_node_t* node, int n);
static void node_shift_right_without_children(cbt_node_t* node, int i,  int n);
static void* bt_delete_helper(cranbtree_t* bt, cbt_node_t* parent,  cbt_node_t* node, int key);
static cbt_node_t* get_right_sibling(cbt_node_t* parent, cbt_node_t* node);
static cbt_node_t* get_left_sibling(cbt_node_t* parent, cbt_node_t* node);
static cbt_node_t* merge_nodes(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right);
static void entry_rotate_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n);
static void entry_rotate_counter_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n);
static int get_child_index(cbt_node_t* node, cbt_node_t* child);
static void insert_child(cbt_node_t* parent, cbt_node_t* child, int n);
static void delete_child(cbt_node_t* parent, cbt_node_t* child, int n);
static void balance_node(cranbtree_t* bt, cbt_node_t** parent_ptr, int key);
static void entry_move_up_clockwise(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* right, int key , int n);
static void entry_move_up_counter_clockwise(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* right, int key , int n);
static cbt_node_t* remove_last_child(cbt_node_t* nodes[], int len);
cbt_entry_t* bt_delete_minimum(cranbtree_t* bt, cbt_node_t* node);
cbt_entry_t* bt_delete_maximum(cranbtree_t* bt, cbt_node_t* node);
int cbt_calculate_min_key(ccbt_node_t* root);
int cbt_calculate_max_key(ccbt_node_t* root);




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
	return cbt->max_key;
}


/**
  * cranbtree_t* -> int
  * EFFECTS: gets the minimum key in the Tree
  * RETURNS: the minimum key 
  */
int cbt_get_min_key(cranbtree_t* cbt)
{
	return cbt->min_key;
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
 void bt_destroy(cranbtree_t* bt, void (* destroy_object)(void*))
 {
 	destroy_bt_helper(bt->root, bt->n, destroy_object);
 	free(bt);
 }						

/*******************************************
 *								 		   *
 *		 Static private Functions          *
 *								 		   *
 *******************************************/

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
	node->entry = malloc(entry_size);
	if(node->entry == NULL)
	{
		free(node);
		return NULL;
	}
	memset(node->entry, 0, sizeof(cbt_entry_t*) * n);
	
	size_t children_size =  sizeof(cbt_node_t*) * (n+1);
	node->children = malloc(children_size);
	if(node->children == NULL)
	{
		free(node->entry);
		free(node);
		return NULL;
	}
	memset(node->children, 0, children_size);
	node->len = 0;
	return node;
}


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
  * cbt_node_t*, cbt_entry_t* -> cbt_node_t*
  * EFFECT: Given the root of the tree, inserts the given entery in the appropriate location in the leaf
  * MODIFIES: Tree
  * RETURNS: void
  *
  * cranbtree_t* bt: btree struct pointer
  * cbt_node_t* root: the root of the tree
  * cbt_entry_t* entry: entry to be inserted in the tree
  * int n: the size of the entries array
  * int level: the current tree level. initially zero
  */
static void bt_insert_helper(cranbtree_t* bt ,cbt_node_t* root, cbt_entry_t* entry)
{	

	assert(bt != NULL);
	/*the first insertion in the tree*/
	if(root == NULL)
	{
		root = bt_create_node(bt->n);
		assert(root != NULL);
		node_insert_entry(root, entry, true, bt->n);
		bt->root = root;
		bt->max_key = entry->key;
		bt->min_key = entry->key;
		return;
	}
	/* we are at the root level*/
	if(is_root(bt, root))
	{
		root = split_full_root(root , bt->n);
		/*splits the root if it needs splitting*/
		if(bt->root != root)
		{
			/*restarts execution with the new root*/
			bt->root = root;
			bt_insert_helper(bt, bt->root, entry);
			return;
		}	
	}
	
	int child_index = get_next_node_index(root, entry->key, bt->n);
	/*this is a leaf (can be a root and a leaf at the same time)*/
	if(is_leaf(root->children[child_index]))
	{
		node_insert_entry(root, entry, true, bt->n);
	}
	else
	{
		cbt_node_t* splitted_node = NULL;
		cbt_entry_t* median_entry = bt_split_child(root->children[child_index], &splitted_node, bt->n);

		/* a split happened*/
		if(median_entry != NULL)
		{
			int splitted_node_ptr_index = node_insert_entry(root, median_entry, true, bt->n);
			root->children[splitted_node_ptr_index] = splitted_node;
			child_index = get_next_node_index(root, entry->key, bt->n); /*recalculates the path*/
		}
		
		bt_insert_helper(bt , root->children[child_index], entry);
	}
}

/*
 * cbt_node_t*, int -> cbt_node_t*
 *
 * EFFECTS: Creates a new root for the tree if the node needs to be splitted
 * MODIFIES: cbt_node_t** root
 * RETURNS: the new node that was created in case of a split, otherwise returns the old node
 *
 */
 static cbt_node_t* split_full_root(cbt_node_t* old_root, int n)
 {
		cbt_node_t* splitted_node = NULL;
		cbt_entry_t* root_entry = bt_split_child(old_root, &splitted_node, n);
		/*creates new root and restarts execution*/
		if(root_entry != NULL)
		{
			cbt_node_t* new_root = bt_create_node(n);
			assert(new_root != NULL);
			node_insert_entry(new_root, root_entry, true, n);
			new_root->children[0] = old_root;
			new_root->children[1] = splitted_node;
			return new_root;
		}
		else
		{
			return old_root;
		}
 	
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
 * cbt_node_t*, cbt_node_t**, int -> cbt_entry_t*
 * splits a given node into two and returns the entry that should be inserted in the parent node
 * return NULL if no split is needed. i.e: the node is not full
 * REQUIRES: node to be sorted
 */
static cbt_entry_t* bt_split_child(cbt_node_t* node, cbt_node_t** splitted_node, int n)
{
	/*the node does not need to be splitted*/
	if(!is_full_node(node, n))
	{
		return NULL;
	}

	
	/*splits the node*/
	cbt_node_t* new_node = bt_create_node(n);
	int borrow_n = ceil_fn(((double) n) / 2.0);
	int new_node_itr = 0;
	
	/*edge case for the pointers*/
	new_node->children[0] = node->children[borrow_n]; 
	node->children[borrow_n] = NULL;
	for(int i = borrow_n; i < n; i++)
	{
		/*copies over the entries and pointers from the old node*/
		new_node->entry[new_node_itr++] = node->entry[i];
		new_node->children[new_node_itr] = node->children[i+1];
		node->entry[i] = NULL;
		node->children[i+1] = NULL;
	}
	/*updates the length*/
	new_node->len = n / 2;
	node->len = n / 2;
	
	cbt_entry_t* entry = node->entry[borrow_n - 1];
	node->entry[borrow_n - 1] = NULL;
	*(splitted_node) = new_node;
	return entry;

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


static void* bt_search_helper(cbt_node_t* node, int key, int n)
{
	if(node == NULL)
	{
		return NULL;
	}
	
	void* object = bt_node_search_helper(node->entry, key, 0, node->len);
	if(object == NULL)
	{
		int i = get_next_node_index(node,  key,  n);
		return bt_search_helper(node->children[i], key, n);
	}
	
	return object;
}

/*
 * cbt_entry_t*[], int key -> void* 
 * EFFECTS: looks for an entry in the entry list with the given search key
 * RETURNS: pointer to the object or NULL if it was not found
 *
 */
static void* bt_node_search_helper(cbt_entry_t* entries[], int key, int min, int max)
{
	if(min == max)
	{
		return NULL;
	}
	int avg = (max + min) / 2;
	if(key == entries[avg]->key)
	{
		return entries[avg]->object;
	}
	else if(key > entries[avg]->key)
	{
		return bt_node_search_helper(entries, key, avg + 1, max);
	}
	else
	{
		return  bt_node_search_helper(entries, key, min, avg);
	}	

}

/**
  * cranbtree_t* , cbt_node_t* , int -> void*
  * EFFECTS: Deletes an entry in respecte to the given search key
  * 		 does nothing if no such entry exist
  * MODIFIES: cranbtree_t* bt
  * RETURNS: the object associated with the entry, or NULL if no such entry was found
  */
static void* bt_delete_helper(cranbtree_t* bt, cbt_node_t* parent,  cbt_node_t* node, int key)
{
	if(node == NULL)
	{
		return NULL;
	}

	cbt_node_t* old_root = bt->root;
	balance_node(bt, &node, key);
	
	/*restarts execution if the root was altered*/
	if(bt->root != old_root)
	{
		return bt_delete_helper(bt, NULL,  bt->root, key);
	}
	
	/*Not found yet*/
	int next_node_index = get_next_node_index(node, key, bt->n);
	void* object = bt_node_search_helper(node->entry, key, 0, node->len);	
	assert(next_node_index != -1);
	if(object == NULL)
	{
		return bt_delete_helper(bt, node, node->children[next_node_index], key);
	}	
	
	/*is it a leaf*/	
	if(is_leaf(node->children[0]))
	{	
		 void* object = bt_delete_entry_helper(node, key, bt->n);
		 if(is_root(bt, node) && node->len == 0)
		 {
		 	bt_destroy_node(node, bt->n, NULL);
			bt->root = NULL;
		 }
		return object;
	}
	else
	{
		/*delete from intermediate node*/
		return bt_delete_int_case(bt, node, key);
	}

}



/**
  * cranbtree_t*, cbt_node_t**, int -> void*
  * REQUIRES: node to be an intermediate node
  * EFFECTS: Removes the given node and rebalances the tree
  * MODIFIES: cranbtree_t* bt
  */
static void* bt_delete_int_case(cranbtree_t* bt, cbt_node_t* node, int key)
{

	int index = get_entry_index(node,  key);
	assert(index >= 0);
	void* object  = node->entry[index]->object;
	
	node->entry[index]->object = NULL;
	cbt_node_t* left = node->children[index];
	cbt_node_t* right = node->children[index+1];
	assert(left != NULL && right != NULL);/*every entry must have a right and left children*/

	
	
	if(right != NULL)
	{
		entry_move_up_counter_clockwise(bt, node, right, key , bt->n);
		return object;
	}
	else if(left != NULL)
	{
		entry_move_up_clockwise(bt, node, left, key , bt->n);
		return object;
	}
	
	return NULL;	
}



/**
  * cranbtree_t*, cbt_node_t**, int -> void*
  * REQUIRES: node to be an leaf node
  * EFFECTS: Removes the given node and rebalances the tree
  * MODIFIES: cranbtree_t* bt, cbt_node_t** parent, cbt_node_t* node
  */
static void balance_node(cranbtree_t* bt, cbt_node_t** parent_ptr, int key)
{
	
	cbt_node_t* parent = *(parent_ptr);
	/*nothing to split*/
	if(is_leaf(parent->children[0]))
	{
		return;
	}
	
	int index = get_next_node_index(parent, key, bt->n);
	cbt_node_t* node = parent->children[index];
	
	int min_n = is_leaf(node->children[0]) ? bt->n / 2 : ceil_fn(((double) bt->n) / 2.0) - 1;
	if(node->len > min_n)
	{
		return;
	}
	cbt_node_t* right_sibling = get_right_sibling(parent, node);
	cbt_node_t* left_sibling = get_left_sibling(parent, node);  
	
	/*case 2a)*/
	if(right_sibling != NULL && right_sibling->len > min_n)
	{
		entry_rotate_counter_clockwise(parent, node, right_sibling, bt->n);
	}
	else if (left_sibling != NULL && left_sibling->len > min_n)
	{
		entry_rotate_clockwise(parent, left_sibling, node, bt->n);
	}
	/*case 2b)*/
	else if(right_sibling != NULL)
	{
		
		*(parent_ptr) = merge_nodes(bt, parent, node, right_sibling);
	}
	else if(left_sibling != NULL)
	{
		*(parent_ptr) = merge_nodes(bt, parent, left_sibling, node);
	}


}

static void* bt_delete_entry_helper(cbt_node_t* node, int key, int n)
{
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] == NULL)
		{
			return NULL;
		}
		if(node->entry[i]->key == key)
		{
			void* object = bt_destroy_entry(node->entry[i], NULL);
			node->entry[i] = NULL;
			node_shift_left_without_children(node, i,  n);
			fix_pointers_gaps(node, n);
			node->len--;
			return object;
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
 
/**
  * cbt_node_t* , cbt_node_t*, int n -> cbt_node_t*
  * REQUIRES: entries 0 < len <= min_n and len1 + len2 <= n
  * EFFECTS: merges two leaf nodes entries into one node
  */ 
static cbt_node_t* merge_leaf_nodes(cbt_node_t* node1, cbt_node_t* node2, int n)
{
	if(node1->entry[0]->key > node2->entry[0]->key)
	{
		cbt_node_t* tmp = node1; 
		node1 = node2; 
		node2 = tmp;
	}
	assert(node1->len + node2->len <= n);
	
	/*edge case*/
	node1->children[node1->len] = node2->children[0];
	for(int i = 0;  node2->entry[i] != NULL; i++)
	{
		int ind = node_insert_entry(node1, node2->entry[i], true, n);
		node1->children[ind] = node2->children[i+1];
		node2->entry[i] = NULL;
		node2->children[i+1] = NULL;
		node2->len--;
	}
	

	bt_destroy_node(node2, n, NULL);
	
	return node1;

}



/*******************************
 *						       *
 *        node helpers         *
 *							   *
 *******************************/

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


/**
  * cranbtree_t*, cbt_node_t*, cbt_node_t*, cbt_node_t* -> cbt_node_t*;
  * EFFECTS: Merges the two children and their parents respective entry into one node
  * REQUIRES: 1) the children contains minimum number of keys 
  *			  2) the left and right children be passed in the correct order
  * MODIFIES: bt_node_left child
  * RETURNS: pointer to the new node
  */
static cbt_node_t* merge_nodes(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right)
{
	(void)get_last_entry_index;
	(void)node_entry_set_null;
	(void)children_shift_right;

	/*no need to merge*/
	if(left->len + right->len + 1 > bt->n)
	{
		return NULL;
	}	
	
	
	int index = get_child_index(parent, left);
	assert(index != -1);
	
	cbt_entry_t* entry_cpy = cpy_entry(parent->entry[index]);
	/*remove it from the parent first*/
	bt_delete_entry_helper(parent, entry_cpy->key, bt->n);
	node_insert_entry(left, entry_cpy, false, bt->n);
	
	/*now merge the nodes*/
	delete_child(parent, left, bt->n);
	delete_child(parent, right, bt->n);
	cbt_node_t* new_node = merge_leaf_nodes(left, right, bt->n);

	/*was the root*/
	if(parent->len == 0)
	{
		assert(bt->root == parent);
		bt_destroy_node(parent, bt->n, NULL);
		bt->root = new_node;
		return new_node;
	}
	else
	{
		insert_child(parent, new_node, bt->n);
	}
	
	return parent;
}

/**
  * cbt_node_t*, cbt_node_t*, cbt_node_t*, int -> void
  * REQUIRES: the right node to have more than the minimum number of keys
  * EFFECTS: borrows an entry from the right node to increase the entries of the left node
  * MODIFIES: parent, right_node, left_node
  */
static void entry_rotate_counter_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n)
{
	int index = get_child_index(parent, left);
	
	cbt_entry_t* entry_cpy = cpy_entry(parent->entry[index]);
	bt_delete_entry_helper(parent, entry_cpy->key, n);
	node_insert_entry(left, entry_cpy, false, n);

	
	entry_cpy = cpy_entry(right->entry[0]);
	bt_delete_entry_helper(right, entry_cpy->key, n);
	node_insert_entry(parent, entry_cpy, false, n);
	cbt_node_t* shifted = children_shift_left(right->children, n);
	left->children[left->len] = shifted;
}


/**
  * cbt_node_t*, cbt_node_t*, cbt_node_t* , int-> void
  * REQUIRES: the left node to have more than the minimum number of keys
  * EFFECTS: borrows an entry from the left node to increase the entries of the right node
  * MODIFIES: parent, right_node, left_node
  */
static void entry_rotate_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n)
{
	int index = get_child_index(parent, left);
	cbt_entry_t* entry_cpy = cpy_entry(parent->entry[index]);
	bt_delete_entry_helper(parent, entry_cpy->key, n);
	node_insert_entry(right, entry_cpy, false, n);
	
	cbt_node_t* last_child = remove_last_child(left->children, left->len);
	children_shift_right(right->children, n);
	right->children[0] = last_child;
	
	index =  get_last_entry_index(left, n);
	entry_cpy = cpy_entry(left->entry[index]);
	bt_delete_entry_helper(left, entry_cpy->key, n);
	node_insert_entry(parent, entry_cpy, false, n);
}

/**
  * cranbtree_t*, cbt_node_t*, cbt_node_t*, cbt_node_t*, int, int -> void
  * EFFECTS: moves a copy of the last entry of the left node and inserts it into parent 
  * 		 after removing the entry with the given key from the parent
  * MODIFIES: cbt_node_t* parent
  * REQUIRES: the left node to have more than the minimum number of keys
  * RETURNS: the key of the last inserted entry
  */
static void entry_move_up_clockwise(cranbtree_t* bt,cbt_node_t* parent, cbt_node_t* left, int key , int n)
{

	cbt_entry_t* entry_cpy = bt_delete_maximum(bt, left);
	bt_delete_entry_helper(parent, key, n);
	node_insert_entry(parent, entry_cpy, false, n);

}


/**
  * cranbtree_t*, cbt_node_t*, cbt_node_t*, cbt_node_t*, int, int -> void
  * EFFECTS: moves a copy of the first entry of the right node and inserts it into parent 
  * 		 after removing the entry with the given key from the parent
  * MODIFIES: cbt_node_t* parent
  * REQUIRES: the right node to have more than the minimum number of keys
  * RETURNS: the key of the last inserted entry
  */
static void entry_move_up_counter_clockwise(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* right, int key , int n)
{
	cbt_entry_t* entry_cpy = bt_delete_minimum(bt, right);
	bt_delete_entry_helper(parent, key, n);
	node_insert_entry(parent, entry_cpy, false, n);
}


/**
  * cranbtree_t*, cbt_node_t* -> cbt_entry_t* 
  * EFFECTS: deletes the minimum key in a Tree starting from the given node
  * MODIFIES: bt
  * RETURNS: entry with the minimum key
  */
cbt_entry_t* bt_delete_minimum(cranbtree_t* bt, cbt_node_t* node)
{
	/*the min will always be at the leafs*/
	if(is_leaf(node->children[0]))
	{
		cbt_entry_t* entry_cpy = cpy_entry(node->entry[0]);
		bt_delete_entry_helper(node, node->entry[0]->key, bt->n);
		return entry_cpy;
	}
	/*since we don't know yet what the min key of the
	tree is we will just pass in a key that is
	smaller than the min key of this node*/
	int key = node->entry[0]->key - 3;
 	balance_node(bt, &node, key);
 	int next_node_index = get_next_node_index(node, key, bt->n);
	return bt_delete_minimum(bt, node->children[next_node_index]);

}

/**
  * cranbtree_t*, cbt_node_t* -> cbt_entry_t* 
  * EFFECTS: deletes the maximum key in a Tree starting from the given node
  * MODIFIES: bt
  * RETURNS: entry with the maximum key
  */
cbt_entry_t* bt_delete_maximum(cranbtree_t* bt, cbt_node_t* node)
{
	/*the max will always be at the leafs*/
	int index = get_last_entry_index(node, bt->n);
	if(is_leaf(node->children[0]))
	{
		cbt_entry_t* entry_cpy = cpy_entry(node->entry[index]);
		bt_delete_entry_helper(node, node->entry[index]->key, bt->n);
		return entry_cpy;
	}
	int key = node->entry[index]->key + 3;
 	balance_node(bt, &node, key);
 	int next_node_index = get_next_node_index(node, key, bt->n);
	return bt_delete_maximum(bt, node->children[next_node_index]);

}



/**
  * ccbt_node_t* -> int
  * EFFECTS: calculates the minimum key in the tree
  * REQUIRES: The tree should not be empty
  * RETURNS: the minimum key in the tree
  */
int cbt_calculate_min_key(cbt_node_t* root)
{
	if(root == NULL)
	{
		return 0;
	}

	if(is_leaf(root->children[0]))
	{
		return root->entry[0]->key;
	}

	return bt_calculate_min_key(root->children[0]);	
}


/**
  * ccbt_node_t* -> int
  * EFFECTS: calculates the maximum key in the tree
  * REQUIRES: The tree should not be empty
  * RETURNS: the maximum key in the tree
  */
int cbt_calculate_max_key(cbt_node_t* root)
{

	if(root == NULL)
	{
		return 0;
	}

	if(is_leaf(root->children[0]))
	{
		return root->entry[root->len-1]->key;
	}

	return bt_calculate_max_key(root->children[root->len]);	
}

  
/*******************************
 *						       *
 *        helpers              *
 *							   *
 *******************************/
 
static int ceil_fn(double n)
{
	int n_tmp = (int) n;
	return n_tmp < n ? n_tmp+1 : n_tmp;
}

static int calculate_depth(cbt_node_t* node)
{
	if(node == NULL)
	{
		return 0;
	}
	return 1 + calculate_depth(node->children[0]);
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

static bool print_level(cbt_node_t* root, int n, int level, int currentLevel)
{


	min(1,2);
	if(level == currentLevel)
	{
		if(root == NULL)
		{
			return false;
		}
		print_node(root, n);
		return true;
	}
	if(root == NULL)
	{
		return false;
	}
	bool ret = true;
	for(int i = 0; i < n+1; i++)
	{
		if(root->children[i] != NULL)
		{
			ret = print_level(root->children[i], n, level, currentLevel+1);
			if(ret == false)
			{
				return false;
			}
		}
		
	}
	return ret;
	
}

static void print_node(cbt_node_t* node, int n)
{

	printf(" || ");
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] != NULL)
		{
			printf("%d ,", node->entry[i]->key);
		}
		else
		{
			printf("* ,");
		}
	}
	printf(" || ");
}

/*
 * Improves code readability
 */
static bool is_leaf(cbt_node_t* node)
{
	return node == NULL;
}

/*
 * Improves code readability
 */
static bool is_root(cranbtree_t* bt, cbt_node_t* node)
{
	return bt->root == node;
}

static int min(int x, int y)
{
	return x < y ? x : y;
}


static void children_shift_right(cbt_node_t* nodes[], int n)
{
	for(int i = n; i > 0; i--)
	{
		nodes[i] = nodes[i-1];
	}
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
/**
  * makes a copy of given entry
  */
static cbt_entry_t* cpy_entry(cbt_entry_t* entry_original)
{
	assert(entry_original != NULL);
	return bt_create_entry(entry_original->key, entry_original->object);
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
 
 
