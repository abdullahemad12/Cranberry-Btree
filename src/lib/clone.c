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
  * Helpers functions for the clone
  */


#include "lib.h"


/**
  * cranbtree_t*, cranbtree_t* -> void
  * EFFECTS: copies the metadata over from the src cranbtree to the destination cranbtree
  * REQUIRES: the metadata of the original to be valid and correct
  * MODIFIES: cranbtree_t* dest
  * PARAMETERS: 
  * - cranbtree_t* src: the source cranbtree which contains the infromation to be copied.
  * - cranbtree_t* dest: the destination cranbtree
  */
static void cbt_copy_metadata(cranbtree_t* src, cranbtree_t* dest)
{
	dest->root = NULL;
	dest->length = src->length;
	dest->max_key = src->max_key;
	dest->min_key = src->min_key;
	dest->n = src->n;
	dest->is_clone = true;
} 


/** 
  * cbt_node_t*, int -> cbt_node_t* 
  * EFFECTS: Given a root of the tree, creates a clone of all of the node in the tree
  * RETURNS: the new root of the clone
  * PARAMETERS:
  * - cbt_node_t* node: root of the cranbtree to be cloned
  * - int n: the order of the tree
  *
  */

static cbt_node_t* cbt_copy_nodes(cbt_node_t* node, int n)
{
	if(node == NULL)
	{
		return NULL;
	}
	cbt_node_t* new_node = bt_create_node(n);
	if(new_node == NULL)
	{
		return NULL;
	}

	/*copies the entries*/
	for(int i = 0; i < n; i++)
	{
		if(node->entry[i] != NULL)
		{
			new_node->entry[i] = cpy_entry(node->entry[i]);
		}
	}
	
	/*copies the children*/
	for(int i = 0; i < n + 1; i++)
	{
		new_node->children[i] = cbt_copy_nodes(node->children[i], n);
	}
	new_node->len = node->len;	
	return new_node;
}