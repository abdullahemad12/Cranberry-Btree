#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "btree.h"

/*protoypes*/
static bt_node_t* bt_create_node(int n);
static bt_entry_t* bt_create_entry(int key, void* object);
static void bt_destroy_node(bt_node_t* node, int n, void (* done)(void*));
static void* bt_destroy_entry(bt_entry_t* entry, void (* done)(void*));
static int ceil_fn(double n);
static bool is_full_node(bt_node_t* node, int n);
static void print_node(bt_node_t* node, int n);
static bool print_level(bt_node_t* root, int n, int level, int currentLevel);
static int node_insert_entry(bt_node_t* node, bt_entry_t* entry, int n);
static bt_entry_t* bt_split_child(bt_node_t* node, bt_node_t** splitted_node, int n);
static int get_next_node_index(bt_node_t* node, int key, int n);
static void node_shift_right(bt_node_t* node, int i,  int n);
static void node_shift_left(bt_node_t* node, int i,  int n);
static bt_node_t* split_full_root(bt_node_t* old_root, int n);
static bool is_leaf(bt_node_t* node);
static void* bt_node_search_helper(bt_entry_t* entries[], int key, int min, int max);
static void* bt_search_helper(bt_node_t* node, int key, int n);
static void bt_insert_helper(btree_t* bt ,bt_node_t* root, bt_entry_t* entry);
static void destroy_bt_helper(bt_node_t* root, int n, void (* done)(void*));
static bool is_root(btree_t* bt, bt_node_t* node);
static void* bt_delete_int_case(btree_t* bt, bt_node_t* node, int key);
static void bt_merge_children(btree_t* bt, bt_node_t** parent_ptr, bt_node_t* node, int node_ind);
static void* bt_delete_entry_helper(bt_node_t* node, int key, int n);
static int get_last_entry_index(bt_node_t* node, int n);
static int min(int x, int y);
static bt_node_t* merge_leaf_nodes(bt_node_t* node1, bt_node_t* node2, int n);
static void node_entry_set_null(bt_node_t* node, int entry_index, int n);
static void children_shift_left(bt_node_t* nodes[], int n);
static void children_shift_right(bt_node_t* nodes[], int n);
static bt_entry_t* cpy_entry(bt_entry_t* entry_original);
static int get_entry_index(bt_node_t* node, int key);
static void* bt_delete_int_case(btree_t* bt, bt_node_t** node_ptr, int key);


/*
 * int -> bptree_t* 
 * Given the maximum number of entries in a node
 * returns a pointer to an empty B-tree
 */
btree_t* bt_create(int n)
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
	
	btree_t* bt = malloc(sizeof(btree_t));
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
  * btree_t*, int, void* -> void
  * EFFECTS: inserts an object in the btree in respect with the search key
  * MODIFIES: btree_t* bt
  * RETURNS: void
  *
  * btree_t* bt: Tree structs that will hold the object
  * int key: search key (choosen by the user)
  * void* object: pointer to the object to be inserted
  */
void bt_insert(btree_t* bt, int key, void* object)
{
	assert(bt != NULL);
	assert(object != NULL);
	
	bt_entry_t* entry = bt_create_entry(key, object);
	bt_insert_helper(bt, bt->root, entry);
}


/**
  * btree_t*, int -> void*
  * EFFECTS: search for an object given the search key
  * RETURN: pointer to the object or NULL if the object was 
  *		    not found
  */
void* bt_search(btree_t* bt, int key)
{
	assert(bt != NULL);
	return bt_search_helper(bt->root, key, bt->n);
} 


/**
  * btree_t*, (void) destroy_object(void*) -> void
  * MODIFIES: btree_t* bt
  * EFFECTS: Frees all the memory associated with the B tree
  * 
  * (void) destroy_object(void*): a pointer to a function that will be invoked on every object 
  * 				  i.e: object_destroy(object). In case the pointer is NULL, nothing is done 
  *						on the object and it becomes the user's responsibility to free it
  */
 void bt_destroy(btree_t* bt, void (* destroy_object)(void*))
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
  * int n -> bt_node_t*
  * Creates a node according to the size n
  *
  */
