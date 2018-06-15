/*
 * Cranberry OS
 * Balanced Search Tree Data structure.
 *
 *  Copyright (c) 2018
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/*
 * Struct for the B-tree node's entries;
 * All the nodes in the left subtree must have value >= this.key
 * All the nodes in the right subtree must have value < this.key
 *
 * Keys for the B-tree are assumed to be positive, otherwise the entry is treated as invalid
 */
typedef struct bt_entry
{
	int key;
	void* object;
}bt_entry_t;

/*
 * Struct for the B+ Tree nodes 
 * The maximum Number of entries n is determined by the users when creating the tree
 * A B+ Tree with n entries can have n+1 pointers
 */

typedef struct bt_node
{
	bt_entry_t** entry;
	struct bt_node** children;
	int len;
}bt_node_t;



/*
 * struct for the Btree
 * 
 * bpt_node_t* root: pointer to the root of the tree
 * unsigned int length: number of nodes in the btree
 * int max_key: the maximum key in the BTree
 * unsinged int n: the maximum number of entries in a node
 */

typedef struct btree
{
	bt_node_t* root;
	unsigned int length;
	int max_key;
	unsigned int n;
}btree_t;
