/**
  * Code for handling the insertions in the B-Tree
  */
#include "lib.h"

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
		node_insert_entry(root, entry, true, bt->n);
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
		node_insert_entry(root, entry, true, bt->n);
	}
	else
	{
		bt_node_t* splitted_node = NULL;
		bt_entry_t* median_entry = bt_split_child(root->children[child_index], &splitted_node, bt->n);

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




