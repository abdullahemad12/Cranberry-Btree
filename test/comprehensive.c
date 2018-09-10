/**
  * This contains comprehensive tests that tests 
  * All the current functionalities of the BTree
  */
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <time.h>
#include <stdlib.h>

void pickNRandomNumber(int arr[], int n);

/**
  * Tries 32768 different random insertions
  */
static void bt_comp_test_helper(void)
{

	int n = 100;

	static int keys[100];

	memset(keys, 0, n);

	cranbtree_t *bt = cbt_create(3);

	srand(time(NULL));

	pickNRandomNumber(keys, n);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
	}

	for (int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
		void *object = cbt_delete(bt, keys[i]);

		free(object);
		CU_ASSERT_PTR_NULL(cbt_search(bt, keys[i]));
	}
	CU_ASSERT_PTR_NULL(bt->root);
	cbt_destroy(bt, free);
}

void bt_comp_test(void)
{
	printf("\n");
	for (int i = 0; i < 500; i++)
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
	int n = 5000000;

	static int keys[5000000];

	memset(keys, 0, n);

	cranbtree_t *bt = cbt_create(3);

	srand(time(NULL));

	printf("\n");
	pickNRandomNumber(keys, n);

	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, keys[i], z);
		printf("\r\tDone With: %d random Insertions ", i + 1);
	}

	for (int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, keys[i]));
		void *object = cbt_delete(bt, keys[i]);

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

	int n = 100000000;
	cranbtree_t *bt = cbt_create(3);

	printf("\n");
	for (int i = 0; i < n; i++)
	{
		int *z = malloc(sizeof(int));

		cbt_insert(bt, i, z);
		printf("\r\tDone With: %d sequential Insertions ", i + 1);
	}
	for (int i = 0; i < n; i++)
	{

		CU_ASSERT_PTR_NOT_NULL(cbt_search(bt, i));
		void *object = cbt_delete(bt, i);

		free(object);
		CU_ASSERT_PTR_NULL(cbt_search(bt, i));
		printf("\r\tDone With: %d sequential Deletions ", i + 1);
	}
	CU_ASSERT_PTR_NULL(bt->root);
	cbt_destroy(bt, free);
}

/**
  * int[], int -> void
  * EFFECTS: Given an array of length N fills it with Random numbers from 1 to n
  * MODIFIES: arr
  */
void pickNRandomNumber(int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = i;
	}
	srand(time(NULL));
	int r = 0;

	for (int i = 0; i < n; i++)
	{
		/** Removed output here to make visible 
		  * Test: cbt_detach_clone_test2 ...passed
		  * Test: cbt_detach_clone_test3 ...passed
		  */
		/*printf("\r\tPicked: %d random numbers ", i + 1); */
		r = rand() % n;
		int swap = arr[r];

		arr[r] = arr[i];
		arr[i] = swap;
	}

}
