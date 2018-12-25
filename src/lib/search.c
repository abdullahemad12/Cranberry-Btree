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
  * code for the search functionality
  */

#include "lib.h"

static void *bt_search_helper(cbt_node_t * node, int key, int n)
{
	if (node == NULL)
	{
		return NULL;
	}

	cbt_entry_t *entry =
	    bt_node_search_helper(node->entry, key, 0, node->len);

	if (entry == NULL)
	{
		int i = get_next_node_index(node, key, n);

		return bt_search_helper(node->children[i], key, n);
	}
	void *object = entry->object;

	return object;
}

/*
 * cbt_entry_t*[], int key -> void* 
 * EFFECTS: looks for an entry in the entry list with the given search key
 * RETURNS: pointer to the object or NULL if it was not found
 *
 */
static cbt_entry_t *bt_node_search_helper(cbt_entry_t * entries[], int key,
					  int min, int max)
{
	if (min == max)
	{
		return NULL;
	}
	int avg = (max + min) / 2;

	if (key == entries[avg]->key)
	{
		return entries[avg];
	}
	else if (key > entries[avg]->key)
	{
		return bt_node_search_helper(entries, key, avg + 1, max);
	}
	else
	{
		return bt_node_search_helper(entries, key, min, avg);
	}

}


/**
  * void*, int (*)(void*) -> void*
  * EFFECTS: search for an entry in the tree according to the rules set 
  *           by the visitor and returns the object
  * RETURNS: pointer to the object or NULL if it was not found
  */
static void *cbt_navigation_search_helper(cbt_node_t* node, void* key, int (*visitor)(void*, void*))
{
	if(node == NULL)
	{
		return NULL;
	}
	for(int i = 0; i < node->len; i++)
	{
		int direction = visitor(key, node->entry[i]->object);
		if(direction < 0)
		{
			return cbt_navigation_search_helper(node->children[i], key, visitor);	
		}
		else if(direction == 0)
		{
			return node->entry[i]->object;
		}
	}
	return cbt_navigation_search_helper(node->children[node->len], key, visitor);	
}

