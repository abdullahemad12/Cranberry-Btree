#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>

bt_node_t* root_global = NULL;
bool entriescmp(bt_node_t* root1, bt_node_t* root2, int n);
bool treecmp(bt_node_t* root1, bt_node_t* root2, int n);
static void bubblesort(int keys[], int n);
static bt_node_t* get_node_by_key(bt_node_t* root, int key, int n);
static void debug_log(bt_node_t* x, int n);

bt_node_t* node21 = NULL;
bt_node_t* children_original21[22];

void get_entry_index_test(void)
{

	
	
	bt_node_t* node  = bt_create_node(11);
	
	for(int i = 0; i < 10; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
		node->len++;
	}
	

	int index = get_entry_index(node, 5);
	CU_ASSERT_EQUAL(index, 5);
	
	index = get_entry_index(node, 5);
	CU_ASSERT_EQUAL(index, 5);
	
	
	index = get_entry_index(node, 9);
	CU_ASSERT_EQUAL(index, 9);
	

	index = get_entry_index(node, 0);
	CU_ASSERT_EQUAL(index, 0)
	
	index = get_entry_index(node, 10);
	CU_ASSERT_EQUAL(index, -1)
	
	bt_destroy_node(node, 10, free);
	
}




/*
 * Creation test
 */
void bt_create_test(void)
{
	btree_t* bt = bt_create(10);
	CU_ASSERT_PTR_NOT_NULL_FATAL(bt);
	CU_ASSERT_PTR_NULL(bt->root);
	CU_ASSERT(bt->length == 0);
	CU_ASSERT(bt->max_key == -1);
	CU_ASSERT(bt->n == 11);
	free(bt);
	
	bt = bt_create(101);
	CU_ASSERT_PTR_NOT_NULL_FATAL(bt);
	CU_ASSERT(bt->root == NULL);
	CU_ASSERT(bt->length == 0);
	CU_ASSERT(bt->max_key == -1);
	CU_ASSERT(bt->n == 101);
	free(bt);
	
	bt = bt_create(3);
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
	btree_t* bt = bt_create(0);
	CU_ASSERT_PTR_NULL_FATAL(bt);
 	bt = bt_create(1);
	CU_ASSERT_PTR_NULL_FATAL(bt);
	CU_PASS("Creation test1 passes!");
}


void bt_create_node_test(void)
{
	bt_node_t* node  = bt_create_node(10);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->children);
	free(node->entry);
	free(node->children);
	free(node);
	
	node  = bt_create_node(3);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node->children);
	free(node->entry);
	free(node->children);
	free(node);
	
	
	node  = bt_create_node(1);
	CU_ASSERT_PTR_NULL_FATAL(node);
	
	CU_PASS("Node Creation test passes!");
}


void bt_create_entry_test(void)
{
	int z = 3;
	bt_entry_t* entry = bt_create_entry(1,&z);
	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT(entry->key == 1);
	CU_ASSERT(entry->object == &z);
	
	free(entry);
	CU_PASS("Entry Creation test passes!");
}


void bt_destroy_entry_test(void)
{
	int* z = malloc(sizeof(int));
	int* y = malloc(sizeof(int));
	int x = 4;
	bt_entry_t* entry1 = bt_create_entry(1,z);
	bt_entry_t* entry2 = bt_create_entry(2,y);
	bt_entry_t* entry3 = bt_create_entry(3,&x);
	
	bt_destroy_entry(entry1, free);
	bt_destroy_entry(entry2, free);
	bt_destroy_entry(entry3, NULL);
	CU_PASS("This test passes if there are no memory leaks or seg faults!...!");
}

