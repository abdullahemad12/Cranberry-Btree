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
  * code for the visit functionality
  */

#include "lib.h"

/**
  * EFFECTS: invokes the function visitor on all the objects in all the entries 
  *           starting from the root node
  * RETURNS: 0 on success or nonzero number on failure
  * PARAMETERS: 
  * - cbt_node_t* node: a parent node
  * - int (* visitor) (int *): pointer to the function to be called
  */
static int cbt_visit_all_helper(cbt_node_t * node, int (*visitor) (void *))
{
	int err;	
	if (node == NULL)
	{
		return 0;
	}

	for (int i = 0; i < node->len; i++)
	{
		err = visitor(node->entry[i]->object);
	}

	int acc = 0;

	for (int i = 0, n = node->len + 1; i < n; i++)
	{
		acc += cbt_visit_all_helper(node->children[i], visitor);
	}
	return acc;
}
