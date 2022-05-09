#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

extern STOCK_NODE *temp_array;
extern STOCK_NODE *tree_head;

int cmp(const void *first, const void *second);
void binary_tree_init();
void show_binary_tree(STOCK_NODE *cur, char *clientBuf);
STOCK_NODE *binary_tree_build(int start, int end);
STOCK_NODE *binary_tree_search(int id);
