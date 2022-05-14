#include "binary_tree.h"

int front = 0; // 큐 앞
int rear = 0;  // 큐 뒤

int cmp(const void *first, const void *second)
{
    return ((STOCK_NODE *)first)->ID > ((STOCK_NODE *)second)->ID;
}

STOCK_NODE *binary_tree_build(int start, int end)
{
    if (start > end)
        return NULL;
    STOCK_NODE *pnew = (STOCK_NODE *)malloc(sizeof(STOCK_NODE));

    int mid = (start + end) / 2;
    // fprintf(stdout, "the id is %d!\n", temp_array[mid].ID);
    pnew->ID = temp_array[mid].ID;                 //주식 id
    pnew->left_stock = temp_array[mid].left_stock; // 재고
    pnew->price = temp_array[mid].price;           // 가격
    pnew->readcnt = 0;                             // reader 는 0명

    // sem_init(&pnew->mutex, 0, 1); // 세마포어 1로 초기화(자신이 들어가고 lock이 걸려야 하므로 0 이면 signal 의 개념이다)
    // sem_init(&pnew->write, 0, 1); // 세마포어 1로 초기화(자신이 들어가고 lock이 걸려야 하므로 0 이면 signal 의 개념이다)

    pnew->left = binary_tree_build(start, mid - 1); // 왼쪽
    pnew->right = binary_tree_build(mid + 1, end);  // 오른쪽

    return pnew;
}

void binary_tree_init(int lines)
{
    qsort(temp_array, lines, sizeof(STOCK_NODE), cmp);

    for (int i = 0; i < lines; i++)
    {
        // fprintf(stdout, "sorted %d %d %d\n", temp_array[i].ID, temp_array[i].left_stock, temp_array[i].price);
    }

    tree_head = binary_tree_build(0, lines - 1);
    free(temp_array); // 코드 스타일 물어보기 free 를 여기서 해주는게 맞는건가?
}

STOCK_NODE *binary_tree_search(int id)
{
    STOCK_NODE *ret = tree_head;

    while (ret)
    {
        if (id > ret->ID)
        {
            ret = ret->right;
        }
        else if (id < ret->ID)
        {
            ret = ret->left;
        }
        else
        {
            // fprintf(stdout, "%d found!\n", ret->ID);
            break; // 같은 경우는 break한다
        }
    }

    return ret;
}

void show_binary_tree(STOCK_NODE *cur, char *clientBuf)
{
    // 접근 금지
    // wait 하고 post
    if (!cur) // 없으면 return
        return;
    // sem_wait(&cur->mutex);
    //(cur->readcnt)++;
    // if (cur->readcnt == 1)
    //     sem_wait(&cur->write);
    // sem_post(&cur->mutex);

    show_binary_tree(cur->left, clientBuf);
    // sprintf(clientBuf, "show binary tree: ");
    sprintf(clientBuf + strlen(clientBuf), "%d %d %d\n", cur->ID, cur->left_stock, cur->price); // 구현에 따라 다르다
    // fprintf(stdout, "%s\n", clientBuf);
    show_binary_tree(cur->right, clientBuf);

    //접근 금지

    // sem_wait(&cur->mutex);
    //(cur->readcnt)--;
    // if (cur->readcnt == 0)
    //     sem_post(&cur->write);

    // sem_post(&cur->mutex);
}
