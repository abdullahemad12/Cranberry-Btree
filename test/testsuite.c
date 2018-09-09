#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>

cbt_node_t *root_global = NULL;
bool entriescmp(cbt_node_t * root1, cbt_node_t * root2, int n);
bool treecmp(cbt_node_t * root1, cbt_node_t * root2, int n);
static void bubblesort(int keys[], int n);
static cbt_node_t *get_node_by_key(cbt_node_t * root, int key, int n);
static void debug_log(cbt_node_t * x, int n);
int get_leaf_key(cbt_node_t * node);
void validate_tree(cbt_node_t * node, int n);
static bool contains(int arr[], int n, int key);
void pickNRandomNumber(int arr[], int n);
void *copy_int_obj(void *x);

cbt_node_t *node21 = NULL;
cbt_node_t *children_original21[22];

void comp_test(void)
{
	cranbtree_t *bt = cbt_create(3);

	int m = 80;

	int keys[] =
	    { 7, 38, 79, 77, 87, 98, 68, 96, 76, 27, 23, 63, 22, 52, 49, 24, 80,
		61, 69, 100, 5, 16, 26, 25, 9, 75, 86, 4, 36, 57, 81, 1, 41, 43,
		39,
		48, 70, 85, 18, 56, 29, 82, 45, 46, 37, 99, 60, 90, 83, 2, 64,
		14,
		78, 66, 3, 21, 97, 6, 71, 84, 40, 65, 55, 15, 72, 33, 67, 62,
		89, 53,
		8, 51, 50, 34, 94, 12, 31, 88, 44, 17, 47, 28, 19, 74, 32, 59,
		92,
		73, 11, 95, 91, 42, 35, 13, 10, 54, 30, 58, 93, 20
	};

	for (int i = 0; i < m; i++)
	{
		int *z = malloc(sizeof(int));

		void *insertedObj = cbt_insert(bt, keys[i], z);

		CU_ASSERT_PTR_NOT_NULL(insertedObj);
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	for (int i = 0; i < m; i++)
	{
		int key = get_leaf_key(bt->root);
		void *object = cbt_delete(bt, key);

		CU_ASSERT_PTR_NOT_NULL(object);
		CU_ASSERT_PTR_NULL(cbt_search(bt, key));
		free(object);
	}

	cbt_destroy(bt, free);

}

void validate_tree_insertions_test(void)
{
	cranbtree_t *bt = cbt_create(3);

	int m = 100000;

	int keys[m];

	srand(time(NULL));

	for (int i = 0; i < m; i++)
	{
		int r = rand() % m;

		keys[i] = r;
	}

	for (int i = 0; i < m; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	validate_tree(bt->root, bt->n);

	cbt_destroy(bt, free);
}

void get_entry_index_test(void)
{

	cbt_node_t *node = bt_create_node(11);

	for (int i = 0; i < 10; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
		node->len++;
	}

	int index = get_entry_index(node, 5);

	CU_ASSERT_EQUAL(index, 5);

	index = get_entry_index(node, 5);
	CU_ASSERT_EQUAL(index, 5);

	index = get_entry_index(node, 9);
	CU_ASSERT_EQUAL(index, 9);

	index = get_entry_index(node, 0);
	CU_ASSERT_EQUAL(index, 0) index = get_entry_index(node, 10);
	CU_ASSERT_EQUAL(index, -1) bt_destroy_node(node, 10, free);

}

/*
 * Creation test
 */
void bt_create_test(void)
{
	cranbtree_t *bt = cbt_create(10);

	CU_ASSERT_PTR_NOT_NULL_FATAL(bt);
	CU_ASSERT_PTR_NULL(bt->root);
	CU_ASSERT(bt->length == 0);
	CU_ASSERT(bt->max_key == -1);
	CU_ASSERT(bt->n == 11);
	free(bt);

	bt = cbt_create(101);
	CU_ASSERT_PTR_NOT_NULL_FATAL(bt);
	CU_ASSERT(bt->root == NULL);
	CU_ASSERT(bt->length == 0);
	CU_ASSERT(bt->max_key == -1);
	CU_ASSERT(bt->n == 101);
	free(bt);

	bt = cbt_create(3);
	CU_ASSERT_PTR_NOT_NULL_FATAL(bt);
	CU_ASSERT(bt->root == NULL);
	CU_ASSERT(bt->length == 0);
	CU_ASSERT(bt->max_key == -1);
	CU_ASSERT(bt->n == 3);
	free(bt);

	CU_PASS("Creation test passes!");

}

void bt_create_test1(void)
{
	cranbtree_t *bt = cbt_create(0);

	CU_ASSERT_PTR_NULL_FATAL(bt);
	bt = cbt_create(1);
	CU_ASSERT_PTR_NULL_FATAL(bt);
	CU_PASS("Creation test1 passes!");
}

void bt_create_node_test(void)
{
	cbt_node_t *node = bt_create_node(10);

	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->children);
	free(node->entry);
	free(node->children);
	free(node);

	node = bt_create_node(3);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->children);
	free(node->entry);
	free(node->children);
	free(node);

	node = bt_create_node(1);
	CU_ASSERT_PTR_NULL_FATAL(node);

	CU_PASS("Node Creation test passes!");
}

void bt_create_entry_test(void)
{
	int z = 3;
	cbt_entry_t *entry = bt_create_entry(1, &z);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT(entry->key == 1);
	CU_ASSERT(entry->object == &z);

	free(entry);
	CU_PASS("Entry Creation test passes!");
}

