#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

extern STOCK_NODE *temp_array; // 전역변수
extern int lines;              // 전역변수

void read_file();
// 파일에 기록
void save_binary_tree(STOCK_NODE *cur);
void recursive_write(STOCK_NODE *cur, FILE *fp);