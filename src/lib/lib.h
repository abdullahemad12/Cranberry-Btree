/** MIT License
  *
  * Copyright (c) 2018 Abdullah Emad
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  */

/**
  * Prototypes for the static functions
  */
static cbt_node_t* bt_create_node(int n);
static cbt_entry_t* bt_create_entry(int key, void* object);
static void bt_destroy_node(cbt_node_t* node, int n, void (* done)(void*));
static void* bt_destroy_entry(cbt_entry_t* entry, void (* done)(void*));
static int ceil_fn(double n);
static bool is_full_node(cbt_node_t* node, int n);
static void print_node(cbt_node_t* node, int n);
static bool print_level(cbt_node_t* root, int n, int level, int currentLevel);
static int node_insert_entry(cbt_node_t* node, cbt_entry_t* entry, bool shift,  int n);
static cbt_entry_t* bt_split_child(cbt_node_t* node, cbt_node_t** splitted_node, int n);
static int get_next_node_index(cbt_node_t* node, int key, int n);
static void node_shift_right(cbt_node_t* node, int i,  int n);
static void node_shift_left(cbt_node_t* node, int i,  int n);
static void node_shift_left_without_children(cbt_node_t* node, int i,  int n);
static cbt_node_t* split_full_root(cbt_node_t* old_root, int n);
static bool is_leaf(cbt_node_t* node);
static void* bt_node_search_helper(cbt_entry_t* entries[], int key, int min, int max);
static void* bt_search_helper(cbt_node_t* node, int key, int n);
static void bt_insert_helper(cranbtree_t* bt ,cbt_node_t* root, cbt_entry_t* entry);
static void destroy_bt_helper(cbt_node_t* root, int n, void (* done)(void*));
static bool is_root(cranbtree_t* bt, cbt_node_t* node);
static void* bt_delete_entry_helper(cbt_node_t* node, int key, int n);
static int get_last_entry_index(cbt_node_t* node, int n);
static cbt_node_t* merge_leaf_nodes(cbt_node_t* node1, cbt_node_t* node2, int n);
static void node_entry_set_null(cbt_node_t* node, int entry_index, int n);
static void children_shift_right(cbt_node_t* nodes[], int n);
static cbt_node_t* children_shift_left(cbt_node_t* nodes[], int n);
static cbt_entry_t* cpy_entry(cbt_entry_t* entry_original);
static int get_entry_index(cbt_node_t* node, int key);
static void* bt_delete_int_case(cranbtree_t* bt, cbt_node_t* node, int key);
static void fix_pointers_gaps(cbt_node_t* node, int n);
static void node_shift_right_without_children(cbt_node_t* node, int i,  int n);
static void* bt_delete_helper(cranbtree_t* bt, cbt_node_t* parent,  cbt_node_t* node, int key);
static cbt_node_t* get_right_sibling(cbt_node_t* parent, cbt_node_t* node);
static cbt_node_t* get_left_sibling(cbt_node_t* parent, cbt_node_t* node);
static cbt_node_t* merge_nodes(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right);
static void entry_rotate_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n);
static void entry_rotate_counter_clockwise(cbt_node_t* parent, cbt_node_t* left, cbt_node_t* right, int n);
static int get_child_index(cbt_node_t* node, cbt_node_t* child);
static void insert_child(cbt_node_t* parent, cbt_node_t* child, int n);
static void delete_child(cbt_node_t* parent, cbt_node_t* child, int n);
static void balance_node(cranbtree_t* bt, cbt_node_t** parent_ptr, int key);
static void entry_move_up_clockwise(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* right, int key , int n);
static void entry_move_up_counter_clockwise(cranbtree_t* bt, cbt_node_t* parent, cbt_node_t* right, int key , int n);
static cbt_node_t* remove_last_child(cbt_node_t* nodes[], int len);
static cbt_entry_t* bt_delete_minimum(cranbtree_t* bt, cbt_node_t* node);
static cbt_entry_t* bt_delete_maximum(cranbtree_t* bt, cbt_node_t* node);
static int cbt_calculate_min_key(cbt_node_t* root);
static int cbt_calculate_max_key(cbt_node_t* root);