static bt_node_t* bt_create_node(int n)
{
	if(n < 3)
	{
		return NULL;
	}
	
	bt_node_t* node = malloc(sizeof(bt_node_t));
	if(node == NULL)
	{
		return NULL;
	}
	size_t entry_size = sizeof(bt_entry_t*) * n;
	node->entry = malloc(entry_size);
	if(node->entry == NULL)
	{
		free(node);
		return NULL;
	}
	memset(node->entry, 0, sizeof(bt_entry_t*) * n);
	
	size_t children_size =  sizeof(bt_node_t*) * (n+1);
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
static bt_entry_t* bt_create_entry(int key, void* object)
{
	assert(object != NULL);
	assert(key >= 0);
	bt_entry_t* entry = calloc(1, sizeof(bt_entry_t));
	if(entry == NULL)
	{
		return NULL;
	}
	entry->key = key;
	entry->object = object;
	return entry;
}

static void destroy_bt_helper(bt_node_t* root, int n, void (* done)(void*))
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
 * bt_node_t*, void (* done)(int) -> void
 * destroys all the data associated with a given node
 */
 
static void bt_destroy_node(bt_node_t* node, int n, void (* done)(void*))
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
 * bt_entry_t*, void (*)(int) -> void*
 * Frees entry  and calls the passed in function on the entry's object if it is not NULL
 */
static void* bt_destroy_entry(bt_entry_t* entry, void (* done)(void*)) 
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
  * bt_node_t*, bt_entry_t* -> bt_node_t*
  * EFFECT: Given the root of the tree, inserts the given entery in the appropriate location in the leaf
  * MODIFIES: Tree
  * RETURNS: void
  *
  * btree_t* bt: btree struct pointer
  * bt_node_t* root: the root of the tree
  * bt_entry_t* entry: entry to be inserted in the tree
  * int n: the size of the entries array
  * int level: the current tree level. initially zero
  */
static void bt_insert_helper(btree_t* bt ,bt_node_t* root, bt_entry_t* entry)
{	

	assert(bt != NULL);
	/*the first insertion in the tree*/
	if(root == NULL)
	{
		root = bt_create_node(bt->n);
		assert(root != NULL);
		node_insert_entry(root, entry, bt->n);
		bt->root = root;
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
		node_insert_entry(root, entry, bt->n);
	}
	else
	{
		bt_node_t* splitted_node = NULL;
		bt_entry_t* median_entry = bt_split_child(root->children[child_index], &splitted_node, bt->n);

		/* a split happened*/
		if(median_entry != NULL)
		{
			int splitted_node_ptr_index = node_insert_entry(root, median_entry, bt->n);
			root->children[splitted_node_ptr_index] = splitted_node;
			child_index = get_next_node_index(root, entry->key, bt->n); /*recalculates the path*/
		}
		
		bt_insert_helper(bt , root->children[child_index], entry);
	}
}

/*
 * bt_node_t*, int -> bt_node_t*
 *
 * EFFECTS: Creates a new root for the tree if the node needs to be splitted
 * MODIFIES: bt_node_t** root
 * RETURNS: the new node that was created in case of a split, otherwise returns the old node
 *
 */
 static bt_node_t* split_full_root(bt_node_t* old_root, int n)
 {
		bt_node_t* splitted_node = NULL;
		bt_entry_t* root_entry = bt_split_child(old_root, &splitted_node, n);
		/*creates new root and restarts execution*/
		if(root_entry != NULL)
		{
			bt_node_t* new_root = bt_create_node(n);
			assert(new_root != NULL);
			node_insert_entry(new_root, root_entry, n);
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
 * bt_node_t*, bt_entry_t*, int -> int
 * MODIFIES: bt_node_t* node
 * EFFECTS: inserts an entry in the node preserving the the order of the entries. i.e 
 * 			keeping the node sorted after the insertions
 * REQUIRES: the node should not be full
 * RETURNS: returns the index of the child corresponding to the new inserted entry on success, or
 * 			-1 on Failure
 */
static int node_insert_entry(bt_node_t* node, bt_entry_t* entry, int n)
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
			node_shift_right(node, i, n);
			node->entry[i] = entry; 
			return i + 1;
		}	
	}
	return -1;
}

