/**
  * This contains comprehensive tests that tests 
  * All the current functionalities of the BTree
  */
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>

/**
  * Tries 32768 different random insertions
  */
static void bt_comp_test_helper(void)
{
	int n = 100;

	static int keys[100];
	memset(keys, 0, n);
	
	cranbtree_t* bt = cbt_create(3);
	srand(time(NULL)); 
	  
	for(int i = 0; i < n; i++)
	{
		int r = 0;
		while(contains(keys, n, r))
		{
			r = rand() % 10000000;
		}
		keys[i] = r;
	}


	for(int i = 0; i < n; i++)
	{
		int* z = malloc(sizeof(int));
		cbt_insert(bt, keys[i], z);
	}

	for(int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
		void* object = cbt_delete(bt, keys[i]);
		free(object);	
		CU_ASSERT_PTR_NULL(cbt_search(bt, keys[i]));
	}
	CU_ASSERT_PTR_NULL(bt->root);
	cbt_destroy(bt, free);
}


void bt_comp_test(void)
{
	printf("\n");
	for(int i = 0; i < 500; i++)
	{
		printf("\r\tIteration: %d ", i + 1);
		bt_comp_test_helper();
	}
	
}


/**
  * Tries 10 Million different random unique insertions and deletions
  */
static void bt_comp_test1(void)
{
	int n = 500000;

	static int keys[500000];
	memset(keys, 0, n);
	
	cranbtree_t* bt = cbt_create(3);
	srand(time(NULL)); 
	 
	printf("\n"); 
	for(int i = 0; i < n; i++)
	{
		int r = 0;
		while(contains(keys, i, r))
		{
			r = rand() % 1000000000;
		}
		keys[i] = r;
	 	printf("\r\tPicked: %d random numbers ", i + 1);
	}


	
	for(int i = 0; i < n; i++)
	{
		int* z = malloc(sizeof(int));
		cbt_insert(bt, keys[i], z);
		printf("\r\tDone With: %d random Insertions ", i + 1);
	}

	for(int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
		void* object = cbt_delete(bt, keys[i]);
		free(object);	
		CU_ASSERT_PTR_NULL(cbt_search(bt, keys[i]));
		printf("\r\tDone With: %d random Deletions ", i + 1);
	}
	CU_ASSERT_PTR_NULL(bt->root);
	cbt_destroy(bt, free);
}


/**
  * sequential insertion and deletion
  */
static void bt_comp_test2(void)
{
	int n = 10000000;
	cranbtree_t* bt = cbt_create(3);
	
	printf("\n");
	for(int i = 0; i < n; i++)
	{
		int* z = malloc(sizeof(int));
		cbt_insert(bt, i, z);
		printf("\r\tDone With: %d sequential Insertions ", i + 1);
	}
	for(int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, i));
		void* object = cbt_delete(bt, i);
		free(object);	
		CU_ASSERT_PTR_NULL(cbt_search(bt, i));
		printf("\r\tDone With: %d sequential Deletions ", i + 1);
	}
	CU_ASSERT_PTR_NULL(bt->root);
	cbt_destroy(bt, free);
}


