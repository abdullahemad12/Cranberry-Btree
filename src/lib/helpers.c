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
  * This contains Helpers
  */


/*
 * Improves code readability
 */
static bool is_root(cranbtree_t* bt, cbt_node_t* node)
{
	return bt->root == node;
}


/*
 * Improves code readability
 */
static bool is_leaf(cbt_node_t* node)
{
	return node == NULL;
}


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

static bool print_level(cbt_node_t* root, int n, int level, int currentLevel)
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