/*
 * bt_node_t*, int n
 * MODIFIES: bt_node_t*
 * EFFECTS: shifts all the entries and children in the array to the right 
 *			starting from position i and i+1 respectively
 * REQUIRES: the node should not be full, otherwise the rightmost element will be orphaned "i < n - 1"
 * RETURNS: void
 *
 */
static void node_shift_right(bt_node_t* node, int i,  int n)
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
 * bt_node_t*, int n
 * MODIFIES: bt_node_t*
 * EFFECTS: shifts all the entries and children in the array to the left 
 *			starting from position i and i-1 respectively
 * REQUIRES:  position i be NULL , otherwise the the i'th element will be orphaned "0 =< i < n-1"
 * RETURNS: void
 *
 */
static void node_shift_left(bt_node_t* node, int i,  int n)
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
 * bt_node_t*, int, int 
 * EFFECTS: calculates the index of the entry in the node entry array 
 * REQUIRES: the entries in the array to be packed together to the left and no of entries > 0
 * RETURNS: an index or -1 on error
 */
static int get_next_node_index(bt_node_t* node, int key, int n)
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
 * bt_node_t*, bt_node_t**, int -> bt_entry_t*
 * splits a given node into two and returns the entry that should be inserted in the parent node
 * return NULL if no split is needed. i.e: the node is not full
 * REQUIRES: node to be sorted
 */
static bt_entry_t* bt_split_child(bt_node_t* node, bt_node_t** splitted_node, int n)
{
	/*the node does not need to be splitted*/
	if(!is_full_node(node, n))
	{
		return NULL;
	}

	
	/*splits the node*/
	bt_node_t* new_node = bt_create_node(n);
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
	
	bt_entry_t* entry = node->entry[borrow_n - 1];
	node->entry[borrow_n - 1] = NULL;
	*(splitted_node) = new_node;
	return entry;

}


/*
 * bt_node_t*, int -> bool
 * return true if the node has no room for more entries
 */
static bool is_full_node(bt_node_t* node, int n)
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


static void* bt_search_helper(bt_node_t* node, int key, int n)
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
 * bt_entry_t*[], int key -> void* 
 * EFFECTS: looks for an entry in the entry list with the given search key
 * RETURNS: pointer to the object or NULL if it was not found
 *
 */
static void* bt_node_search_helper(bt_entry_t* entries[], int key, int min, int max)
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
  * btree_t* , bt_node_t* , int -> void*
  * EFFECTS: Deletes an entry in respecte to the given search key
  * 		 does nothing if no such entry exist
  * MODIFIES: btree_t* bt
  * RETURNS: the object associated with the entry, or NULL if no such entry was found
  */
static void* bt_delete_helper(btree_t* bt, bt_node_t* node, int key)
{
	if(node == NULL)
	{
		return NULL;
	}

	int next_node_index = get_next_node_index(node, key, bt->n);
	/*Merge if needed and traverse deeper*/
	bt_merge_children(bt, &node, node->children[next_node_index], next_node_index);
	next_node_index = get_next_node_index(node, key, bt->n); //recalculate the index
	
	if(is_leaf(node->children[0]))
	{
		void* object =  bt_delete_entry_helper(node, key, bt->n);
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
		void* object = bt_node_search_helper(node->entry, key, 0, node->len);
		if(object != NULL)
			return bt_delete_int_case(bt, node, key);
	}
	return bt_delete_helper(bt, node->children[next_node_index], key);
	
}

/**
  * btree_t*, bt_node_t**, int -> void*
  * REQUIRES: node to be an intermediate node
  * EFFECTS: Removes the given node and rebalances the tree
  * MODIFIES: btree_t* bt
  */