void bt_destroy_entry_test(void)
{
	int *z = malloc(sizeof(int));
	int *y = malloc(sizeof(int));
	int x = 4;
	cbt_entry_t *entry1 = bt_create_entry(1, z);
	cbt_entry_t *entry2 = bt_create_entry(2, y);
	cbt_entry_t *entry3 = bt_create_entry(3, &x);

	bt_destroy_entry(entry1, free);
	bt_destroy_entry(entry2, free);
	bt_destroy_entry(entry3, NULL);
	CU_PASS
	    ("This test passes if there are no memory leaks or seg faults!...!");
}

void bt_destroy_node_test(void)
{
	int *x = malloc(sizeof(int));
	int *y = malloc(sizeof(int));
	int *z = malloc(sizeof(int));

	cbt_entry_t *entry1 = bt_create_entry(1, x);
	cbt_entry_t *entry2 = bt_create_entry(2, y);
	cbt_entry_t *entry3 = bt_create_entry(3, z);

	cbt_node_t *node = bt_create_node(3);

	node->entry[0] = entry1;
	node->entry[1] = entry2;
	node->entry[2] = entry3;

	bt_destroy_node(node, 3, free);
	CU_PASS
	    ("This test passes if there are no memory leaks or seg faults!...!");
}

void bt_destroy_node_test1(void)
{
	int *x = malloc(sizeof(int));
	int *y = malloc(sizeof(int));

	cbt_entry_t *entry1 = bt_create_entry(1, x);
	cbt_entry_t *entry2 = bt_create_entry(2, y);

	cbt_node_t *node = bt_create_node(3);

	node->entry[0] = entry1;
	node->entry[2] = entry2;

	bt_destroy_node(node, 3, free);
	CU_PASS
	    ("This test passes if there are no memory leaks or seg faults!...!");
}

void ceil_test()
{
	double x = 3.2;
	double y = 3.0;
	double z = 5.9;
	double w = 5.0;

	int x1 = ceil_fn(x);
	int y1 = ceil_fn(y);
	int z1 = ceil_fn(z);
	int w1 = ceil_fn(w);

	CU_ASSERT_EQUAL(x1, 4);
	CU_ASSERT_EQUAL(y1, 3);
	CU_ASSERT_EQUAL(z1, 6);
	CU_ASSERT_EQUAL(w1, 5);

}

void is_full_node_test(void)
{
	int *x = malloc(sizeof(int));
	int *y = malloc(sizeof(int));
	int *z = malloc(sizeof(int));

	cbt_entry_t *entry1 = bt_create_entry(1, x);
	cbt_entry_t *entry2 = bt_create_entry(2, y);
	cbt_entry_t *entry3 = bt_create_entry(3, z);

	cbt_node_t *node = bt_create_node(3);

	node->entry[0] = entry1;
	node->entry[1] = entry2;
	node->entry[2] = entry3;

	for (int i = 0; i < 4; i++)
	{
		node->children[i] = node;
	}

	CU_ASSERT(is_full_node(node, 3));

	bt_destroy_node(node, 3, free);

	/*node with 10 entries */
	node = bt_create_node(10);
	for (int i = 0; i < 11; i++)
	{
		node->children[i] = node;
	}

	for (int i = 0; i < 10; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	CU_ASSERT(is_full_node(node, 10));

	bt_destroy_node(node, 10, free);

	CU_PASS("The is full node test passes");
}

void is_full_node_test1(void)
{
	int *x = malloc(sizeof(int));
	int *z = malloc(sizeof(int));

	cbt_entry_t *entry1 = bt_create_entry(1, x);
	cbt_entry_t *entry3 = bt_create_entry(3, z);

	cbt_node_t *node = bt_create_node(3);

	node->entry[0] = entry1;
	node->entry[2] = entry3;

	for (int i = 0; i < 4; i++)
	{
		node->children[i] = node;
	}

	CU_ASSERT(!is_full_node(node, 3));

	bt_destroy_node(node, 3, free);
	CU_PASS("The is full node test passes");
}

void bt_split_child_test(void)
{
	int *x = malloc(sizeof(int));
	int *z = malloc(sizeof(int));

	cbt_entry_t *entry1 = bt_create_entry(1, x);
	cbt_entry_t *entry3 = bt_create_entry(3, z);

	cbt_node_t *node = bt_create_node(3);

	node->entry[0] = entry1;
	node->entry[2] = entry3;

	for (int i = 0; i < 4; i++)
	{
		node->children[i] = node;
	}

	CU_ASSERT_PTR_NULL_FATAL(bt_split_child(node, NULL, 3));
	bt_destroy_node(node, 3, free);
	CU_PASS("The split_child test passes");
}

/*even case*/
void bt_split_child_test1(void)
{

	cbt_node_t *node = bt_create_node(10);
	cbt_node_t *children_original[11];

	for (int i = 0; i < 11; i++)
	{
		node->children[i] = bt_create_node(10);
		children_original[i] = node->children[i];
	}

	for (int i = 0; i < 10; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	cbt_node_t *splitted_node = NULL;
	cbt_entry_t *entry = bt_split_child(node, &splitted_node, 10);

	CU_ASSERT_PTR_NOT_NULL_FATAL(splitted_node);
	CU_ASSERT_EQUAL(entry->key, 4);

	for (int i = 0; i < 4; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	for (int i = 4; i < 10; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		CU_ASSERT_EQUAL(splitted_node->entry[i]->key, i + 5);
	}
	for (int i = 6; i < 10; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}

	for (int i = 0; i < 11; i++)
	{
		bt_destroy_node(children_original[i], 10, free);
	}
	bt_destroy_node(node, 10, free);
	bt_destroy_node(splitted_node, 10, free);
	free(entry->object);
	free(entry);
	CU_PASS("The split_child test passes");

}

/*odd case*/
void bt_split_child_test2(void)
{
	cbt_node_t *node = bt_create_node(21);
	cbt_node_t *children_original[22];

	for (int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(22);
		children_original[i] = node->children[i];
	}
	for (int i = 0; i < 21; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	cbt_node_t *splitted_node = NULL;
	cbt_entry_t *entry = bt_split_child(node, &splitted_node, 21);

	CU_ASSERT_PTR_NOT_NULL_FATAL(splitted_node);
	CU_ASSERT_EQUAL(entry->key, 10);

	for (int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	for (int i = 10; i < 21; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(splitted_node->entry[i]->key, i + 11);
	}
	for (int i = 10; i < 21; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}
	for (int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original[i], 22, free);
	}
	bt_destroy_node(node, 10, free);
	bt_destroy_node(splitted_node, 10, free);
	free(entry->object);
	free(entry);
	CU_PASS("The split_child test passes");

}

/*first insertion*/
void bt_insert_helper_test(void)
{
	cbt_entry_t *entries[5];
	cranbtree_t *bt = cbt_create(5);

	for (int i = 0; i < 5; i++)
	{
		int *z = malloc(sizeof(int));

		entries[i] = bt_create_entry(i + 1, z);
	}
	cbt_node_t *root = NULL;

	for (int i = 0; i < 5; i++)
	{
		bt_insert_helper(bt, root, entries[i]);
		cbt_node_t *new_root = bt->root;

		if (i != 0)
		{
			CU_ASSERT_EQUAL(root, new_root);
		}
		root = new_root;
		CU_ASSERT_PTR_NOT_NULL_FATAL(root);
		CU_ASSERT_PTR_NULL(root->children[i]);
		CU_ASSERT_PTR_NOT_NULL_FATAL(root->entry[i]);
		CU_ASSERT_EQUAL(root->entry[i]->key, i + 1);

	}

	int *z = malloc(sizeof(int));
	cbt_entry_t *entry = bt_create_entry(78, z);

	bt_insert_helper(bt, root, entry);

	CU_ASSERT(root != bt->root);
	root = bt->root;
	CU_ASSERT_EQUAL(root->entry[0]->key, 3);
	CU_ASSERT_EQUAL(root->children[0]->entry[0]->key, 1);
	CU_ASSERT_EQUAL(root->children[0]->entry[1]->key, 2);
	CU_ASSERT_EQUAL(root->children[1]->entry[0]->key, 4);
	CU_ASSERT_EQUAL(root->children[1]->entry[1]->key, 5);
	CU_ASSERT_EQUAL(root->children[1]->entry[2]->key, 78);
	bt_destroy_node(root->children[0], 5, free);
	bt_destroy_node(root->children[1], 5, free);
	bt_destroy_node(root, 5, free);
	free(bt);
	CU_PASS("The insert helper works");
}

/*builds a tree with the insert*/
void bt_insert_helper_test1(void)
{

	cranbtree_t *bt = cbt_create(5);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}
	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	cbt_destroy(bt, free);

}

void bt_insert_helper_test2(void)
{
	int n = 10000;
	int keys[n];
	cranbtree_t *bt = cbt_create(15);

	srand(time(NULL));

	for (int i = 0; i < n; i++)
	{
		int r = rand() % 10000;

		keys[i] = r;
	}

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < n; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}
	cbt_destroy(bt, free);
}

void node_shift_right_test(void)
{

	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 20; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	node_shift_right(node, 11, 21);

	CU_ASSERT_PTR_NULL(node->entry[11]);

	for (int i = 0; i < 11; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
		CU_ASSERT_EQUAL(node->children[i], children_original21[i]);
	}
	for (int i = 12; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i + 1], children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
}

void node_shift_right_test1(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 20; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}
	node_shift_right(node, 19, 21);

	CU_ASSERT_PTR_NULL(node->entry[19]);

	for (int i = 0; i < 19; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
		CU_ASSERT_EQUAL(node->children[i], children_original21[i]);
	}
	for (int i = 20; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i + 1], children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}

}

void node_shift_right_test2(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 20; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	node_shift_right(node, 0, 21);

	CU_ASSERT_PTR_NULL(node->entry[0]);

	for (int i = 1; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i + 1], children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
}

void node_shift_left_test(void)
{
	cbt_node_t *node = bt_create_node(21);
	int ptr = 0;

	for (int i = 0; i < 21; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[ptr++] = bt_create_entry(i, x);
		node->len++;
	}

	free(node->entry[5]->object);
	free(node->entry[5]);
	node->entry[5] = NULL;
	node->len--;

	node_shift_left(node, 5, 21);

	for (int i = 0; i < 5; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	CU_ASSERT_EQUAL(node->entry[5]->key, 6);
	for (int i = 6; i < 20; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i + 1);
	}

	bt_destroy_node(node, 21, free);
}

void node_insert_entry_test(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;

	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0)
		{
			int *x = malloc(sizeof(int));

			node->entry[ptr++] = bt_create_entry(i, x);
		}
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry1 = bt_create_entry(31, x);

	node_insert_entry(node, entry1, true, 21);

	bool found = false;

	for (int i = 0; i < 21; i++)
	{
		if (node->entry[i] == NULL)
		{
			continue;
		}
		if (node->entry[i]->key == 31)
		{
			found = true;
		}
		for (int j = i; j < 21; j++)
		{
			if (node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <=
					  node->entry[j]->key);
			}
		}

	}

	CU_ASSERT(found);
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}

}

