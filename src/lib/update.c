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
  * Code for handling the Updates in the B-Tree
  */

#include "lib.h"



/**
  * cbt_node_t*, int, void*, int -> void* 
  * EFFECTS: Updates the pointer of a certain entry with the specified key if it exists
  * MODIFIES: cbt_node_t* 
  * RETURNS: Pointer to the old node or NULL if the entry was not found
  *
  */
static void* cbt_update_helper(cbt_node_t* node, int key, void* new_object, int n)
{
	if(node == NULL)
	{
		return NULL;
	}
	
	cbt_entry_t* entry =  bt_node_search_helper(node->entry, key, 0, node->len);
	if(entry == NULL)
	{
		int index = get_next_node_index(node, key, n);
		return cbt_update_helper(node->children[index], key, new_object, n);
	}
	
	void* old_object = entry->object;
	entry->object = new_object;
	return old_object;
}
