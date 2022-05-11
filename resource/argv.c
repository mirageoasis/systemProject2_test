#include "argv.h"

void argvFeed(char *BUF2, char *argv[])
{
    int argvIdx = 0; // argv index
    char *ptr = BUF2;
    argv[argvIdx++] = ptr;   // 첫 출발
    ptr = strchr(BUF2, ' '); // 다음으로 스킵

    while (ptr != NULL)
    {
        *ptr = 0;
        argv[argvIdx++] = ptr + 1;
        ptr = strchr(ptr + 1, ' ');
    }
    argv[argvIdx] = NULL; // 마지막 null처리
}

void argvTest(char *argv[])
{
    for (int i = 0; argv[i] != NULL; i++)
    {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}