void node_insert_entry_test1(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 1; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;

	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0)
		{
			int *x = malloc(sizeof(int));

			node->entry[ptr++] = bt_create_entry(i, x);
		}
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry1 = bt_create_entry(0, x);

	node_insert_entry(node, entry1, true, 21);

	bool found = false;

	for (int i = 0; i < 21; i++)
	{
		if (node->entry[i] == NULL)
		{
			continue;
		}
		if (node->entry[i]->key == 0)
		{
			found = true;
		}
		for (int j = i; j < 21; j++)
		{
			if (node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <=
					  node->entry[j]->key);
			}
		}

	}

	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		if (children_original21[i] != NULL)
		{
			bt_destroy_node(children_original21[i], 21, NULL);
		}
		children_original21[i] = NULL;
	}

}

void node_insert_entry_test2(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 1; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;

	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0)
		{
			int *x = malloc(sizeof(int));

			node->entry[ptr++] = bt_create_entry(i, x);
		}
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry1 = bt_create_entry(70, x);

	node_insert_entry(node, entry1, true, 21);

	bool found = false;

	for (int i = 0; i < 21; i++)
	{
		if (node->entry[i] == NULL)
		{
			continue;
		}
		if (node->entry[i]->key == 70)
		{
			found = true;
		}
		for (int j = i; j < 21; j++)
		{
			if (node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <=
					  node->entry[j]->key);
			}
		}

	}

	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 21; i++)
	{
		if (children_original21[i] != NULL)
		{
			bt_destroy_node(children_original21[i], 21, NULL);
		}
		children_original21[i] = NULL;
	}

}

void node_insert_entry_test3(void)
{
	cbt_node_t *node = bt_create_node(21);

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry1 = bt_create_entry(70, x);

	node_insert_entry(node, entry1, true, 21);

	bool found = false;

	for (int i = 0; i < 21; i++)
	{
		if (node->entry[i] == NULL)
		{
			continue;
		}
		if (node->entry[i]->key == 70)
		{
			found = true;
		}
		for (int j = i; j < 21; j++)
		{
			if (node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <=
					  node->entry[j]->key);
			}
		}

	}

	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);

}

void get_next_node_index_test(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 21; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry = bt_create_entry(50, x);

	int i = get_next_node_index(node, entry->key, 21);

	CU_ASSERT_EQUAL(i, 21);

	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	free(x);
	free(entry);
}

void get_next_node_index_test1(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 3; i < 24; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i - 3] = bt_create_entry(i, x);
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry = bt_create_entry(1, x);

	int i = get_next_node_index(node, entry->key, 21);

	CU_ASSERT_EQUAL(i, 0);

	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	free(x);
	free(entry);
}

void bt_node_search_helper_test(void)
{
	int keys[10000];
	cbt_entry_t *entries[10000];

	(void)keys;
	(void)entries;
	srand(time(NULL));

	for (int i = 0; i < 10000; i++)
	{
		int r = rand();

		r = r % 10000;
		keys[i] = r;
	}
	bubblesort(keys, 10000);

	for (int i = 0; i < 10000; i++)
	{
		int *x = malloc(sizeof(int));

		entries[i] = bt_create_entry(keys[i], x);
	}
	for (int i = 0; i < 10000; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_node_search_helper
				       (entries, keys[i], 0, 10000));
	}

	for (int i = 0; i < 10000; i++)
	{
		int r = rand();

		r = r % 10000;
		if (bt_node_search_helper(entries, keys[i], 0, 10000) == NULL)
		{
			for (int j = 0; j < 10000; j++)
			{
				CU_ASSERT_NOT_EQUAL(r, keys[i]);
			}
		}

	}

	for (int i = 0; i < 10000; i++)
	{
		free(entries[i]->object);
		free(entries[i]);
	}
}

void get_next_node_index_test2(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 20; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 19; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(2 * i, x);
	}

	int *x = malloc(sizeof(int));
	cbt_entry_t *entry = bt_create_entry(31, x);

	int i = get_next_node_index(node, entry->key, 21);

	CU_ASSERT_EQUAL(i, 16);

	bt_destroy_node(node, 21, free);
	node = NULL;
	for (int i = 0; i < 20; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	free(x);
	free(entry);
}

void split_full_root_test(void)
{
	cbt_node_t *node = bt_create_node(21);

	for (int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for (int i = 0; i < 21; i++)
	{
		int *x = malloc(sizeof(int));

		node->entry[i] = bt_create_entry(i, x);
	}

	cbt_node_t *test_node = split_full_root(node, 21);

	CU_ASSERT_NOT_EQUAL(test_node, node);
	CU_ASSERT_EQUAL(test_node->entry[0]->key, 10);
	CU_ASSERT_EQUAL(test_node->children[0], node);

	for (int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(test_node->children[0]->entry[i]->key, i);
	}
	for (int i = 11; i < 21; i++)
	{
		CU_ASSERT_EQUAL(test_node->children[1]->entry[i - 11]->key, i);
	}

	bt_destroy_node(node, 21, free);
	bt_destroy_node(test_node->children[1], 21, free);
	bt_destroy_node(test_node, 21, free);
	for (int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}

}

void cbt_delete_leaf_case_test(void)
{

	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}
	/*cbt_node_t* node =  get_node_by_key(bt->root, 6, 3);
	   cbt_node_t* parent =  get_node_by_key(bt->root, 50, 3);

	   void* object = cbt_delete_leaf_case(bt, parent, node, 0,5);
	   free(object);
	   CU_ASSERT_PTR_NULL(cbt_search(bt, 5));

	   for(int i = 0; i < 18; i++)
	   {
	   if(keys[i] != 5)
	   {
	   CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	   }
	   } */

	cbt_destroy(bt, free);
}

