#include <semaphore.h>

#ifndef STRUCT_H__
#define STRUCT_H__

typedef struct STOCK_NODE
{
    int ID;                   //주식 id
    int left_stock;           //주식 수량
    int price;                //주식의 가격
    int readcnt;              // 주식을 읽는 사용자 수
    sem_t mutex;              // 일반 세마포어
    sem_t write;              // writer semaphore
    struct STOCK_NODE *left;  // 왼쪽 자식
    struct STOCK_NODE *right; // 오른쪽 자식
} STOCK_NODE;

#endif