void bt_destroy_node_test(void)
{
	int* x = malloc(sizeof(int));
	int* y = malloc(sizeof(int));
	int* z = malloc(sizeof(int));
	
	bt_entry_t* entry1 = bt_create_entry(1,x);
	bt_entry_t* entry2 = bt_create_entry(2,y);
	bt_entry_t* entry3 = bt_create_entry(3,z);
	
	bt_node_t* node  = bt_create_node(3);
	
	node->entry[0] = entry1;
	node->entry[1] = entry2;
	node->entry[2] = entry3;
	
	bt_destroy_node(node, 3, free);
	CU_PASS("This test passes if there are no memory leaks or seg faults!...!");
}
void bt_destroy_node_test1(void)
{
	int* x = malloc(sizeof(int));
	int* y = malloc(sizeof(int));
	
	bt_entry_t* entry1 = bt_create_entry(1,x);
	bt_entry_t* entry2 = bt_create_entry(2,y);

	
	bt_node_t* node  = bt_create_node(3);
	
	node->entry[0] = entry1;
	node->entry[2] = entry2;

	
	bt_destroy_node(node, 3, free);
	CU_PASS("This test passes if there are no memory leaks or seg faults!...!");
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
	int* x = malloc(sizeof(int));
	int* y = malloc(sizeof(int));
	int* z = malloc(sizeof(int));
	
	bt_entry_t* entry1 = bt_create_entry(1,x);
	bt_entry_t* entry2 = bt_create_entry(2,y);
	bt_entry_t* entry3 = bt_create_entry(3,z);
	
	bt_node_t* node  = bt_create_node(3);
	
	node->entry[0] = entry1;
	node->entry[1] = entry2;
	node->entry[2] = entry3;
	
	for(int i = 0; i < 4; i++)
	{
		node->children[i] = node;
	}
	
	CU_ASSERT(is_full_node(node, 3));
	
	bt_destroy_node(node, 3, free);
	
	
	/*node with 10 entries*/
	node  = bt_create_node(10);
	for(int i = 0; i < 11; i++)
	{
		node->children[i] = node;
	}
	
	for(int i = 0; i < 10; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	
	CU_ASSERT(is_full_node(node, 10));
	
	bt_destroy_node(node, 10, free);
	
	CU_PASS("The is full node test passes");
}

void is_full_node_test1(void)
{
	int* x = malloc(sizeof(int));
	int* z = malloc(sizeof(int));
	
	bt_entry_t* entry1 = bt_create_entry(1,x);
	bt_entry_t* entry3 = bt_create_entry(3,z);
	
	bt_node_t* node  = bt_create_node(3);
	
	node->entry[0] = entry1;
	node->entry[2] = entry3;
	
	for(int i = 0; i < 4; i++)
	{
		node->children[i] = node;
	}
	
	CU_ASSERT(!is_full_node(node, 3));
	
	bt_destroy_node(node, 3, free);
	CU_PASS("The is full node test passes");
}

void bt_split_child_test(void)
{
	int* x = malloc(sizeof(int));
	int* z = malloc(sizeof(int));
	
	bt_entry_t* entry1 = bt_create_entry(1,x);
	bt_entry_t* entry3 = bt_create_entry(3,z);
	
	bt_node_t* node  = bt_create_node(3);
	
	node->entry[0] = entry1;
	node->entry[2] = entry3;
	
	for(int i = 0; i < 4; i++)
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

	bt_node_t* node  = bt_create_node(10);
	bt_node_t* children_original[11];
	for(int i = 0; i < 11; i++)
	{
		node->children[i] = bt_create_node(10);
		children_original[i] = node->children[i];
	}
	
	for(int i = 0; i < 10; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	bt_node_t* splitted_node = NULL;
	bt_entry_t* entry = bt_split_child(node, &splitted_node, 10);
		
	CU_ASSERT_PTR_NOT_NULL_FATAL(splitted_node);
	CU_ASSERT_EQUAL(entry->key, 4);

	for(int i = 0; i < 4; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	for(int i = 4; i < 10; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}
	
	for(int i = 0; i < 5; i++)
	{
		CU_ASSERT_EQUAL(splitted_node->entry[i]->key, i + 5);
	}
	for(int i = 6; i < 10; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}
	
	for(int i = 0; i < 11; i++)
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
	bt_node_t* node  = bt_create_node(21);
	bt_node_t* children_original[22];
	for(int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(22);
		children_original[i] = node->children[i];
	}
	for(int i = 0; i < 21; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	bt_node_t* splitted_node = NULL;
	bt_entry_t* entry = bt_split_child(node, &splitted_node, 21);
	
	CU_ASSERT_PTR_NOT_NULL_FATAL(splitted_node);
	CU_ASSERT_EQUAL(entry->key, 10);
	
	for(int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	for(int i = 10; i < 21; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}
	
	for(int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(splitted_node->entry[i]->key, i + 11);
	}
	for(int i = 10; i < 21; i++)
	{
		CU_ASSERT_PTR_NULL(node->entry[i]);
	}
	for(int i = 0; i < 22; i++)
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
	bt_entry_t* entries[5];
	btree_t* bt = bt_create(5);
	for(int i = 0; i < 5; i++)
	{
		int* z = malloc(sizeof(int));
		entries[i] = bt_create_entry(i + 1,z);
	}
	bt_node_t* root = NULL;
	for(int i = 0; i < 5; i++)
	{
		bt_insert_helper(bt, root, entries[i]);
		bt_node_t* new_root = bt->root;
		if(i != 0)
		{
			CU_ASSERT_EQUAL(root, new_root);
		}
		root = new_root;
		CU_ASSERT_PTR_NOT_NULL_FATAL(root);
		CU_ASSERT_PTR_NULL(root->children[i]);
		CU_ASSERT_PTR_NOT_NULL_FATAL(root->entry[i]);
		CU_ASSERT_EQUAL(root->entry[i]->key, i + 1);
		
	}
	
	int* z = malloc(sizeof(int));
	bt_entry_t* entry = bt_create_entry(78,z);
	
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

	btree_t* bt = bt_create(5);

	int keys[] = {9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45, 10};

	for(int i = 0; i < 18; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}
	for(int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}

	
	bt_destroy(bt, free);
	
}
void bt_insert_helper_test2(void)
{
	int n = 10000;
	int keys[n];
	btree_t* bt = bt_create(15);
	srand(time(NULL));   
	
	for(int i = 0; i < n; i++)
	{
		int r = rand() % 10000;
		keys[i] = r;
	}
	
	for(int i = 0; i < n; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}

	for(int i = 0; i < n; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}
	bt_destroy(bt, free);
}

void node_shift_right_test(void)
{
	
	bt_node_t* node  = bt_create_node(21);

	for(int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 20; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	
	node_shift_right(node, 11, 21);
	
	CU_ASSERT_PTR_NULL(node->entry[11]);
	
	for(int i = 0; i < 11; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
		CU_ASSERT_EQUAL(node->children[i] , children_original21[i]);
	}
	for(int i = 12; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i + 1] , children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
}



void node_shift_right_test1(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 20; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	node_shift_right(node, 19, 21);
	
	CU_ASSERT_PTR_NULL(node->entry[19]);
	
	for(int i = 0; i < 19; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
		CU_ASSERT_EQUAL(node->children[i] , children_original21[i]);
	}
	for(int i = 20; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i + 1] , children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	
}
void node_shift_right_test2(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 20; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}

	node_shift_right(node, 0, 21);
	
	CU_ASSERT_PTR_NULL(node->entry[0]);
	
	for(int i = 1; i < 21; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node->entry[i]);
		CU_ASSERT_EQUAL(node->entry[i]->key, i - 1);
		CU_ASSERT_EQUAL(node->children[i+1] , children_original21[i]);
	}
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
}


void node_shift_left_test(void)
{
	bt_node_t* node  = bt_create_node(21);
	int ptr = 0;
	for(int i = 0; i < 21; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[ptr++] = bt_create_entry(i,x);
		node->len++;
	}
	
	free(node->entry[5]->object);
	free(node->entry[5]);
	node->entry[5] = NULL;
	node->len--;
	
	node_shift_left(node, 5,  21);
	
	for(int i = 0; i < 5; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i);
	}
	CU_ASSERT_EQUAL(node->entry[5]->key, 6);
	for(int i = 6; i < 20; i++)
	{
		CU_ASSERT_EQUAL(node->entry[i]->key, i+1);
	}
	
	bt_destroy_node(node, 21, free);
}
 
void node_insert_entry_test(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;
	for(int i = 0; i < 37; i++)
	{
		if(i % 2 == 0)
		{
			int* x = malloc(sizeof(int));
			node->entry[ptr++] = bt_create_entry(i,x);
		}
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry1 = bt_create_entry(31,x);
	node_insert_entry(node, entry1, true, 21);
	
	bool found = false;
	
	for(int i = 0; i < 21; i++)
	{
		if(node->entry[i] == NULL)
		{
			continue;
		}
		if(node->entry[i]->key == 31)
		{
			found = true;
		}
		for(int j = i; j < 21; j++)
		{
			if(node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <= node->entry[j]->key);
			}
		}
		
	}
	
	
	CU_ASSERT(found);
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	
}


void node_insert_entry_test1(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 1; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;
	for(int i = 0; i < 37; i++)
	{
		if(i % 2 == 0)
		{
			int* x = malloc(sizeof(int));
			node->entry[ptr++] = bt_create_entry(i,x);
		}
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry1 = bt_create_entry(0,x);
	node_insert_entry(node, entry1, true, 21);
	
	bool found = false;
	
	for(int i = 0; i < 21; i++)
	{
		if(node->entry[i] == NULL)
		{
			continue;
		}
		if(node->entry[i]->key == 0)
		{
			found = true;
		}
		for(int j = i; j < 21; j++)
		{
			if(node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <= node->entry[j]->key);
			}
		}
		
	}
	
	
	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		if(children_original21[i] != NULL)
		{
			bt_destroy_node(children_original21[i], 21, NULL);
		}
		children_original21[i] = NULL;
	}
	
}

void node_insert_entry_test2(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 1; i < 21; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	int ptr = 0;
	for(int i = 0; i < 37; i++)
	{
		if(i % 2 == 0)
		{
			int* x = malloc(sizeof(int));
			node->entry[ptr++] = bt_create_entry(i,x);
		}
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry1 = bt_create_entry(70,x);
	node_insert_entry(node, entry1, true, 21);
	
	bool found = false;
	
	for(int i = 0; i < 21; i++)
	{
		if(node->entry[i] == NULL)
		{
			continue;
		}
		if(node->entry[i]->key == 70)
		{
			found = true;
		}
		for(int j = i; j < 21; j++)
		{
			if(node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <= node->entry[j]->key);
			}
		}
		
	}
	
	
	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 21; i++)
	{
		if(children_original21[i] != NULL)
		{
			bt_destroy_node(children_original21[i], 21, NULL);
		}
		children_original21[i] = NULL;
	}
	
}

void node_insert_entry_test3(void)
{
	bt_node_t* node  = bt_create_node(21);
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry1 = bt_create_entry(70,x);
	node_insert_entry(node, entry1,true, 21);
	
	bool found = false;
	
	for(int i = 0; i < 21; i++)
	{
		if(node->entry[i] == NULL)
		{
			continue;
		}
		if(node->entry[i]->key == 70)
		{
			found = true;
		}
		for(int j = i; j < 21; j++)
		{
			if(node->entry[j] != NULL)
			{
				CU_ASSERT(node->entry[i]->key <= node->entry[j]->key);
			}
		}
		
	}
	
	
	CU_ASSERT(found);
	CU_ASSERT_PTR_NOT_NULL_FATAL(node);
	bt_destroy_node(node, 21, free);
 	
	
}
void get_next_node_index_test(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 21; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry = bt_create_entry(50,x);
	
	int i = get_next_node_index(node, entry->key, 21);

	CU_ASSERT_EQUAL(i, 21);
	
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}
	free(x);	
	free(entry);
}