static void* bt_delete_int_case(btree_t* bt, bt_node_t** node_ptr, int key)
{
	bt_node_t* node = *(node_ptr);
	/*case 3a)*/
	int entry_ind = get_entry_index(node, key);
	bt_node_t* child1 = node->children[entry_ind];
	bt_node_t* child2 = node->children[entry_ind+1];
	/*is_leaf(child1) ==> is_leaf(child2)*/
	int min_n = is_leaf(child1->children[0]) ? bt->n / 2 : ceil_fn(((double) bt->n) / 2.0) - 1;
	
	/*tries to borrow from the children if it can */
	if(child1->len > bt->n)
	{
		int last_ent_ind = get_last_entry_index(child1, bt->n);
		bt_entry_t* entry_cpy = cpy_entry(child1->entry[last_ent_ind]);
		/*recursively delete that entry*/
		bt_delete_helper(bt, bt->root, entry_cpy->key);
		/*remove the target entry*/
		void* object =  bt_delete_entry_helper(node, key, bt->n);
		/*insert replacement*/
		node_insert_entry(node, entry_cpy, bt->n);
		return object;
	}
	else if(child2->len > bt->n)
	{
		/*does the same*/
		bt_entry_t* entry_cpy = cpy_entry(child2->entry[0]);
		bt_delete_helper(bt, bt->root, entry_cpy->key);
		void* object =  bt_delete_entry_helper(node, key, bt->n);
		node_insert_entry(node, entry_cpy, bt->n);
		return object;
	}
	/*merge with the children*/
	else
	{
		node_insert_entry(child1, node->entry[entry_ind], bt->n);
		bt_node_t* new_node = merge_leaf_nodes(child1, child2, bt->n);
		node_entry_set_null(node, entry_ind, bt->n);
		node->children[entry_ind+1] = new_node;
		if(bt->root == node && node->len == 0)
		{
			bt_destroy_node(node, bt->n, NULL);
			bt->root = new_node;
			*(node_ptr) = bt->root;
		}
		return 	bt_delete_helper(bt, bt->root, key);
	}
}

/**
  * btree_t*, bt_node_t*, bt_node_t*, int -> void*
  * REQUIRES: node to be an leaf node
  * EFFECTS: Removes the given node and rebalances the tree
  * MODIFIES: btree_t* bt, bt_node_t** parent, bt_node_t* node
  * RETURNS: the new parent if any, otherwise the old one
  */
static void bt_merge_children(btree_t* bt, bt_node_t** parent_ptr, bt_node_t* node, int node_ind)
{
	/*nothing to split*/
	if(node == NULL)
	{
		return;
	}
	bt_node_t* parent = *parent_ptr;
	int min_n = is_leaf(node->children[0]) ? bt->n / 2 : ceil_fn(((double) bt->n) / 2.0) - 1;

	/*no need to merge*/
	if(node->len > min_n)
	{
		return;
	}
	
	/*case 2a)*/
	if(node_ind < bt->n &&  parent->children[node_ind+1] != NULL 
		&& parent->children[node_ind+1]->len > min_n)
	{
		bt_entry_t* sibling_entry = parent->children[node_ind+1]->entry[0];
		bt_node_t* sibling_child = parent->children[node_ind+1]->children[0];
		
		node_entry_set_null(parent->children[node_ind+1], 0, bt->n);
		
		node_insert_entry(node, parent->entry[node_ind], bt->n);
		int ind = get_last_entry_index(node, bt->n);
		node->children[ind+1] = sibling_child;
		children_shift_left(node->children, bt->n);
		
		parent->entry[node_ind] = sibling_entry;
		return;

	}
	else if (node_ind > 0 && parent->children[node_ind-1] != NULL
			&& parent->children[node_ind-1]->len > min_n)
	{
		int ind = get_last_entry_index(parent->children[node_ind-1], bt->n);
		bt_entry_t* sibling = parent->children[node_ind-1]->entry[ind];
		bt_node_t* sibling_child = parent->children[node_ind-1]->children[ind+1];
		
		node_entry_set_null(parent->children[node_ind-1], ind, bt->n);
		node_insert_entry(node, parent->entry[node_ind-1], bt->n);
		
		children_shift_right(node->children, bt->n);
		node->children[0] = sibling_child;
		parent->children[node_ind-1]->children[ind+1] = NULL;
		
		parent->entry[node_ind-1] = sibling;
		return;
	}
	
	
	/*case 2b)*/
	if(node_ind < bt->n &&  parent->children[node_ind+1] != NULL)
	{
	
		node_insert_entry(node, parent->entry[node_ind], bt->n);
		bt_node_t*	new_node =  merge_leaf_nodes(node, parent->children[node_ind+1], bt->n);
		node_entry_set_null(parent, node_ind, bt->n);
		parent->children[node_ind] = new_node;
		if(bt->root == parent && parent->len == 0)
		{
			bt_destroy_node(parent, bt->n, NULL);
			bt->root = new_node;
			*(parent_ptr) = bt->root;
		}
		return;
	}
	else if(node_ind > 0 &&  parent->children[node_ind-1] != NULL)
	{
		assert(node_ind > 0);
		
		node_insert_entry(parent->children[node_ind-1], parent->entry[node_ind-1], bt->n);	
		bt_node_t*	new_node =  merge_leaf_nodes(parent->children[node_ind-1], node , bt->n);
		parent->children[node_ind] = NULL;
		node_entry_set_null(parent, node_ind-1, bt->n);
		parent->children[node_ind-1] = new_node;
		if(bt->root == parent && parent->len == 0)
		{
			assert(bt->root == parent);
			bt_destroy_node(parent, bt->n, NULL);
			bt->root = new_node;
			*(parent_ptr) = bt->root;
		}
		return;
	}

}

