#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>


void get_right_sibling_test(void)
{
	bt_node_t* parent = bt_create_node(4);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;
	
	bt_node_t* child1 = bt_create_node(4);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->len = 2;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(4);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	
	
	bt_node_t* child3 = bt_create_node(4);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 3;
	parent->children[2] = child3;
	
	
	bt_node_t* sibling =  get_right_sibling(parent, child1);
	CU_ASSERT_EQUAL(sibling, child2);
	
	sibling = get_right_sibling(parent, child2);
	CU_ASSERT_EQUAL(sibling, child3);
	
	sibling = get_right_sibling(parent, child3);
	CU_ASSERT_PTR_NULL(sibling);
	
	
	bt_destroy_node(parent, 4, NULL);
}

void get_left_sibling_test(void)
{
	bt_node_t* parent = bt_create_node(4);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;
	
	bt_node_t* child1 = bt_create_node(4);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->entry[1] = bt_create_entry(3, &z);
	child1->len = 2;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(4);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	
	
	bt_node_t* child3 = bt_create_node(4);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 3;
	parent->children[2] = child3;
	
	
	bt_node_t* sibling = get_left_sibling(parent, child1);
	CU_ASSERT_PTR_NULL(sibling);
	
	sibling = get_left_sibling(parent, child2);
	CU_ASSERT_EQUAL(sibling, child1);
	
	sibling = get_left_sibling(parent, child3);
	CU_ASSERT_EQUAL(sibling, child2);
	
	
	bt_destroy_node(parent, 4, NULL);
}


void merge_nodes_test(void)
{
	btree_t* bt = bt_create(5);
	
	
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->len = 2;
	
	bt_node_t* child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->entry[1] = bt_create_entry(11, &z);
	child3->len = 2;
	parent->children[2] = child3;
	
	bt_node_t* new_node = merge_nodes(bt, parent, child1, child2);
	
	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->entry[0]->key, 1);
	CU_ASSERT_EQUAL(new_node->entry[1]->key, 5);
	CU_ASSERT_EQUAL(new_node->entry[2]->key, 6);
	CU_ASSERT_EQUAL(new_node->entry[3]->key, 8);
	
	CU_ASSERT_EQUAL(parent->entry[0]->key, 9);
	CU_ASSERT_PTR_NULL(parent->entry[1]);
	CU_ASSERT_PTR_NULL(parent->entry[2]);
	CU_ASSERT_PTR_NULL(parent->entry[3]);
	
	CU_ASSERT_EQUAL(parent->children[0], new_node);
	CU_ASSERT_EQUAL(parent->children[1], child3);
	CU_ASSERT_PTR_NULL(parent->children[2]);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);
	
	CU_ASSERT_EQUAL(parent->len, 1);
	CU_ASSERT_EQUAL(new_node->len, 4);
	
	bt_destroy(bt, NULL);
}


void merge_nodes_test1(void)
{
	btree_t* bt = bt_create(5);
	
	
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	
	bt_node_t* child1 = bt_create_node(4);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;
	
	
	bt_node_t* child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;
	
	
	
	bt_node_t* new_node = merge_nodes(bt, parent, child2, child3);
	
	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->entry[0]->key, 6);
	CU_ASSERT_EQUAL(new_node->entry[1]->key, 8);
	CU_ASSERT_EQUAL(new_node->entry[2]->key, 9);
	CU_ASSERT_EQUAL(new_node->entry[3]->key, 10);
	
	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 14);
	CU_ASSERT_PTR_NULL(parent->entry[2]);
	CU_ASSERT_PTR_NULL(parent->entry[3]);
	
	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], new_node);
	CU_ASSERT_EQUAL(parent->children[2], child4);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);
	
	
	CU_ASSERT_EQUAL(parent->len, 2);
	CU_ASSERT_EQUAL(new_node->len, 4);
	
	bt_destroy(bt, NULL);
}


