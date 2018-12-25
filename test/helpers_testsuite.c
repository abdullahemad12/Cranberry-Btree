#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>

/*static int navigation_search_helper(void* object)
{
	int* x = (int*) object;
	if()
}
void navigation_search_test1(void)
{
	cranbtree_t* cbt = cbt_create(3);

	
}*/

void get_right_sibling_test(void)
{
	cbt_node_t *parent = bt_create_node(4);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;

	cbt_node_t *child1 = bt_create_node(4);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(4);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;

	cbt_node_t *child3 = bt_create_node(4);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *sibling = get_right_sibling(parent, child1);

	CU_ASSERT_EQUAL(sibling, child2);

	sibling = get_right_sibling(parent, child2);
	CU_ASSERT_EQUAL(sibling, child3);

	sibling = get_right_sibling(parent, child3);
	CU_ASSERT_PTR_NULL(sibling);

	bt_destroy_node(parent, 4, NULL);
	bt_destroy_node(child1, 4, NULL);
	bt_destroy_node(child2, 4, NULL);
	bt_destroy_node(child3, 4, NULL);

}

void get_left_sibling_test(void)
{
	cbt_node_t *parent = bt_create_node(4);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;

	cbt_node_t *child1 = bt_create_node(4);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(4);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;

	cbt_node_t *child3 = bt_create_node(4);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *sibling = get_left_sibling(parent, child1);

	CU_ASSERT_PTR_NULL(sibling);

	sibling = get_left_sibling(parent, child2);
	CU_ASSERT_EQUAL(sibling, child1);

	sibling = get_left_sibling(parent, child3);
	CU_ASSERT_EQUAL(sibling, child2);

	bt_destroy_node(parent, 4, NULL);
	bt_destroy_node(child1, 4, NULL);
	bt_destroy_node(child2, 4, NULL);
	bt_destroy_node(child3, 4, NULL);
}

void merge_nodes_test(void)
{
	cranbtree_t *bt = cbt_create(5);

	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 2;
	parent->children[2] = child3;

	cbt_node_t *new_node = merge_nodes(bt, parent, child1, child2);

	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->children[0]->entry[0]->key, 1);
	CU_ASSERT_EQUAL(new_node->children[0]->entry[1]->key, 5);
	CU_ASSERT_EQUAL(new_node->children[0]->entry[2]->key, 6);
	CU_ASSERT_EQUAL(new_node->children[0]->entry[3]->key, 8);

	CU_ASSERT_EQUAL(new_node->entry[0]->key, 9);
	CU_ASSERT_PTR_NULL(new_node->entry[1]);
	CU_ASSERT_PTR_NULL(new_node->entry[2]);
	CU_ASSERT_PTR_NULL(new_node->entry[3]);

	CU_ASSERT_EQUAL(parent->children[1], child3);
	CU_ASSERT_PTR_NULL(parent->children[2]);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);

	CU_ASSERT_EQUAL(parent->len, 1);

	cbt_destroy(bt, NULL);
}

void merge_nodes_test1(void)
{
	cranbtree_t *bt = cbt_create(5);

	cbt_node_t *parent = bt_create_node(5);
	int *z = malloc(sizeof(int));

	parent->entry[0] = bt_create_entry(5, z);
	parent->entry[1] = bt_create_entry(9, z);
	parent->entry[2] = bt_create_entry(14, z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, z);
	child2->entry[1] = bt_create_entry(8, z);
	child2->len = 2;
	parent->children[1] = child2;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, z);
	child4->entry[1] = bt_create_entry(16, z);
	child4->len = 2;
	parent->children[3] = child4;

	cbt_node_t *new_node = merge_nodes(bt, parent, child2, child3);

	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->children[1]->entry[0]->key, 6);
	CU_ASSERT_EQUAL(new_node->children[1]->entry[1]->key, 8);
	CU_ASSERT_EQUAL(new_node->children[1]->entry[2]->key, 9);
	CU_ASSERT_EQUAL(new_node->children[1]->entry[3]->key, 10);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 14);
	CU_ASSERT_PTR_NULL(parent->entry[2]);
	CU_ASSERT_PTR_NULL(parent->entry[3]);

	CU_ASSERT_EQUAL(parent->children[0], child1);

	CU_ASSERT_EQUAL(parent->children[2], child4);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < bt->root->len + 1; j++)
		{
			CU_ASSERT_PTR_NULL(bt->root->children[j]->children[i]);
		}
	}

	for (int i = bt->root->len + 1; i < 6; i++)
	{
		CU_ASSERT_PTR_NULL(bt->root->children[i]);
	}
	CU_ASSERT_EQUAL(parent->len, 2);

	cbt_destroy(bt, NULL);
	free(z);
}