static void* bt_delete_entry_helper(bt_node_t* node, int key, int n)
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
			node_shift_left(node, i,  n);
			node->len--;
			return object;
		}
	}
	return NULL;
}

/*
 * get_last_entry_index
 */
static int get_last_entry_index(bt_node_t* node, int n)
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
  * bt_node_t* , bt_node_t*, int n -> bt_node_t*
  * REQUIRES: entries 0 < len <= min_n and len1 + len2 <= n
  * EFFECTS: merges two leaf nodes entries into one node
  */ 
static bt_node_t* merge_leaf_nodes(bt_node_t* node1, bt_node_t* node2, int n)
{
	if(node1->entry[0]->key > node2->entry[0]->key)
	{
		bt_node_t* tmp = node1; 
		node1 = node2; 
		node2 = tmp;
	}
	assert(node1->len + node2->len <= n);
	
	/*edge case*/
	node1->children[node1->len] = node2->children[0];
	for(int i = 0;  node2->entry[i] != NULL; i++)
	{
		int ind = node_insert_entry(node1, node2->entry[i], n);
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
 *        helpers              *
 *							   *
 *******************************/
 
static int ceil_fn(double n)
{
	int n_tmp = (int) n;
	return n_tmp < n ? n_tmp+1 : n_tmp;
}

int calculate_depth(bt_node_t* node)
{
	if(node == NULL)
	{
		return 0;
	}
	return 1 + calculate_depth(node->children[0]);
}
void printTree(btree_t* bt)
{
	printf("\n\n");
	for(int i = 0, n = calculate_depth(bt->root); i < n; i++)
	{
			for(int j = 0; j < n * (n - i); j++)
			{
				printf("\t\t");
			}
			print_level(bt->root, bt->n, i, 0);
			printf("\n\n");
	}


}

static bool print_level(bt_node_t* root, int n, int level, int currentLevel)
{



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

static void print_node(bt_node_t* node, int n)
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
static bool is_leaf(bt_node_t* node)
{
	return node == NULL;
}

/*
 * Improves code readability
 */
static bool is_root(btree_t* bt, bt_node_t* node)
{
	return bt->root == node;
}

static int min(int x, int y)
{
	return x < y ? x : y;
}

static void children_shift_left(bt_node_t* nodes[], int n)
{
	for(int i = 0; i < n; i++)
	{
		nodes[i] = nodes[i+1];
	}
}
static void children_shift_right(bt_node_t* nodes[], int n)
{
	for(int i = n; i > 0; i--)
	{
		nodes[i] = nodes[i-1];
	}
}

static void node_entry_set_null(bt_node_t* node, int entry_index, int n)
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
static bt_entry_t* cpy_entry(bt_entry_t* entry_original)
{
	return bt_create_entry(entry_original->key, entry_original->object);
}

/**
  * bt_node_t*, int -> int
  * EFFECTS: gets the index of an entry in an array of entries
  * RETURNS: the index of the entry if found, -1 otherwise
  */
static int get_entry_index(bt_node_t* node, int key)
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

