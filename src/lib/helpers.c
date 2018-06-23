/**
  * This contains Helpers
  */


/*
 * Improves code readability
 */
static bool is_root(btree_t* bt, bt_node_t* node)
{
	return bt->root == node;
}


/*
 * Improves code readability
 */
static bool is_leaf(bt_node_t* node)
{
	return node == NULL;
}


static int ceil_fn(double n)
{
	int n_tmp = (int) n;
	return n_tmp < n ? n_tmp+1 : n_tmp;
}



static int calculate_depth(bt_node_t* node)
{
	if(node == NULL)
	{
		return 0;
	}
	return 1 + calculate_depth(node->children[0]);
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

