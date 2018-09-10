# Search
This document discusses the search operation of the cranbtree and briefly highlights the algorithm used.

# Algorithm 

First let us quickly review the structure of cranbtree before diving into the algorithm. A cranbtree of order m is made up of nodes with at most m-1 entries and at most m children that are also nodes. Let us zoom in in one node and see how it is structured and assume that the node we are looking at contains n entries ordered ascendingly by key and m children. Now, the children are ordered in the following way: a child at position i will point to a subtree of which all of its keys are smaller then the key of the entry of position i and greater than or equal the key of the entry at position i + 1, taking care of course of the edge cases (the first and last entry). Now as in search tree, when searching we have the ability to choose which path to go to next according to the value of the key we are searching for. 

1. if the node is NULL return NULL
2. Search for the key in the current node's entries.
3. if the key is found return the object.
4. Get the next node according to the value of the key.
5. repeat steps 1-5 with the next node


# Code structure

The interface function cbt_search() found in [cranbtree.c](../src/cranbtree.c) uses a helper function (found in [search.c](../src/lib/search.c)) that takes the root of the tree as an argument and does the search for it. The function performs the steps described in the algorithm above recursively.

