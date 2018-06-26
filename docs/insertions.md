# Insertions 
This documents goes through the algorithm for insertions used in Cranberry tree and explains the implementation of the insertion operation in a convenient amount of details.

# Algorithm 

**Quick Note:** The description of this algorithm abstracts the low level operations done on a node and its entries and children and rather focuses on the high level implementation of the algorithm.

Important thing to note is that we need to preserve the balance in the tree after each operation and that is we need to preserve the following conditions: 

1. Every branch must end at the same level of every other branch in the tree. In other words, the length of any path, where path is measured in the number of edges traversed, from the root to any of the leaf nodes must be equal to all the other paths. 
2. Every node must have a maximum of n entries and n+1 children.
3. Every intermediate must have a minimum of **ceil(n/2) - 1** entries and **ceil(n/2)** children, with the expection of the root which can have at least 1 entry and 2 children. 
4. Every leaf must have a mininmum of **floor(n/2)** entries and no any children.

The approach to preserve these constraint on every insertion is relatively simple; before traversing always check the child we are going to visit next. if it is full split it into two, otherwise keep traversing remembering to account for the root of course.

Here is a more formal algorithm.

1. If we are at the root and it is full then split and create a new root for the tree with the middle entry of the old root and pointing to two new nodes.
2. If we are at the leaf (which could be also the root), insert the entry in this node.
3. If the next child we are going to traverse needs to be splitted, then split the child into two. 
4. call yourself recursively on the next child.

Note: We distribute the children among the two nodes when we are splitting an intermediate node.


# Code

First let us very quickly go over the interface function: 

`void cbt_insert(cranbtree_t* bt, int key, void* object)`

The function will create a new entry with the key and object and then will call a helper function that will do the insertion for it. After that it will update the cranbtree attributes such as the max key, min key and length.


## Insert Helper: 

All the magic happens in this helper function which can be found in [insert.c](../src/lib/insert.c):

`static void bt_insert_helper(cranbtree_t* bt ,cbt_node_t* root, cbt_entry_t* entry)` 

On high level, the function wil go through all the steps highlighted above in the [Algorithm](#Algorithm) section. However what I want to do in this section is point out the different pieces of code that helps this function do so.

We have our node functions which can be found in [node.c](../src/lib/node.c) which does entry insertions preserving the order of entries and their children as well as gets the next child to be traversed.

Finally, we have two split functions in the same file. The first one handles the split on the root. If the root is full, a new root is created with the middle entry of the old root and assigned to the root attribute of the cranbtree struct. The old root entries is the distributed among the new two roots. The other function splits the child if it is not a leaf and the middle entry is inserted into the parent of this child. 