void merge_nodes_test2(void)
{
	cranbtree_t *bt = cbt_create(5);

	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	cbt_node_t *new_node = merge_nodes(bt, parent, child3, child4);

	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->children[2]->entry[0]->key, 10);
	CU_ASSERT_EQUAL(new_node->children[2]->entry[1]->key, 14);
	CU_ASSERT_EQUAL(new_node->children[2]->entry[2]->key, 15);
	CU_ASSERT_EQUAL(new_node->children[2]->entry[3]->key, 16);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_PTR_NULL(parent->entry[2]);
	CU_ASSERT_PTR_NULL(parent->entry[3]);

	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child2);

	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);

	CU_ASSERT_EQUAL(parent->len, 2);

	cbt_destroy(bt, NULL);
}

void get_child_index_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	int index = get_child_index(parent, child4);

	CU_ASSERT_EQUAL(index, 3);

	index = get_child_index(parent, child1);
	CU_ASSERT_EQUAL(index, 0);

	index = get_child_index(parent, child3);
	CU_ASSERT_EQUAL(index, 2);

	(void)get_child_index;

	cbt_destroy(bt, NULL);
}

void insert_child_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	cbt_node_t *child5 = bt_create_node(5);

	child5->entry[0] = bt_create_entry(11, &z);
	child5->entry[1] = bt_create_entry(12, &z);
	child5->len = 2;

	insert_child(parent, child5, 5);

	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child2);
	CU_ASSERT_EQUAL(parent->children[2], child3);
	CU_ASSERT_EQUAL(parent->children[3], child5);
	CU_ASSERT_EQUAL(parent->children[4], child4);

	cbt_destroy(bt, NULL);

}

void delete_child_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	cbt_node_t *child5 = bt_create_node(5);

	child5->entry[0] = bt_create_entry(11, &z);
	child5->entry[1] = bt_create_entry(12, &z);
	child5->len = 2;

	insert_child(parent, child5, 5);

	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child2);
	CU_ASSERT_EQUAL(parent->children[2], child3);
	CU_ASSERT_EQUAL(parent->children[3], child5);
	CU_ASSERT_EQUAL(parent->children[4], child4);

	delete_child(parent, child2, 5);
	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child3);
	CU_ASSERT_EQUAL(parent->children[2], child5);
	CU_ASSERT_EQUAL(parent->children[3], child4);
	CU_ASSERT_EQUAL(parent->children[4], NULL);

	delete_child(parent, child1, 5);
	CU_ASSERT_EQUAL(parent->children[0], child3);
	CU_ASSERT_EQUAL(parent->children[1], child5);
	CU_ASSERT_EQUAL(parent->children[2], child4);
	CU_ASSERT_EQUAL(parent->children[3], NULL);
	CU_ASSERT_EQUAL(parent->children[4], NULL);

	delete_child(parent, child4, 5);
	CU_ASSERT_EQUAL(parent->children[0], child3);
	CU_ASSERT_EQUAL(parent->children[1], child5);
	CU_ASSERT_EQUAL(parent->children[2], NULL);
	CU_ASSERT_EQUAL(parent->children[3], NULL);
	CU_ASSERT_EQUAL(parent->children[4], NULL);

	cbt_destroy(bt, NULL);

	bt_destroy_node(child2, 5, NULL);
	bt_destroy_node(child1, 5, NULL);
	bt_destroy_node(child4, 5, NULL);
}

void entry_rotate_counter_clockwise_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	entry_rotate_counter_clockwise(parent, child1, child2, 5);

	CU_ASSERT_EQUAL(child1->entry[0]->key, 1);
	CU_ASSERT_EQUAL(child1->entry[1]->key, 5);
	CU_ASSERT_PTR_NULL(child1->entry[2]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 6);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 8);
	CU_ASSERT_PTR_NULL(child2->entry[1]);

	cbt_destroy(bt, NULL);
}

