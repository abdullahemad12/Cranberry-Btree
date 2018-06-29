# Deletion
Deletion can be a little bit more complicated than the other operations on the tree becuase we have to make sure that, after removing an element from a certain node in the tree, the tree is still balanced and the number of keys in a node still satisfies the minimum according to the order of the tree. In this document, we will take a look at the algorithm used for the deletion operation and we will take a quick tour around the implementation and source code responsible for making the deletion.

# Algorithm
Let us take a look at the algorithm used for our Btree highlighting that it might be slightly different from other deletion algorithms you might find elsewhere.

## Introduction
As you might expect to retain the tree balanced after removing an entry from the tree we will need to do some rearranging to the tree; however, the algorithm used here chooses to rearrange the entries in the tree before actually removing the entry; that is the algorithm will make sure that all the nodes that exists in the path leading to the node containing our target entry will have more than the allowed minimum number of entries. Doing so, we will still have to account for two main cases of deletion; Deleting from the middle of the tree or deleting from the leaf. Putting everything together, we get that the algorithm has three main pieces

1. Balancing the next node in the path
2. Deleting from an intermediate node.
3. Deleting from a leaf node.

#### Recall
The minimum allowed keys in a node is `ceil(n/2) - 1` for intermediate nodes and `floor(n/2)` for leaf nodes. 

## Balancing nodes

Now we take a look at the algorithm that balances the tree as we go on down the path to the target node. First, we need to make sure that the node actually has a children and that its next child actually needs more entries if we were to delete from it. Assume we have done the necessary checks and it turns out the next child needs more entries. Now we have one of two options:

- borrow an entry from any of the neighbouring nodes (rotate left or right).
- merge with any of the neighbouring entries. 

**definition** target node refers to the anticipated node that the algorithm should visit next when searching for a key.

**Important Note:** We are always aiming to balance the next child we are expecting to travel ignoring that the key might not exist at the tree at all. That is we make sure that the next child we expect to travel contains enough entries in case we ever need to remove an entry from it (think about the root!).

#### Entries rotation
In this step we check to see if the left or the right node contains entries more than the mininmum allowed, if they do we simple perform an operation called rotation in a direction depending on which node contains enough entries, the right or the left. If the left node contains enough entries we perform a clock wise shift. We simple take the entry with the largest key and insert it in the root instead of the parent entry which corresponds to the index of the left child, then insert the entry that was removed from parent in the target node. Similarly if the left node didnot contain enough entries but the right did. However, this time we perform a counter clockwise shift. That is we insert the entry with the smallest key in the right node and put it instead of the parent's entry whose index corresponds to the **target child** index. Of course performing this kind of rotation would require us to also move over the children of the children.

As a quick reference let us take a look at the counter clock wise rotation algorithm: 

1. let i be the index of the target node and i + 1 is the index of the right neighbouring node in the parent's children.
2. Insert the parent entry with index i in the target node (preserving the target node sorted by keys).
3. Insert the first entry of the right child in the parent node in position i.
4. Remove the first entry from the right node (doing any necessary shift or operation to preserve the shape of the node).
5. Move over the first children of the right node to the last children pointer of the target node.

<img src = "Art/rotation.jpg" alt = "rotation">

#### Merging: 
This is the case when both of the neighbouring nodes do not have a enogh entries. In this case we might choose to to merge the target node with one of the neighbouring nodes. The merge is simple: 

steps for merging the target node with the right child: 

1. let i be the index of the target node and i + 1 is the index of the right neighbouring node in the parent's children.
2. insert the entry at position i of the parent in the target node. 
3. create a new node. 
4. copy over all the entries from the target node to the new node.
5. copy over all the entries from the right node to the new node.
6. destroy the right and target nodes. 
7. Adjust the pointers of the parent to point to the new node.




## Deleting from the leaf

Deleting from the parent leaf is now very easy after we have balanced the tree using the algorithm described [above](#Balancing-nodes). When you find the desired entry at the leaf you simply throw it away and return the associated object.


## Deleting from the intermediate node:
Deleting from an intermediate node is slightly complicated as it requires more work to be done. Since children depends on the values of the parents entries, you can not just remove the entry once you find it. What you actually need to do is replace it with another entry. We will refer to this operation as move up clockwise and move up counter clockwise. Before looking at the algorithm, one must note that every entry refers to two children. One that the contains and points to children that contains entries with keys less then the parent's and one with keys greater. Thus and entry points to two subtrees. What we need to do is either replace the entry we want to delete with the maximum of the left tree or the minimum of the right tree.

1. Choose which the subtree with a root that has number of entries greater then the minimum.
2. If the left was chosen delete the maximum of that subtree. 
3. If the right was chosen delete the minimum of that subtree.
4. replace the target entry of the deleted mimimum or maximum.


# Code:

As usual, our interface calls a helper function that does the work for it. The helper function can be found in [delete.c](../src/lib/delete.c) "cbt_delete_helper". The helper function is responsible for searching for the entry to be deleted; as it goes down the tree it calls a function that balances the next node as described in the algorithm balance_node. If the entry was found, the function has to determine if the current node is leaf or intermediate. In case it was a leaf it just removes the entry. otherwise it calls another function that performs the deletion on an intermediate node.

For the operations described above: 

1. Deleting from the intermediate node is handled by bt_delete_int_case()
2. Merge nodes is handled by merge_nodes()
3. balancing is handled by balance_node()