void cbt_delete_leaf_case_test1(void)
{

	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	void *object = bt_delete_helper(bt, bt->root, 57);

	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 45);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 9);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 10);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 60);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 73);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 300);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 100);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 80);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 85);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 71);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 200);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 50);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 70);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 72);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 6);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 5);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);
	object = bt_delete_helper(bt, bt->root, 8);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		CU_ASSERT_PTR_NULL(object);
	}

	cbt_destroy(bt, free);
}

int get_leaf_key(cbt_node_t * node)
{
	if (is_leaf(node->children[0]))
	{
		int r = rand() % node->len;

		return node->entry[r]->key;
	}
	int r = rand() % node->len + 1;

	return get_leaf_key(node->children[r]);
}

void bt_delete_test(void)
{

	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	//cbt_node_t* parent =  get_node_by_key(bt->root, 50, 3);
	//cbt_node_t* parent2 =  get_node_by_key(bt->root, 80, 3);

	//cbt_node_t* node =  get_node_by_key(bt->root, 57, 3);
	//cbt_node_t* node1 =  get_node_by_key(bt->root, 70, 3);

	void *object = bt_delete_helper(bt, bt->root, 57);

	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 70);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 73);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 71);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 72);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 6);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 5);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 8);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 200);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 300);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 100);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 10);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);
	object = bt_delete_helper(bt, bt->root, 9);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 80);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 45);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 60);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 50);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	object = bt_delete_helper(bt, bt->root, 85);
	CU_ASSERT_PTR_NOT_NULL(object);
	free(object);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		CU_ASSERT_PTR_NULL(object);
	}

	cbt_destroy(bt, free);
}

void bt_delete_test2(void)
{
	cranbtree_t *bt = cbt_create(3);
	cranbtree_t *bt1 = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	/*tries to delete a key that doesnot exist */
	void *object = cbt_delete(bt, 500);

	CU_ASSERT_PTR_NULL(object);

	/*deletes from an empty tree */
	object = cbt_delete(bt1, 200);
	CU_ASSERT_PTR_NULL(object);

	cbt_destroy(bt, free);
	cbt_destroy(bt1, free);
}

void cbt_calculate_min_key_test(void)
{
	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	int min = cbt_calculate_min_key(bt->root);

	CU_ASSERT_EQUAL(min, 5);
	CU_ASSERT_EQUAL(bt->min_key, 5);

	free(cbt_delete(bt, 5));

	min = cbt_calculate_min_key(bt->root);
	CU_ASSERT_EQUAL(min, 6);
	CU_ASSERT_EQUAL(bt->min_key, 6);

	int *z = malloc(sizeof(int));

	cbt_insert(bt, 3, z);

	min = cbt_calculate_min_key(bt->root);
	CU_ASSERT_EQUAL(min, 3);
	CU_ASSERT_EQUAL(bt->min_key, 3);

	cbt_destroy(bt, free);
}

void cbt_calculate_max_key_test(void)
{
	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	int max = cbt_calculate_max_key(bt->root);

	CU_ASSERT_EQUAL(max, 300);
	CU_ASSERT_EQUAL(bt->max_key, 300);

	free(cbt_delete(bt, 300));

	max = cbt_calculate_max_key(bt->root);
	CU_ASSERT_EQUAL(max, 200);
	CU_ASSERT_EQUAL(bt->max_key, 200);

	int *z = malloc(sizeof(int));

	cbt_insert(bt, 400, z);

	max = cbt_calculate_max_key(bt->root);
	CU_ASSERT_EQUAL(max, 400);
	CU_ASSERT_EQUAL(bt->max_key, 400);

	cbt_destroy(bt, free);
}

/**
  * NULL pointer
  */
void cbt_clone_test1(void)
{
	cranbtree_t *clone = cbt_clone(NULL);

	CU_ASSERT_PTR_NULL(clone);
}

/**
  * empty tree
  */
void cbt_clone_test2(void)
{
	cranbtree_t *bt = cbt_create(3);

	cranbtree_t *clone = cbt_clone(bt);

	CU_ASSERT_PTR_NOT_NULL_FATAL(clone);
	CU_ASSERT_PTR_NULL(clone->root);
	CU_ASSERT_EQUAL(clone->length, 0);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, -1);
	CU_ASSERT_EQUAL(clone->min_key, -1);

	cbt_destroy(clone, free);
	cbt_destroy(bt, free);
}

/**
  * one node
  */
void cbt_clone_test3(void)
{
	cranbtree_t *bt = cbt_create(5);

	for (int i = 0; i < 4; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
	}
	cranbtree_t *clone = cbt_clone(bt);

	CU_ASSERT_PTR_NOT_NULL_FATAL(clone);

	CU_ASSERT_EQUAL(clone->length, bt->length);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, bt->max_key);
	CU_ASSERT_EQUAL(clone->min_key, bt->min_key);
	CU_ASSERT(clone->is_clone);
	CU_ASSERT(treecmp(bt->root, clone->root, bt->n));

	cbt_destroy(clone, free);
	cbt_destroy(bt, free);
}

/**
  * many nodes
  */