void entry_rotate_counter_clockwise_test1(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	entry_rotate_counter_clockwise(parent, child3, child4, 5);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 10);
	CU_ASSERT_EQUAL(child3->entry[1]->key, 14);
	CU_ASSERT_PTR_NULL(child1->entry[2]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 15);

	CU_ASSERT_EQUAL(child4->entry[0]->key, 16);
	CU_ASSERT_PTR_NULL(child4->entry[1]);

	cbt_destroy(bt, NULL);
}

void entry_rotate_counter_clockwise_test2(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(12, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	entry_rotate_counter_clockwise(parent, child2, child3, 5);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 6);
	CU_ASSERT_EQUAL(child2->entry[1]->key, 9);
	CU_ASSERT_PTR_NULL(child1->entry[2]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 10);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 12);
	CU_ASSERT_PTR_NULL(child3->entry[1]);

	cbt_destroy(bt, NULL);
}

void entry_rotate_clockwise_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->len = 1;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(12, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	entry_rotate_clockwise(parent, child1, child2, 5);

	CU_ASSERT_EQUAL(child1->entry[0]->key, 1);
	CU_ASSERT_PTR_NULL(child1->entry[1]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 4);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 5);
	CU_ASSERT_EQUAL(child2->entry[1]->key, 6);
	CU_ASSERT_PTR_NULL(child2->entry[2]);

	cbt_destroy(bt, NULL);
}

void entry_rotate_clockwise_test1(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;

	entry_rotate_clockwise(parent, child2, child3, 5);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 6);
	CU_ASSERT_PTR_NULL(child2->entry[1]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 7);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 9);
	CU_ASSERT_EQUAL(child3->entry[1]->key, 10);
	CU_ASSERT_PTR_NULL(child3->entry[2]);

	cbt_destroy(bt, NULL);
}

void entry_rotate_clockwise_test2(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(12, &z);
	child3->len = 2;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->len = 1;
	parent->children[3] = child4;

	entry_rotate_clockwise(parent, child3, child4, 5);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 10);
	CU_ASSERT_PTR_NULL(child3->entry[1]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 12);

	CU_ASSERT_EQUAL(child4->entry[0]->key, 14);
	CU_ASSERT_EQUAL(child4->entry[1]->key, 15);
	CU_ASSERT_PTR_NULL(child3->entry[2]);

	cbt_destroy(bt, NULL);
}

void entry_move_up_clockwise_test(void)
{

	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->entry[2] = bt_create_entry(4, &z);
	child1->len = 3;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(12, &z);
	child3->len = 2;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->len = 1;
	parent->children[3] = child4;

	entry_move_up_clockwise(bt, parent, child1, 5, 5);

	CU_ASSERT_EQUAL(child1->entry[0]->key, 1);
	CU_ASSERT_EQUAL(child1->entry[1]->key, 3);
	CU_ASSERT_PTR_NULL(child1->entry[2]);
	CU_ASSERT_PTR_NULL(child1->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 4);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	cbt_destroy(bt, NULL);
}

void entry_move_up_clockwise_test1(void)
{

	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->entry[2] = bt_create_entry(4, &z);
	child1->len = 3;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->entry[2] = bt_create_entry(8, &z);
	child2->len = 3;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(12, &z);
	child3->len = 2;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->len = 1;
	parent->children[3] = child4;

	entry_move_up_clockwise(bt, parent, child2, 9, 5);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 6);
	CU_ASSERT_EQUAL(child2->entry[1]->key, 7);
	CU_ASSERT_PTR_NULL(child2->entry[2]);
	CU_ASSERT_PTR_NULL(child2->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 8);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	cbt_destroy(bt, NULL);
}

void entry_move_up_clockwise_test2(void)
{

	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->entry[2] = bt_create_entry(12, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->entry[2] = bt_create_entry(17, &z);
	child4->len = 3;
	parent->children[3] = child4;

	entry_move_up_clockwise(bt, parent, child3, 14, 5);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 10);
	CU_ASSERT_EQUAL(child3->entry[1]->key, 11);
	CU_ASSERT_PTR_NULL(child3->entry[2]);
	CU_ASSERT_PTR_NULL(child3->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 12);

	cbt_destroy(bt, NULL);
}

void entry_move_up_counter_clockwise_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->entry[2] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->entry[2] = bt_create_entry(12, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->len = 1;
	parent->children[3] = child4;

	entry_move_up_counter_clockwise(bt, parent, child2, 5, 5);

	CU_ASSERT_EQUAL(child2->entry[0]->key, 7);
	CU_ASSERT_EQUAL(child2->entry[1]->key, 8);
	CU_ASSERT_PTR_NULL(child2->entry[2]);
	CU_ASSERT_PTR_NULL(child2->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 6);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	cbt_destroy(bt, NULL);

}

