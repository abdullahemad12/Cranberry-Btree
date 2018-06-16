/**
  * A simple CLI to interact with the binary tree
  *
  * Some of the code here is based on: 
  * https://stackoverflow.com/a/4023921
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "btree.h"


#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

static int getLine (char *prmpt, char *buff, size_t sz);
static int parse_input(char* str);
static void strdcpy(char* src, char* dst);

btree_t* bt = NULL; 
int main(void)
{
	srand(time(NULL));
	bt = bt_create(4);
	while(1)
	{
		char input[50];
		int ret =  getLine (">>", input, 50);
		if(ret == TOO_LONG)
		{
			printf("input was too long. retry:\n");
			continue;
		}
		int i = parse_input(input);
		if(i != 0)
			printf("invalid command. type ? for help\n");	

	}
	bt_destroy(bt, free);
}

static int parse_input(char* str)
{
	char digits_str[strlen(str)];
	strdcpy(str, digits_str);
	int key = atoi(digits_str);
	if(str[0] == 'i')
	{
		int* z = malloc(sizeof(int));
		bt_insert(bt, key ,z);
		return 0;
	}
	else if(str[0] == 'd')
	{
		void* object = bt_delete(bt, key);
		if(object != NULL)
		{
			free(object);
		}
		return 0;
	}
	else if (str[0] == 's')
	{
		void* object = bt_search(bt, key);
		if(object != NULL)
		{
			printf("The key was found\n");
		}	
		else
		{
			printf("Not Found \n");	
		}
		return 0;
	}
	else if (str[0] == 'f')
	{
		for(int i = 0; i < key; i++)
		{
			int r = rand() % (key * 25);
			int* z = malloc(sizeof(int));
			bt_insert(bt, r ,z);
		}
		return 0;
	}
	else if (str[0] == 'p')
	{
		printTree(bt);
	}
	else if(str[0] == '?')
	{
		printf("type an operation character followed by a key number.\ni: insert\nd: delete\ns: search\nf: insert random numbers\n");
		return 0;
	}
	
		return -1;
}

/**
  * copies digits only
  */
static void strdcpy(char* src, char* dst)
{
	int ptr = 0;
	for(int i = 0, n = strlen(src); i < n; i++)
	{
		if(48 <= src[i] && src[i] <= 57)
			dst[ptr++] = src[i];
	}
	dst[ptr] = '\0';
}

/**
  * Author: paxdiablo
  * source: https://stackoverflow.com/a/4023921
  */
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}