void get_next_node_index_test1(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 3; i < 24; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i-3] = bt_create_entry(i,x);
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry = bt_create_entry(1,x);
	
	int i = get_next_node_index(node, entry->key, 21);
	
	CU_ASSERT_EQUAL(i, 0);
	
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 22; i++)
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
	bt_entry_t* entries[10000];
	(void) keys;
	(void) entries;
	srand(time(NULL));   

	for(int i = 0; i < 10000; i++)
	{
		int r = rand();
		r = r % 10000;
		keys[i] = r;
	}
	bubblesort(keys, 10000);
	
	for(int i = 0; i < 10000; i++)
	{
		int* x = malloc(sizeof(int));
		entries[i] = bt_create_entry(keys[i],x);
	}
	for(int i = 0; i < 10000; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_node_search_helper(entries,keys[i], 0, 10000));
	}
	
	for(int i = 0; i < 10000; i++)
	{
		int r = rand();
		r = r % 10000;
		if(bt_node_search_helper(entries,keys[i], 0, 10000) == NULL)
		{
			for(int j = 0; j < 10000; j++)
			{
				CU_ASSERT_NOT_EQUAL(r, keys[i]);
			}
		}
		
	}

	for(int i = 0; i < 10000; i++)
	{
		free(entries[i]->object);
		free(entries[i]);
	}
}