void merge_nodes_test2(void)
{
	btree_t* bt = bt_create(5);
	
	
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	
	bt_node_t* child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;
	
	
	bt_node_t* child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;
	
	
	
	bt_node_t* new_node = merge_nodes(bt, parent, child3, child4);
	
	CU_ASSERT_PTR_NOT_NULL_FATAL(new_node);
	CU_ASSERT_EQUAL(new_node->entry[0]->key, 10);
	CU_ASSERT_EQUAL(new_node->entry[1]->key, 14);
	CU_ASSERT_EQUAL(new_node->entry[2]->key, 15);
	CU_ASSERT_EQUAL(new_node->entry[3]->key, 16);
	
	CU_ASSERT_EQUAL(parent->entry[0]->key, 5);
	CU_ASSERT_EQUAL(parent->entry[1]->key, 9);
	CU_ASSERT_PTR_NULL(parent->entry[2]);
	CU_ASSERT_PTR_NULL(parent->entry[3]);
	
	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child2);
	CU_ASSERT_EQUAL(parent->children[2], new_node);
	CU_ASSERT_PTR_NULL(parent->children[3]);
	CU_ASSERT_PTR_NULL(parent->children[4]);
	
	CU_ASSERT_EQUAL(parent->len, 2);
	CU_ASSERT_EQUAL(new_node->len, 4);
	
	bt_destroy(bt, NULL);
}

void get_child_index_test(void)
{
	btree_t* bt = bt_create(5);
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;
	
	bt_node_t* child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;
	
	
	bt_node_t* child4 = bt_create_node(5);

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
	
	(void) get_child_index;

	
	bt_destroy(bt, NULL);
}

void insert_child_test(void)
{
	btree_t* bt = bt_create(5);
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;
	
	bt_node_t* child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;
	
	
	bt_node_t* child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;
	
	
	
	bt_node_t* child5 = bt_create_node(5);

	child5->entry[0] = bt_create_entry(11, &z);
	child5->entry[1] = bt_create_entry(12, &z);
	child5->len = 2;
	
	insert_child(parent, child5, 5);
	
	
	CU_ASSERT_EQUAL(parent->children[0], child1);
	CU_ASSERT_EQUAL(parent->children[1], child2);
	CU_ASSERT_EQUAL(parent->children[2], child3);
	CU_ASSERT_EQUAL(parent->children[3], child5);
	CU_ASSERT_EQUAL(parent->children[4], child4);
	
	bt_destroy(bt, NULL);

}


void delete_child_test(void)
{
	btree_t* bt = bt_create(5);
	bt_node_t* parent = bt_create_node(5);
	int z = 0;
	parent->entry[0] = bt_create_entry(5, &z);
	parent->entry[1] = bt_create_entry(9, &z);
	parent->entry[2] = bt_create_entry(14, &z);
	parent->len = 3;
	bt->root = parent;
	
	bt_node_t* child1 = bt_create_node(5);

	child1->entry[0] = bt_create_entry(1, &z);
	child1->len = 1;
	parent->children[0] = child1;
	
	bt_node_t* child2 = bt_create_node(5);

	child2->entry[0] = bt_create_entry(6, &z);
	child2->entry[1] = bt_create_entry(8, &z);
	child2->len = 2;
	parent->children[1] = child2;
	bt->root = parent;
	
	bt_node_t* child3 = bt_create_node(5);

	child3->entry[0] = bt_create_entry(10, &z);
	child3->len = 1;
	parent->children[2] = child3;
	
	
	bt_node_t* child4 = bt_create_node(5);

	child4->entry[0] = bt_create_entry(15, &z);
	child4->entry[1] = bt_create_entry(16, &z);
	child4->len = 2;
	parent->children[3] = child4;
	
	
	
	bt_node_t* child5 = bt_create_node(5);

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
	
	bt_destroy(bt, NULL);
}