void cbt_clone_test4(void)
{
	cranbtree_t *bt = cbt_create(5);

	int keys[] =
	    { 2, 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300,
		45, 10, 56, 45, 56, 345, 675, 675, 4566, 2345, 543, 222, 5566,
		7554,
		3453
	};
	for (int i = 0; i < 32; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}
	cranbtree_t *clone = cbt_clone(bt);

	CU_ASSERT_PTR_NOT_NULL_FATAL(clone);

	CU_ASSERT_EQUAL(clone->length, bt->length);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, bt->max_key);
	CU_ASSERT_EQUAL(clone->min_key, bt->min_key);
	CU_ASSERT(clone->is_clone);
	CU_ASSERT(treecmp(bt->root, clone->root, bt->n));

	cbt_destroy(clone, free);
	cbt_destroy(bt, free);
}

/**
  * insertion, deletion and update on a clone, are no-op (invaild) operations
  */
void cbt_clone_test5(void)
{
	cranbtree_t *bt = cbt_create(5);
	static int keys[50];
	int n = 50;

	/*test no initial error recorded */
	int errorNo = cbt_errno(bt);
	const char *errorMessage = cbt_errstr(bt);

	CU_ASSERT(errorNo == 0);
	CU_ASSERT_PTR_NULL(errorMessage);

	pickNRandomNumber(keys, n);
	int n1 = 25;

	for (int i = 0; i < n1; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}
	cranbtree_t *clone = cbt_clone(bt);

	CU_ASSERT_PTR_NOT_NULL_FATAL(clone);

	CU_ASSERT_EQUAL(clone->length, bt->length);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, bt->max_key);
	CU_ASSERT_EQUAL(clone->min_key, bt->min_key);
	CU_ASSERT(clone->is_clone);
	CU_ASSERT(treecmp(bt->root, clone->root, bt->n));

	/*tries search for all the keys */
	for (int i = 0; i < n1; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		CU_ASSERT_PTR_NOT_NULL(object);
		void *object1 = cbt_search(clone, keys[i]);

		CU_ASSERT_PTR_NOT_NULL(object1);
		CU_ASSERT_PTR_EQUAL(object, object1);
	}

	/*tries to update all the entries */
	for (int i = 0; i < n1; i++)
	{
		int *z = malloc(sizeof(int));

		clone->op_errno = CBT_NO_ERROR;
		void *oldobj = cbt_search(clone, keys[i]);

		void *object = cbt_update(clone, keys[i], z);

		CU_ASSERT_PTR_NULL(object);
		CU_ASSERT_EQUAL(clone->op_errno, CBT_CLONE_BAD_OP);

		void *newobj = cbt_search(clone, keys[i]);

		CU_ASSERT_EQUAL(oldobj, newobj);
		CU_ASSERT_NOT_EQUAL(z, newobj);
	}

	for (int i = n1; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		clone->op_errno = CBT_NO_ERROR;
		cbt_insert(clone, keys[i], z);
		CU_ASSERT_EQUAL(clone->op_errno, CBT_CLONE_BAD_OP);
	}

	/*makes sure nothing was inserted */

	CU_ASSERT_EQUAL(clone->length, bt->length);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, bt->max_key);
	CU_ASSERT_EQUAL(clone->min_key, bt->min_key);
	CU_ASSERT(clone->is_clone);
	CU_ASSERT(treecmp(bt->root, clone->root, bt->n));

	/*deletes the object */
	for (int i = 0; i < n1; i++)
	{
		clone->op_errno = CBT_NO_ERROR;
		void *object = cbt_delete(clone, keys[i]);

		CU_ASSERT_PTR_NULL(object);
		CU_ASSERT_EQUAL(clone->op_errno, CBT_CLONE_BAD_OP);
	}

	/*makes sure nothing was deleted */

	CU_ASSERT_EQUAL(clone->length, bt->length);
	CU_ASSERT_EQUAL(clone->n, bt->n);
	CU_ASSERT_EQUAL(clone->max_key, bt->max_key);
	CU_ASSERT_EQUAL(clone->min_key, bt->min_key);
	CU_ASSERT(treecmp(bt->root, clone->root, bt->n));

	/*last operation of clone failed (was ignored), test error message */
	errorNo = cbt_errno(clone);
	errorMessage = cbt_errstr(clone);
	CU_ASSERT(errorNo == CBT_CLONE_BAD_OP);
	CU_ASSERT_STRING_EQUAL(errorMessage,
			       "Cannot perform this operation on a cloned tree");

	cbt_destroy(clone, free);
	cbt_destroy(bt, free);
}

void cbt_detach_clone_test1(void)
{
	cranbtree_t *cbt = NULL;
	cranbtree_t *clone = cbt_clone(cbt);

	cbt_detach_clone(clone, NULL);
	CU_ASSERT_PTR_NULL(clone);
}

void cbt_detach_clone_test2(void)
{
	cranbtree_t *bt = cbt_create(5);
	static int keys[50000];
	int n = 50000;

	pickNRandomNumber(keys, n);
	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	cranbtree_t *clone = cbt_clone(bt);

	cbt_detach_clone(clone, NULL);
	CU_ASSERT_EQUAL(clone->is_clone, false);

	for (int i = 0; i < n; i++)
	{
		void *obj_original = cbt_search(bt, keys[i]);
		void *obj_clone = cbt_search(clone, keys[i]);

		CU_ASSERT_EQUAL(obj_original, obj_clone);
	}
	cbt_destroy(bt, free);
	cbt_destroy(clone, NULL);
}

