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
#include <unistd.h>
#include <btree.h>


#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

/*prototypes*/
static int getLine (char *prmpt, char *buff, size_t sz);
static int parse_input(char* str);
static void strdcpy(char* src, char* dst);
static void load_from_keys_dot_txt(void);
static void delete_slow_motion(int sec);
static void populate_with_random(int n);


/*global variables*/

const char* help = "type an operation character followed by a key number.\ni: insert\nd: delete\ns: search\nf: insert random numbers\np: Print the current Tree\n";

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
	switch(str[0])
	{
		case 'i':
		{ 
			int* z = malloc(sizeof(int));
			bt_insert(bt, key ,z);
			return 0;
		}
		case 'd':
		{
			void* object = bt_delete(bt, key);
			if(object != NULL)
				free(object);
			return 0;
		}
		case 's':
		{
			void* object = bt_search(bt, key);
			if(object != NULL)
				printf("The key was found\n");
			else
				printf("Not Found \n");	
			return 0;
		}
		case 'f': 
			populate_with_random(key);
			return 0;
		case 'L':
			load_from_keys_dot_txt();
			return 0;
		case 'x':
			/*keys is the number of seconds*/
			delete_slow_motion(key);
			return 0;
		case 'p':
			printTree(bt);
			return 0;
		case '?': 
			printf("%s", help);
			return 0;
	}
	
	return -1;
}

static void populate_with_random(int n)
{
	for(int i = 0; i < n; i++)
	{
		int r = rand() % (n * 25);
		int* z = malloc(sizeof(int));
		bt_insert(bt, r ,z);
	}
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
  * Inserts the keys in keys.txt to the BTree
  *
  */
static void load_from_keys_dot_txt(void)
{
	FILE* file = fopen("keys.txt", "r");
	char buffer[100];
	while(fgets(buffer, 100, file) != NULL)
	{
		int key = atoi(buffer);
		int* z = malloc(sizeof(int));	
		bt_insert(bt, key ,z);
	}
	fclose(file);
} 


/**
  * Deletes the keys in keys.txt from the BTree
  *
  */
static void delete_slow_motion(int sec)
{
	FILE* file = fopen("keys.txt", "r");
	char buffer[100];
	while(fgets(buffer, 100, file) != NULL)
	{
		int key = atoi(buffer);
		free(bt_delete(bt, key));
		printf("key deleted: %d\n\n", key);
		printTree(bt);
		sleep(sec);
	}
	fclose(file);
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