void get_next_node_index_test2(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 20; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 19; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(2 * i,x);
	}
	
	int* x = malloc(sizeof(int));
	bt_entry_t* entry = bt_create_entry(31,x);
	
	int i = get_next_node_index(node, entry->key, 21);
	
	CU_ASSERT_EQUAL(i, 16);
	
	bt_destroy_node(node, 21, free);
 	node = NULL;
 	for(int i = 0; i < 20; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}	
	free(x);	
	free(entry);
}


void split_full_root_test(void)
{
	bt_node_t* node  = bt_create_node(21);
	for(int i = 0; i < 22; i++)
	{
		node->children[i] = bt_create_node(21);
		children_original21[i] = node->children[i];
	}
	for(int i = 0; i < 21; i++)
	{
		int* x = malloc(sizeof(int));
		node->entry[i] = bt_create_entry(i,x);
	}
	
	
	bt_node_t* test_node = split_full_root(node, 21);
		
	CU_ASSERT_NOT_EQUAL(test_node, node);
	CU_ASSERT_EQUAL(test_node->entry[0]->key, 10);
	CU_ASSERT_EQUAL(test_node->children[0], node);
	
	for(int i = 0; i < 10; i++)
	{
		CU_ASSERT_EQUAL(test_node->children[0]->entry[i]->key, i);	
	}
	for(int i = 11; i < 21; i++)
	{
		CU_ASSERT_EQUAL(test_node->children[1]->entry[i - 11]->key, i);	
	}
		
	bt_destroy_node(node, 21, free);
	bt_destroy_node(test_node->children[1], 21, free);
	bt_destroy_node(test_node, 21, free);
	for(int i = 0; i < 22; i++)
	{
		bt_destroy_node(children_original21[i], 21, NULL);
		children_original21[i] = NULL;
	}

}