void entry_move_up_counter_clockwise_test1(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->entry[2] = bt_create_entry(12, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->len = 1;
	parent->children[3] = child4;

	entry_move_up_counter_clockwise(bt, parent, child3, 9, 5);

	CU_ASSERT_EQUAL(child3->entry[0]->key, 11);
	CU_ASSERT_EQUAL(child3->entry[1]->key, 12);
	CU_ASSERT_PTR_NULL(child3->entry[2]);
	CU_ASSERT_PTR_NULL(child3->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 10);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 14);

	cbt_destroy(bt, NULL);

}

void entry_move_up_counter_clockwise_test2(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->entry[2] = bt_create_entry(12, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->entry[2] = bt_create_entry(17, &z);
	child4->len = 3;
	parent->children[3] = child4;

	entry_move_up_counter_clockwise(bt, parent, child4, 14, 5);

	CU_ASSERT_EQUAL(child4->entry[0]->key, 16);
	CU_ASSERT_EQUAL(child4->entry[1]->key, 17);
	CU_ASSERT_PTR_NULL(child4->entry[2]);
	CU_ASSERT_PTR_NULL(child4->entry[3]);

	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_EQUAL(parent->entry[2]->key, 15);

	cbt_destroy(bt, NULL);

}

static void children_shift_left_test(void)
{
	cranbtree_t *bt = cbt_create(5);
	cbt_node_t *parent = bt_create_node(5);
	int z = 0;

	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;

	cbt_node_t *child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(4, &z);
	child1->len = 2;
	parent->children[0] = child1;

	cbt_node_t *child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(7, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;

	cbt_node_t *child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->entry[2] = bt_create_entry(12, &z);
	child3->len = 3;
	parent->children[2] = child3;

	cbt_node_t *child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->entry[2] = bt_create_entry(17, &z);
	child4->len = 3;
	parent->children[3] = child4;

	cbt_node_t *removed_node = children_shift_left(parent->children, 5);

	CU_ASSERT_EQUAL(removed_node, child1);
	CU_ASSERT_EQUAL(parent->children[0], child2);
	CU_ASSERT_EQUAL(parent->children[1], child3);
	CU_ASSERT_EQUAL(parent->children[2], child4);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);
	bt_destroy_node(removed_node, 5, NULL);
	cbt_destroy(bt, NULL);
}

void bt_delete_minimum_test(void)
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

	cbt_entry_t *entry = bt_delete_minimum(bt, bt->root->children[1]);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 70);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 70)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_minimum_test1(void)
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

	cbt_entry_t *entry = bt_delete_minimum(bt, bt->root->children[0]);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 5);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 5)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_minimum_test2(void)
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

	cbt_entry_t *entry =
	    bt_delete_minimum(bt, bt->root->children[1]->children[1]);
	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 72);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 72)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_minimum_test3(void)
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

	cbt_entry_t *entry = bt_delete_minimum(bt, bt->root);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 5);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 5)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_maximum_test(void)
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

	cbt_entry_t *entry = bt_delete_maximum(bt, bt->root->children[1]);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 300);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 300)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}
	bt_destroy_entry(entry, free);

	cbt_destroy(bt, free);
}

void bt_delete_maximum_test1(void)
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

	cbt_entry_t *entry = bt_delete_maximum(bt, bt->root->children[0]);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 57);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 57)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}
	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_maximum_test2(void)
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

	cbt_entry_t *entry =
	    bt_delete_maximum(bt, bt->root->children[1]->children[1]);
	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 73);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 73)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}

void bt_delete_maximum_test3(void)
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

	cbt_entry_t *entry = bt_delete_maximum(bt, bt->root);

	CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
	CU_ASSERT_EQUAL(entry->key, 300);

	for (int i = 0; i < 18; i++)
	{
		void *object = cbt_search(bt, keys[i]);

		if (keys[i] == 300)
		{
			CU_ASSERT_PTR_NULL(object);
		}
		else
		{
			CU_ASSERT_PTR_NOT_NULL(object);
		}
	}

	bt_destroy_entry(entry, free);
	cbt_destroy(bt, free);
}
