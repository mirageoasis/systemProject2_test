/*
 * echoservert.c - An iterative echo server
 */
/* $begin echoserverimain */

#include "stockserver.h"
#define SBUFSIZE 1024
#define NTHREADS 1024

int lines = 0; /*파일 라인 수(주식의 개수)*/
int loop = 1;  /*루프 지속할지 말지 결정하는 변수*/
sem_t filemutex;
int clientNumber = 0;

typedef struct
{
    int *buf;    /* Buffer array */
    int n;       /* Maximum number of slots */
    int front;   /* buf[(front+1)%n] is first item */
    int rear;    /* buf[rear%n] is last item */
    sem_t mutex; /* Protects accesses to buf */
    sem_t slots; /* Counts available slots */
    sem_t items; /* Counts available items */
} sbuf_t;

sbuf_t sbuf;
void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);

void echo(int connfd);
void command(char *BUF2, char *buf, char *argv[], char *clientBuf);
void sigint_handler();
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough space for any address */ // line:netp:echoserveri:sockaddrstorage
    sigset_t signal_pool;
    pthread_t tid;
    sem_init(&filemutex, 0, 1);
    // int status = 0; // 주식 현재 상태

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    read_file();

    // fprintf(stdout, "read file done!\n");

    // building binary tree
    binary_tree_init(lines); /*이진 트리 빌드하기(qsort로 배열 정렬하고 head 설정)*/

    // fprintf(stdout, "building tree done!\n");
    assert(tree_head != NULL);
    // fprintf(stdout, "head %d %d %d\n", tree_head->ID, tree_head->left_stock, tree_head->price);

    // levelOrder(tree_head, lines); // test code

    listenfd = Open_listenfd(argv[1]);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&signal_pool);
    Sigaddset(&signal_pool, SIGINT);

    sbuf_init(&sbuf, SBUFSIZE);
    for (int i = 0; i < NTHREADS; i++) /* Create worker threads */
        Pthread_create(&tid, NULL, thread, NULL);
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }

    exit(0);
}

void command(char *BUF2, char *buf, char *argv[], char *clientBuf)
{
    int argc = 0;

    memset(clientBuf, 0, MAXLINE); // clinet 에 보낼 buffer 청소하기
    strcpy(BUF2, buf);             // buf2 에 buf내용 복사하기
    BUF2[strlen(BUF2) - 1] = 0;    // 개행문자 없에서 argv 로 분리하기 쉽게 만듬
    argvFeed(BUF2, argv);          // 인자 분리하기

    for (int i = 0; argv[i] != NULL; i++)
        argc++;

    // fprintf(stdout, "argc is %d\n", argc);
    if (!strcmp(argv[0], "show"))
    {
        // lock 걸기
        if (argc != 1)
        {
            sprintf(clientBuf, "USAGE: %s !\n", argv[0]);
            return;
        }

        show_binary_tree(tree_head, clientBuf);
    }
    else if (!strcmp(argv[0], "buy"))
    {
        // lock 걸기
        // fprintf(stdout, "in buy now!\n");

        if (argc != 3)
        {
            sprintf(clientBuf, "USAGE: %s <stock_id> <amount>!\n", argv[0]);
            return;
        }

        int id = atoi(argv[1]);     // 주식 id
        int amount = atoi(argv[2]); // 구매할 주식 양

        STOCK_NODE *target = binary_tree_search(id);

        if (target == NULL)
        {
            sprintf(clientBuf, "stock not found!\n");
        }
        else
        {
            sem_wait(&target->write);
            // fprintf(stdout, "before sem_wait!\n");

            if (amount > target->left_stock)
            {
                //재고보다 구매량이 많은 경우
                sprintf(clientBuf, "Not enough left stocks\n");
            }
            else
            {
                target->left_stock -= amount;
                sprintf(clientBuf, "[buy] " ANSI_COLOR_GREEN "success" ANSI_COLOR_RESET "\n");
                // fprintf(stdout, "changed! %d %d -> %d %d\n", target->ID, target->left_stock + amount, target->ID, target->left_stock);
            }

            sem_post(&target->write);
        }
    }
    else if (!strcmp(argv[0], "sell"))
    {
        // lock 걸기
        // fprintf(stdout, "in sell now!\n");

        if (argc != 3)
        {
            sprintf(clientBuf, "USAGE: %s <stock_id> <amount>!\n", argv[0]);
            return;
        }

        int id = atoi(argv[1]);     // 주식 id
        int amount = atoi(argv[2]); // 구매할 주식 양

        STOCK_NODE *target = binary_tree_search(id);

        if (target == NULL)
        {
            sprintf(clientBuf, "stock not found!\n");
        }
        else
        {
            sem_wait(&target->write);

            target->left_stock += amount;                                                   // 고객이 파는 상황이므로 주식의 개수를 늘린다.
            sprintf(clientBuf, "[sell] " ANSI_COLOR_GREEN "success" ANSI_COLOR_RESET "\n"); //성공 출력메시지
            // fprintf(stdout, "changed! %d %d -> %d %d\n", target->ID, target->left_stock - amount, target->ID, target->left_stock);

            sem_post(&target->write);
        }
    }
    else
    {
        fprintf(stdout, "command does not exists!\n");
        sprintf(clientBuf, "wrong command!\n");
        return;
    }
}

void sigint_handler()
{
    // Sio_puts("in sigint\n");
    loop = 0;
    // Sio_puts("in sigint\n");
    // levelOrder_write_file();
    // 트리정리어캐 할꺼?

    exit(1);
}

void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];       // 원본
    char BUF2[MAXLINE];      // 복사본
    char clientBuf[MAXLINE]; // client 한테 보낼 입력
    char *argv[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        // printf("server received %d bytes\n", n);
        command(BUF2, buf, argv, clientBuf);
        Rio_writen(connfd, clientBuf, MAXLINE);
        // fprintf(stdout, "command: %s", buf);
    }
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1)
    {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buf */
        sem_wait(&filemutex);            // 연결 끝나면 정산 time
        clientNumber++;
        sem_post(&filemutex);

        echo_cnt(connfd); /* Service client */

        sem_wait(&filemutex); // 연결 끝나면 정산 time
        clientNumber--;
        assert(clientNumber >= 0);
        if (clientNumber == 0)
            save_binary_tree(tree_head); // 개수 줄이고 만약에 남아있는 client 개수가 0개면 기록하자!
        sem_post(&filemutex);
        Close(connfd);
    }
}

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;                  /* Buffer holds max of n items */
    sp->front = sp->rear = 0;   /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0); /* Initially, buf has 0 items */
}

void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);                          /* Wait for available slot */
    P(&sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item; /* Insert the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->items);                          /* Announce available item */
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                           /* Wait for available item */
    P(&sp->mutex);                           /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)]; /* Remove the item */
    V(&sp->mutex);                           /* Unlock the buffer */
    V(&sp->slots);                           /* Announce available slot */
    return item;
}