void bt_delete_helper_test(void)
{
	int n = 10000;
	int keys[n];
	btree_t* bt = bt_create(15);
	srand(time(NULL));   
	
	for(int i = 0; i < n; i++)
	{
		int r = rand() % 10000;
		keys[i] = r;
	}
	
	for(int i = 0; i < n; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}

	for(int i = 0; i < n; i++)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(bt_search(bt, keys[i]));
		bt_delete_helper(bt, bt->root, keys[i]);
		CU_ASSERT_PTR_NULL_FATAL(bt_search(bt, keys[i]));
	}
	bt_destroy(bt, free);
}


void bt_delete_leaf_case_test(void)
{
	
	btree_t* bt = bt_create(3);

	int keys[] = {9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45, 10};

	for(int i = 0; i < 18; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}
	
	for(int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}
	/*bt_node_t* node =  get_node_by_key(bt->root, 6, 3);
	bt_node_t* parent =  get_node_by_key(bt->root, 50, 3);
	
	void* object = bt_delete_leaf_case(bt, parent, node, 0,5);
	free(object);
	CU_ASSERT_PTR_NULL(bt_search(bt, 5));
	
	for(int i = 0; i < 18; i++)
	{
		if(keys[i] != 5)
		{
			CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
		}
	}*/
	
	bt_destroy(bt, free);
}

void bt_delete_leaf_case_test1(void)
{
	
	btree_t* bt = bt_create(3);

	int keys[] = {9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45, 10};

	for(int i = 0; i < 18; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}
	
	for(int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}



	void* object = bt_delete_helper(bt, bt->root, 57);
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
	
	
	

	for(int i = 0; i < 18; i++)
	{
			void* object  = bt_search(bt, keys[i]);
			CU_ASSERT_PTR_NULL(object);
	}
	
	bt_destroy(bt, free);
}


void bt_delete_test(void)
{
	
	btree_t* bt = bt_create(3);

	int keys[] = {9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45, 10};

	for(int i = 0; i < 18; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}
	
	for(int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}

	//bt_node_t* parent =  get_node_by_key(bt->root, 50, 3);
	//bt_node_t* parent2 =  get_node_by_key(bt->root, 80, 3);
	
	//bt_node_t* node =  get_node_by_key(bt->root, 57, 3);
	//bt_node_t* node1 =  get_node_by_key(bt->root, 70, 3);



	void* object = bt_delete_helper(bt, bt->root, 57);
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
	
	
	
	

	for(int i = 0; i < 18; i++)
	{
			void* object  = bt_search(bt, keys[i]);
			CU_ASSERT_PTR_NULL(object);
	}
	
	bt_destroy(bt, free);
}