void cbt_detach_clone_test3(void)
{
	cranbtree_t *bt = cbt_create(5);
	static int keys[50000];
	int n = 50000;

	pickNRandomNumber(keys, n);
	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		*z = i;
		cbt_insert(bt, keys[i], z);
	}

	cranbtree_t *clone = cbt_clone(bt);

	cbt_detach_clone(clone, copy_int_obj);
	CU_ASSERT_EQUAL(clone->is_clone, false);

	for (int i = 0; i < n; i++)
	{
		void *obj_original = cbt_search(bt, keys[i]);
		void *obj_clone = cbt_search(clone, keys[i]);

		CU_ASSERT_NOT_EQUAL(obj_original, obj_clone);
		int *x = (int *)obj_original;
		int *y = (int *)obj_clone;

		CU_ASSERT_EQUAL(*x, *y);
	}
	cbt_destroy(bt, free);
	cbt_destroy(clone, free);
}

void merge_leaf_nodes_test(void)
{

	cranbtree_t *bt = cbt_create(3);

	int keys[] =
	    { 9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45,
		10
	};

	for (int i = 0; i < 18; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
	}

	cbt_node_t *node1 = get_node_by_key(bt->root, 57, 3);
	cbt_node_t *node2 = get_node_by_key(bt->root, 70, 3);
	cbt_node_t *parent = get_node_by_key(bt->root, 80, 3);

	parent->children[0] = NULL;
	cbt_node_t *ret = merge_leaf_nodes(node1, node2, 3);

	(void)node1;
	(void)node2;
	CU_ASSERT_PTR_NOT_NULL(ret);
	CU_ASSERT_PTR_EQUAL(node1, ret);
	CU_ASSERT_EQUAL(ret->entry[0]->key, 57);
	CU_ASSERT_EQUAL(ret->entry[1]->key, 70);
	CU_ASSERT_PTR_NULL(ret->entry[2]);

	cbt_destroy(bt, free);
}

void delete_test_root_entry(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(7, &z);
	parent->len = 1;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(4, &z);
	child1->entry[1] = bt_create_entry(5, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(34, &z);
	child2->entry[1] = bt_create_entry(45, &z);
	child2->len = 2;
	parent->children[1] = child2;

	cbt_delete(bt, 7);
	cbt_destroy(bt, NULL);
}

void cbt_update_test(void)
{
	int n = 10000;
	cranbtree_t *bt = cbt_create(3);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
	}

	int *z = malloc(sizeof(int));
	void *oldz = cbt_update(bt, 100, z);

	CU_ASSERT_PTR_NOT_NULL_FATAL(oldz);
	free(oldz);

	int *newz = cbt_search(bt, 100);

	CU_ASSERT_EQUAL(z, newz);

	z = malloc(sizeof(int));
	oldz = cbt_update(bt, 0, z);
	CU_ASSERT_PTR_NOT_NULL_FATAL(oldz);
	free(oldz);

	newz = cbt_search(bt, 0);
	CU_ASSERT_EQUAL(z, newz);

	z = malloc(sizeof(int));
	oldz = cbt_update(bt, 9999, z);
	CU_ASSERT_PTR_NOT_NULL_FATAL(oldz);
	free(oldz);

	newz = cbt_search(bt, 9999);
	CU_ASSERT_EQUAL(z, newz);

	cbt_destroy(bt, free);
}

void cbt_update_test1(void)
{
	int n = 10000;
	cranbtree_t *bt = cbt_create(3);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
	}

	int *z = malloc(sizeof(int));
	void *oldz = cbt_update(bt, 1000000, z);

	CU_ASSERT_PTR_NULL_FATAL(oldz);

	int *newz = cbt_search(bt, 1000000);

	CU_ASSERT_EQUAL(z, newz);

	cbt_destroy(bt, free);
}

void cbt_key_not_found_error_test1(void)
{
	int n = 10000;
	cranbtree_t *bt = cbt_create(3);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
	}
	
	/*tries to delete a non-existing key*/
	void* obj = cbt_delete(bt, n * 100);
	CU_ASSERT_PTR_NULL(obj);
	int errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_KEY_NOT_FOUND);

	/*tries to search for a non-existing key*/	
	obj = cbt_search(bt, n * 100);
	CU_ASSERT_PTR_NULL(obj);
	errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_KEY_NOT_FOUND);


	/*tries to update a non-existing key*/
	int* z = malloc(sizeof(int));
	obj = cbt_update(bt, n * 100, z);
	CU_ASSERT_PTR_NULL(obj);
	errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_KEY_NOT_FOUND);


	cbt_destroy(bt, free);

}


void cbt_key_not_found_error_test2(void)
{
	int n = 10000;
	cranbtree_t *bt = cbt_create(3);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
	}
	
	/*tries to delete a existing key*/
	void* obj = cbt_delete(bt, n/2);
	CU_ASSERT_PTR_NOT_NULL(obj);
	int errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_NO_ERROR);
	free(obj);

	/*tries to search for a non-existing key*/	
	obj = cbt_search(bt, (n/2) + 1);
	CU_ASSERT_PTR_NOT_NULL(obj);
	errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_NO_ERROR);


	/*tries to update a non-existing key*/
	int* z = malloc(sizeof(int));
	obj = cbt_update(bt,(n/2) + 2, z);
	CU_ASSERT_PTR_NOT_NULL(obj);
	errorno = cbt_errno(bt);
	CU_ASSERT_EQUAL(errorno, CBT_NO_ERROR);
	free(obj);

	cbt_destroy(bt, free);

}

bool treecmp(cbt_node_t * root1, cbt_node_t * root2, int n)
{
	if ((root1 == NULL && root2 != NULL)
	    || (root2 == NULL && root1 != NULL))
	{
		return false;
	}

	if (root1 == NULL)
	{
		return true;
	}

	if (!entriescmp(root1, root2, n))
	{
		return false;
	}

	for (int i = 0; i < n + 1; i++)
	{
		if (!treecmp(root1->children[i], root2->children[i], n))
		{
			return false;
		}
	}
	return true;
}

