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
 * code for calculating statistics about the tree
 */
#include "lib.h"

/**
  * cbt_node_t* -> int
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

	return cbt_calculate_min_key(root->children[0]);	
}


/**
  * cbt_node_t* -> int
  * EFFECTS: calculates the maximum key in the tree
  * REQUIRES: The tree should not be empty
  * RETURNS: the maximum key in the tree
  */
int cbt_calculate_max_key(cbt_node_t* root)
{
	return 0;
}