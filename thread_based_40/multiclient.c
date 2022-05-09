#include "csapp.h"
#include <time.h>

#define MAX_CLIENT 100		// 최대 client 명수
#define ORDER_PER_CLIENT 50 // 처음에 시작하는 주문 개수
#define ORDER_DEC 10		// 주문량 감수
#define STOCK_NUM 40		// 주식의 개수
#define BUY_SELL_MAX 15		// 거래 한도
#define CLIENT_START 50		// 처음에 시작하는 client 개수
#define CLIENT_DEC 10		// client 감소
#define MEAN_LOOP_CNT 5		// 평균 낼 때 순회하는 루프수

int main(int argc, char **argv)
{
	struct timeval start;
	struct timeval end;
	unsigned long e_usec;
	FILE *fp;

	pid_t pids[MAX_CLIENT];
	int runprocess = 0, status;

	int clientfd, num_client;
	char *host, *port, buf[MAXLINE], tmp[3];
	char buf2[MAXLINE];
	rio_t rio;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}

	if ((fp = fopen("result.csv", "w")) == NULL)
	{
		fprintf(stdout, "cannot open file!\n");
		exit(0);
	}
	host = argv[1];
	port = argv[2];
	num_client = CLIENT_START; // 여기 설정잘하기
	int max_order = ORDER_PER_CLIENT;
	int cnt = 1;
	// i 는 order 개수  j는 client 의 개수
	/*	fork for each client process	*/
	sprintf(buf2, "stock number,order per client,client number,time\n");
	for (int i = max_order; i > 0; i -= ORDER_DEC)
	{
		for (int j = num_client; j > 0; j -= CLIENT_DEC) // client 개수 별로 실험하기 j 는 client의 개수
		{
			e_usec = 0; // 시간 초기화
			for (int k = 0; k < MEAN_LOOP_CNT; k++)
			{
				runprocess = 0; // 자식 프로세스의 개수 초기화해주기
				gettimeofday(&start, 0);
				while (runprocess < j)
				{
					pids[runprocess] = fork();

					if (pids[runprocess] < 0)
						return -1;
					/*	child process		*/
					else if (pids[runprocess] == 0)
					{
						// printf("child %ld\n", (long)getpid());

						clientfd = Open_clientfd(host, port);
						Rio_readinitb(&rio, clientfd);
						srand((unsigned int)getpid());

						for (int l = 0; l < i; l++)
						{

							int option = rand() % 3;

							if (option == 0)
							{ // show
								strcpy(buf, "show\n");
							}
							else if (option == 1)
							{ // buy
								int list_num = rand() % STOCK_NUM + 1;
								int num_to_buy = rand() % BUY_SELL_MAX + 1; // 1~10

								strcpy(buf, "buy ");
								sprintf(tmp, "%d", list_num);
								strcat(buf, tmp);
								strcat(buf, " ");
								sprintf(tmp, "%d", num_to_buy);
								strcat(buf, tmp);
								strcat(buf, "\n");
							}
							else if (option == 2)
							{ // sell
								int list_num = rand() % STOCK_NUM + 1;
								int num_to_sell = rand() % BUY_SELL_MAX + 1; // 1~10

								strcpy(buf, "sell ");
								sprintf(tmp, "%d", list_num);
								strcat(buf, tmp);
								strcat(buf, " ");
								sprintf(tmp, "%d", num_to_sell);
								strcat(buf, tmp);
								strcat(buf, "\n");
							}
							// strcpy(buf, "buy 1 2\n");
							// fprintf(stdout, "id %d command: %s", getpid(), buf);
							// fprintf(stdout, "%s", buf);
							Rio_writen(clientfd, buf, strlen(buf));
							Rio_readnb(&rio, buf, MAXLINE);
							Fputs(buf, stdout);

							usleep(1000000);
						}

						Close(clientfd);
						exit(0);
					}
					runprocess++;
				}

				for (int l = 0; l < j; l++)
				{
					waitpid(pids[l], &status, 0);
				}
				gettimeofday(&end, 0);
				e_usec += ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
			}

			sprintf(buf2 + strlen(buf2), "%d,%d,%d,%lu\n", STOCK_NUM, i, j, e_usec / MEAN_LOOP_CNT);			 // order per client, client number , time
			fprintf(stdout, "order per %d cilent number %d : %lu microseconds\n", i, j, e_usec / MEAN_LOOP_CNT); // order per client, client number , time
		}
	}
	fprintf(fp, "%s", buf2);
	fclose(fp);
	return 0;
}
