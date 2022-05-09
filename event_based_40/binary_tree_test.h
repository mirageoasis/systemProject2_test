#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void Enqueue_test(STOCK_NODE *ptr, STOCK_NODE **Queue);
STOCK_NODE *Dequeue_test(STOCK_NODE **Queue);
void levelOrder_test(STOCK_NODE *ptr, int lines);