void merge_leaf_nodes_test(void)
{
	
	btree_t* bt = bt_create(3);

	int keys[] = {9, 8, 100, 50, 60, 70, 80, 5, 6, 57, 71, 73, 72, 85, 200, 300, 45, 10};

	for(int i = 0; i < 18; i++)
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, keys[i], z);
	}
	
	for(int i = 0; i < 18; i++)
	{
		CU_ASSERT_PTR_NOT_NULL(bt_search(bt, keys[i]));
	}


	
	bt_node_t* node1 =  get_node_by_key(bt->root, 57, 3);
	bt_node_t* node2 =  get_node_by_key(bt->root, 70, 3);
	bt_node_t* parent =  get_node_by_key(bt->root, 80, 3);
	parent->children[0] = NULL;
	bt_node_t* ret = merge_leaf_nodes(node1, node2, 3);	
	
	(void) node1;
	(void) node2;
	min(1, 2);
	CU_ASSERT_PTR_NOT_NULL(ret);
	CU_ASSERT_PTR_EQUAL(node1, ret);
	CU_ASSERT_EQUAL(ret->entry[0]->key, 57);
	CU_ASSERT_EQUAL(ret->entry[1]->key, 70);
	CU_ASSERT_PTR_NULL(ret->entry[2]);

	bt_destroy(bt, free);
}


bool treecmp(bt_node_t* root1, bt_node_t* root2, int n)
{
	debug_log(root1, n);
	if((root1 == NULL && root2 != NULL)
		|| (root2 == NULL && root1 != NULL))
	{
		return false;	
	}
	
	if(root1 == NULL)
	{
		return true;
	}
	
	if(!entriescmp(root1, root2, n))
	{
		return false;
	}
	
	for(int i = 0; i < n + 1; i++)
	{
		if(!treecmp(root1->children[i], root2->children[i], n))
		{
			return false;
		}
	}
	return true;
}

bool entriescmp(bt_node_t* root1, bt_node_t* root2, int n)
{
	for(int i = 0; i < n; i++)
	{
		if((root1->entry[i] == NULL && root2->entry[i] != NULL)
		|| (root2->entry[i] == NULL && root1->entry[i] != NULL))
		{

			return false;
		}
		
		if(root1->entry[i] != NULL)
		{
			if(root1->entry[i]->key != root2->entry[i]->key)
			{
				return false;
			}
		}
	}
	return true;
}

static void bubblesort(int keys[], int n)
{

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n - 1; j++)
		{
			if(keys[j] > keys[j+1])
			{
				int tmp = keys[j];
				keys[j] = keys[j+1];
				keys[j+1] = tmp;
			}
		}
	
	}
}

static bt_node_t* get_node_by_key(bt_node_t* root, int key, int n)
{

	if(root == NULL)
	{
		return NULL;
	}

	void* object = bt_node_search_helper(root->entry, key, 0, root->len);
	int i = get_next_node_index(root, key, n);
	return object != NULL ? root : get_node_by_key(root->children[i], key, n);
}
// for debugging
void debug_log(bt_node_t* x, int n)
{
	printf("\n");
	if(x == NULL)
	{
		printf("NULL node\n");
		return;
	}
	for(int i = 0; i < n; i++)
	{
		printf("\t");
	}
	print_node(x, n);
	printf("\n\n");
	for(int i = 0; i < n + 1; i++)
	{
		if(x->children[i] == NULL)
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
	root_global->entry[0] = bt_create_entry(70 ,&tmp);
	
	bt_node_t* int_node1 = bt_create_node(4);
	int_node1->entry[0] = bt_create_entry(8, &tmp);
	int_node1->entry[1] = bt_create_entry(50, &tmp);
	
	bt_node_t* int_node2 = bt_create_node(4);
	int_node2->entry[0] = bt_create_entry(73, &tmp);
	int_node2->entry[1] = bt_create_entry(100, &tmp);
	
	
	bt_node_t* leaf1 = bt_create_node(4);
	leaf1->entry[0] = bt_create_entry(5, &tmp);
	leaf1->entry[1] = bt_create_entry(6, &tmp);
	
	
	bt_node_t* leaf2 = bt_create_node(4);
	leaf2->entry[0] = bt_create_entry(9, &tmp);
	leaf2->entry[1] = bt_create_entry(10, &tmp);
	leaf2->entry[2] = bt_create_entry(45, &tmp);
	
	bt_node_t* leaf3 = bt_create_node(4);
	leaf3->entry[0] = bt_create_entry(57, &tmp);
	leaf3->entry[1] = bt_create_entry(60, &tmp);


	bt_node_t* leaf4 = bt_create_node(4);
	leaf4->entry[0] = bt_create_entry(71, &tmp);
	leaf4->entry[1] = bt_create_entry(72, &tmp);
	
	bt_node_t* leaf5 = bt_create_node(4);
	leaf5->entry[0] = bt_create_entry(80, &tmp);
	leaf5->entry[1] = bt_create_entry(85, &tmp);
	
	bt_node_t* leaf6 = bt_create_node(4);
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
