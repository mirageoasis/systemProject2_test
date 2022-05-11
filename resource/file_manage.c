#include "file_manage.h"

void read_file()
{
    FILE *fp;
    int stock_id;
    int remains;
    int cost;

    if ((fp = fopen("stock.txt", "r")) == NULL)
    {
        fprintf(stdout, "file does not exists!\n");
        exit(0);
    }

    while (fscanf(fp, "%d %d %d", &stock_id, &remains, &cost) != EOF)
    {
        lines++;
    }
    // fprintf(stdout, "total line is! %d\n", lines);

    fclose(fp);

    if ((fp = fopen("stock.txt", "r")) == NULL)
    {
        fprintf(stdout, "file does not exists!\n");
        exit(0);
    }

    temp_array = (STOCK_NODE *)malloc(sizeof(STOCK_NODE) * lines); /*파일의 줄 수 만큼 주식 개수 생성*/

    for (int i = 0; i < lines; i++)
    {
        fscanf(fp, "%d %d %d", &temp_array[i].ID, &temp_array[i].left_stock, &temp_array[i].price);
        // fprintf(stdout, "stock %d info is :  %d %d %d\n", i + 1, temp_array[i].ID, temp_array[i].left_stock, temp_array[i].price);
    }

    fclose(fp);
}

void save_binary_tree(STOCK_NODE *cur)
{
    FILE *fp;
    // 일단 여기서 순회하면서 write function 으로 사용한다.
    // open function 활용하기
    if ((fp = fopen("stock.txt", "w")) == NULL)
    {
        fprintf(stdout, "file does not exists!\n");
        exit(0);
    }

    recursive_write(cur, fp);

    fclose(fp);
}

void recursive_write(STOCK_NODE *cur, FILE *fp)
{
    if (!cur) // 없으면 return
        return;
    recursive_write(cur->left, fp);
    // sprintf(clientBuf, "show binary tree: ");
    fprintf(fp, "%d %d %d\n", cur->ID, cur->left_stock, cur->price); // 구현에 따라 다르다
    // fprintf(stdout, "%s\n", clientBuf);
    recursive_write(cur->right, fp);
}