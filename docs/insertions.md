# Insertions 
This documents goes through the algorithm for insertions used in Cranberry tree and explains the implementation of the insertion operation in a convenient amount of details.

# Algroithm 

**Quick Note:** The description of this algorithm abstracts the low level operations done on a node and its entries and children and rather focuses on the high level implementation of the algorithm.

Important thing to note is that we need to preserve the balance in the tree after each operation and that is we need to preserve the following conditions: 

1. Every branch must end at the same level of every other branch in the tree. In other words, the length of any path, where path is measured in the number of edges traversed, from the root to any of the leaf nodes must be equal to all the other paths. 
2. Every node must have a maximum of n entries and n+1 children.
3. Every intermediate must have a minimum of **ceil(n/2) - 1** entries and **ceil(n/2)** children, with the expection of the root which can have at least 1 entry and 2 children. 
4. Every 
