/** Cranberry Tree Copyright (C) 2018  Abdullah Emad
  * This program comes with ABSOLUTELY NO WARRANTY. 
  * This is free software, and you are welcome to redistribute it
  * under certain conditions.
  */

/**
  * code for the search functionality
  */

# include "lib.h"

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
