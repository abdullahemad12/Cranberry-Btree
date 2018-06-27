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

  
/*
 * Functions for the deletion
 */

#include "lib.h"



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



/**********************************
 *						          *
 *        node delete helpers     *
 *							      *
 **********************************/


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
static cbt_entry_t* bt_delete_minimum(cranbtree_t* bt, cbt_node_t* node)
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
static cbt_entry_t* bt_delete_maximum(cranbtree_t* bt, cbt_node_t* node)
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
