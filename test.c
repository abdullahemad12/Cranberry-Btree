#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "btree.c"
#include "testsuite.c"

/*prototypes*/
static int init(void);
static int run_tests(void);

/*Array of all the tests*/
struct test {
	const char* name;
	void (*func)(void);
};

static struct test tests[] = 
{	
	{"validate_tree_insertions_test", validate_tree_insertions_test},
	//{"comp_test", comp_test},
	{"creation_test", bt_create_test},
	{"creation_test1",bt_create_test1},
	{"node_creation_test",bt_create_node_test},
	{"entry creation test", bt_create_entry_test},
	{"Entry Destroy test", bt_destroy_entry_test},
	{"node Destroy test", bt_destroy_node_test},
	{"node destroy test1", bt_destroy_node_test1},
	{"ceil function test", ceil_test},
	{"is_full_node test", is_full_node_test},
	{"is_full_node test1", is_full_node_test1},
	{"split_child_test", bt_split_child_test},
	{"split_child_test1 even", bt_split_child_test1},
	{"split_child_test2_odd", bt_split_child_test2},
	{"node_shift_right_test", node_shift_right_test},
	{"node shift_right_test1", node_shift_right_test1},
	{"node shift_right_test2", node_shift_right_test2},
	{"node_insert_entry_test", node_insert_entry_test},
	{"node_insert_entry_test1", node_insert_entry_test1},
	{"node_insert_entry_test2", node_insert_entry_test2},
	{"node_insert_entry_test3", node_insert_entry_test3},
	{"get_next_node_index_test", get_next_node_index_test},
	{"get_next_node_index_test1", get_next_node_index_test1},
	{"get_next_node_index_test2", get_next_node_index_test2},
	{"bt_insert_helper_test", bt_insert_helper_test},
	{"bt_insert_helper_test1", bt_insert_helper_test1},
	{"bt_insert_helper_test2", bt_insert_helper_test2},
	{"split_full_root_test", split_full_root_test},
	{"bt_node_search_helper_test", bt_node_search_helper_test},
	//{"bt_delete_helper_test", bt_delete_helper_test},
	{"bt_delete_leaf_case_test", bt_delete_leaf_case_test},
	{"bt_delete_leaf_case_test1", bt_delete_leaf_case_test1},
	{"node_shift_left_test", node_shift_left_test},
	{"merge_leaf_nodes_test", merge_leaf_nodes_test},
	{"get_entry_index_test", get_entry_index_test},
	{"bt_delete_test", bt_delete_test},
	{NULL, NULL}

};

int main(void)
{
	
	int err; 
	/*initialize*/
	err = init();
	if(err != CUE_SUCCESS)
	{
		return err;
	}

		
   /* Run all tests */
	return run_tests();

}

static int init(void)
{
	CU_pSuite pSuite = NULL;
	
   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
   if (NULL == pSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   for(int i = 0; tests[i].name != NULL && tests[i].func != NULL; i++)
	{
		 if ((NULL == CU_add_test(pSuite, tests[i].name, tests[i].func)))
		 {
		 	CU_cleanup_registry();
		 	return CU_get_error();
		 }
	}

   return CUE_SUCCESS;
}


static int run_tests(void)
{
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}


