/*
 * echoservert.c - An iterative echo server
 */
/* $begin echoserverimain */

#include "stockserver.h"

typedef struct
{ /* Represents a pool of connected descriptors */ // line:conc:echoservers:beginpool
    int maxfd;                                     /* Largest descriptor in read_set */
    fd_set read_set;                               /* Set of all active descriptors */
    fd_set ready_set;                              /* Subset of descriptors ready for reading  */
    int nready;                                    /* Number of ready descriptors from select */
    int maxi;                                      /* Highwater index into client array */
    int clientfd[FD_SETSIZE];                      /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE];                   /* Set of active read buffers */
} pool;                                            // line:conc:echoservers:endpool

int lines = 0; /*파일 라인 수(주식의 개수)*/
int loop = 1;  /*루프 지속할지 말지 결정하는 변수*/
sem_t readMutex;

void echo(int connfd);
void add_client(int connfd, pool *p);
int check_clients(pool *p);
void command(char *BUF2, char *buf, char *argv[], char *clientBuf);
void sigint_handler();
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough space for any address */ // line:netp:echoserveri:sockaddrstorage
    sigset_t signal_pool;
    pthread_t tid;
    // char client_hostname[MAXLINE], client_port[MAXLINE];
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

    while (loop)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfdp = Malloc(sizeof(int));                              // line:conc:echoservert:beginmalloc
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen); // line:conc:echoservert:endmalloc
        Pthread_create(&tid, NULL, thread, connfdp);
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
            }
            // fprintf(stdout, "before sem_post!\n");
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

void *thread(void *vargp)
{
    int n;
    char buf[MAXLINE];       // 원본
    char BUF2[MAXLINE];      // 복사본
    char clientBuf[MAXLINE]; // client 한테 보낼 입력
    char *argv[MAXLINE];
    rio_t rio;

    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); // line:conc:echoservert:detach
    Free(vargp);                    // line:conc:echoservert:free
    // echo(connfd);
    //  여기서 부터 시작!
    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        // printf("server received %d bytes\n", n);
        command(BUF2, buf, argv, clientBuf);
        Rio_writen(connfd, clientBuf, MAXLINE);
    }
    save_binary_tree(tree_head);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */