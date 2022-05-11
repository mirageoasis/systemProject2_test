#include "binary_tree_test.h"

int front_test = 0; // 큐 앞
int rear_test = 0;  // 큐 뒤

void Enqueue_test(STOCK_NODE *ptr, STOCK_NODE **Queue)
{
    // fprintf(stdout, "enqueue rear %d!\n", rear + 1);
    Queue[++rear_test] = ptr;
}

STOCK_NODE *Dequeue_test(STOCK_NODE **Queue)
{
    // fprintf(stdout, "dequeue! front %d\n", front + 1);
    return Queue[++front_test];
}

void levelOrder_test(STOCK_NODE *ptr, int lines)
{
    if (!ptr)
        return;

    STOCK_NODE **Queue = (STOCK_NODE **)malloc(sizeof(STOCK_NODE *) * (lines + 10));

    for (int i = 0; i < lines + 10; i++)
        Queue[i] = 0;

    // STOCK_NODE *Queue[lines]; // 이거 숫자 고치기

    Enqueue_test(ptr, Queue);

    while (1)
    {
        ptr = Dequeue_test(Queue);
        // fprintf(stdout, "front: %d rear: %d\n", front, rear);
        if (ptr)
        {
            assert(ptr != NULL);
            // fprintf(stdout, "%d\n", ptr->ID);

            if (ptr->left)
                Enqueue_test(ptr->left, Queue);
            if (ptr->right)
                Enqueue_test(ptr->right, Queue);
        }
        else
            break;
    }
    // fprintf(stdout, "queue finished\n");
    free(Queue);
}
