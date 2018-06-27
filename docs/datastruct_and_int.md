# Data structure and Interface 
This documents goes through the structs used to represent the binary tree and explains the code structure pattern used as well as the definition of interfaces.

# Structs 
The structs used to represent the B-tree can be classified into three:

1. **struct cbt_entry**
2. **struct cbt_node**
3. **struct cranbtree**

## cranbtree: 
This struct represents The B-tree. It is what the user will pass into any of the interface functions to perform a certain operation on the Btree. It contains several attributes:

1. [struct cbt_node](#cbt-node) root: this represents the root node of the tree which is used to traverse the whole tree. Initially this is NULL and is set to NULL whenever the tree is empty. 
2. unsigned int length: this is the length of the tree, i.e the count of the [struct cbt_entry](#cbt-entry) in tree. This is updated on every succussfull deletion and insertions.
3. int max_key: this is the Maximum key in the tree. It's used to speed up answering questions about the maximum key. It is updated on every succussfull deletion and insertions. On Insertions, the max key is compared to the newly inserted key, if it is larger, it is replaced. Otherwise nothing is done. On deletion, however, if the key that was successfully deleted happens to be the maximum key. The tree is traversed from root to the most right node and the right most element is chosen to replace the old max_key.
4. unsigned int n: by convention of several books, this is named n. n is defined to be the B-tree order, or the maximum number of keys a node can have.

#### The creation and deletion of a B-tree: 
Two interface functions are responsible for deletions and insertions into B-trees. cbt_create() and cbt_destroy(). 
- Creation is simple, it just takes the order of the tree, n, as an argument, allocates memory for the cranbtree struct, sets all the fields appropriately and returns a pointer to that cranbtree. 

- However, destroying requires more work to be done, since the B-tree might not be empty by the time it's destroyed. First, the root is passed on to a helper function which traverses the tree recursively. Destroys each of the children of a [node](#cbt-node), and all the [entries](#cbt-entry) in the node. However, there is a catch here, since the a non empty entry will contain a pointer to an object, all these pointers will be lost, which might result in memory leaks overall. In this case, the user can choose between several options: 
	1. Do nothing these objects and handle them on their own.
	2. Delete the objects one by one independtly and destroy the objects on their own. 
	3. pass the pointer to their own custom destroy function, which will get called on every object when the entry is destroyed.


## cbt node: 
This struct represents a single node in the Btree and has the following attributes: 

1. cbt_entry_t** entry: array of [entries](#cbt-entry) that belongs to this node. An important invariant is that entries **MUST** be sorted by their key in incearsing order.
2. struct cbt_node** children: array of nodes. These are the children of this node sorted according to the keys of the entries. For example, a child in the first position in the array must have entries with smaller keys then the key of the first entry and so on. An important invariant is that children **MUST** always be one greater than the number of entries. that is len + 1
3. int len: this is total number of entries in the node. constraints: 
	- For root: 0 < len  <= n
	- For intermediate nodes: ceil(n / 2) - 1 < len <= n 
	- For leaf nodes: floor(n/2) < len <= n

#### Operations on nodes: 
All the operations on nodes can be found in [node.c](../src/lib/node.c). It includes but not limited to: 
1. Shifting the array of entries to the right and left with their children or without them. 
2. Shifting the array of children to the right and left.
3. Inserting a new entry in the node while keeping all the entries sorted and handling the children. 
4. Deleting an entry from the node also while keeping the entries sorted.
5. Merging two nodes into one node when the sum of their entries adds up to n. 
6. Splitting full nodes into two nodes. 


## cbt entry: 
This struct represents and entry in a node. It has the following attributes: 
1. int key: the key of the entry. 
2. void* object: a pointer to the object to be stored by the user.



## Tree
<img src = "Art/tree.png">