bool entriescmp(cbt_node_t * root1, cbt_node_t * root2, int n)
{
	(void)debug_log;
	for (int i = 0; i < n; i++)
	{
		if ((root1->entry[i] == NULL && root2->entry[i] != NULL)
		    || (root2->entry[i] == NULL && root1->entry[i] != NULL))
		{

			return false;
		}

		if (root1->entry[i] != NULL)
		{
			if (root1->entry[i]->key != root2->entry[i]->key)
			{
				return false;
			}
		}
	}
	return true;
}

static void bubblesort(int keys[], int n)
{

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (keys[j] > keys[j + 1])
			{
				int tmp = keys[j];

				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
		}

	}
}

static cbt_node_t *get_node_by_key(cbt_node_t * root, int key, int n)
{

	if (root == NULL)
	{
		return NULL;
	}

	void *object = bt_node_search_helper(root->entry, key, 0, root->len);
	int i = get_next_node_index(root, key, n);

	return object != NULL ? root : get_node_by_key(root->children[i], key,
						       n);
}

// for debugging
void debug_log(cbt_node_t * x, int n)
{
	printf("\n");
	if (x == NULL)
	{
		printf("NULL node\n");
		return;
	}
	for (int i = 0; i < n; i++)
	{
		printf("\t");
	}
	print_node(x, n);
	printf("\n\n");
	for (int i = 0; i < n + 1; i++)
	{
		if (x->children[i] == NULL)
		{
			printf("|| NULL child  || ");
		}
		else
		{
			print_node(x->children[i], n);
		}

	}
	printf("\n");

}

void validate_node(cbt_node_t * node, int n)
{
	int n_entries = 0;
	int n_children = 0;
	bool end = false;
	bool messed = false;

	for (int i = 0; i < n; i++)
	{
		n_entries += node->entry[i] != NULL ? 1 : 0;
		messed = node->entry[i] != NULL && end ? true : false;
		end = node->entry[i] == NULL;
	}
	CU_ASSERT(!messed);
	CU_ASSERT_EQUAL(n_entries, node->len);
	if (node->children[0] == NULL)
	{
		return;
	}
	end = false;
	messed = false;
	for (int i = 0; i < n + 1; i++)
	{
		n_children += node->children[i] != NULL ? 1 : 0;
		messed = node->children[i] != NULL && end ? true : false;
		end = node->children[i] == NULL;
	}
	CU_ASSERT(!messed);
	CU_ASSERT_EQUAL(n_children, node->len + 1);
}

void validate_tree(cbt_node_t * node, int n)
{
	(void)contains;
	if (node == NULL)
	{
		return;
	}

	validate_node(node, n);
	for (int i = 0; i < n + 1; i++)
	{
		validate_tree(node->children[i], n);
	}
}

/**
  * returns true if the array contains the key
  */
static bool contains(int arr[], int n, int key)
{
	for (int i = 0; i < n; i++)
	{
		int cmp = arr[i];

		if (cmp == key)
		{
			return true;
		}
	}
	return false;
}

void *copy_int_obj(void *x)
{
	int *x_int = (int *)x;
	int *y = malloc(sizeof(int));

	*y = *x_int;
	return y;
}

/*****************************
 * before and after functions*
 *****************************/

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite(void)
{
	int tmp = 0;

	root_global = bt_create_node(4);
	root_global->entry[0] = bt_create_entry(70, &tmp);

	cbt_node_t *int_node1 = bt_create_node(4);

	int_node1->entry[0] = bt_create_entry(8, &tmp);
	int_node1->entry[1] = bt_create_entry(50, &tmp);

	cbt_node_t *int_node2 = bt_create_node(4);

	int_node2->entry[0] = bt_create_entry(73, &tmp);
	int_node2->entry[1] = bt_create_entry(100, &tmp);

	cbt_node_t *leaf1 = bt_create_node(4);

	leaf1->entry[0] = bt_create_entry(5, &tmp);
	leaf1->entry[1] = bt_create_entry(6, &tmp);

	cbt_node_t *leaf2 = bt_create_node(4);

	leaf2->entry[0] = bt_create_entry(9, &tmp);
	leaf2->entry[1] = bt_create_entry(10, &tmp);
	leaf2->entry[2] = bt_create_entry(45, &tmp);

	cbt_node_t *leaf3 = bt_create_node(4);

	leaf3->entry[0] = bt_create_entry(57, &tmp);
	leaf3->entry[1] = bt_create_entry(60, &tmp);

	cbt_node_t *leaf4 = bt_create_node(4);

	leaf4->entry[0] = bt_create_entry(71, &tmp);
	leaf4->entry[1] = bt_create_entry(72, &tmp);

	cbt_node_t *leaf5 = bt_create_node(4);

	leaf5->entry[0] = bt_create_entry(80, &tmp);
	leaf5->entry[1] = bt_create_entry(85, &tmp);

	cbt_node_t *leaf6 = bt_create_node(4);

	leaf6->entry[0] = bt_create_entry(200, &tmp);
	leaf6->entry[1] = bt_create_entry(300, &tmp);

	root_global->children[0] = int_node1;
	root_global->children[1] = int_node2;

	int_node1->children[0] = leaf1;
	int_node1->children[1] = leaf2;
	int_node1->children[2] = leaf3;

	int_node2->children[0] = leaf4;
	int_node2->children[1] = leaf5;
	int_node2->children[2] = leaf6;

	return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(void)
{
	destroy_bt_helper(root_global, 4, NULL);
	root_global = NULL;
	return 0;
}
