
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 5001

void insert(char *str, char *pos, char symbol, int k)
{

    int len = strlen(str);
    int insertpos = 0;

    if (strcmp(pos, "left") == 0)
    {
        insertpos = 0;
    }
    else if (strcmp(pos, "right") == 0)
    {
        insertpos = len;
    }
    else
    {
        insertpos = k - 1;
    }

    for (int i = len; i >= insertpos; i--)
    {
        str[i + 1] = str[i];
    }
    str[insertpos] = symbol;
}

void delete(char *str, char *pos, int k)
{

    int len = strlen(str);
    int insertpos = 0;

    if (strcmp(pos, "left") == 0)
    {
        insertpos = 0;
    }
    else if (strcmp(pos, "right") == 0)
    {
        insertpos = len - 1;
    }
    else
    {
        insertpos = k - 1;
    }

    for (int j = insertpos; j < len; j++)
    {
        str[j] = str[j + 1];
    }
}

void scan(char *str)
{
    int len = strlen(str);

    if (len == 0)
    {
        printf("0\n");
        return;
    }

    int maxcount = 1, count = 1;

    for (int i = 1; i < len; i++)
    {
        if (str[i] == str[i - 1])
        {
            count++;
        }
        else
        {
            if (count > maxcount)
            {
                maxcount = count;
            }
            count = 1;
        }
    }

    if (count > maxcount)
    {
        maxcount = count;
    }

    count = 1;
    char result[10000];
    int index = 0;

    for (int j = 1; j <= len; j++)
    {
        if (j < len && str[j] == str[j - 1])
        {
            count++;
        }
        else
        {
            if (count == maxcount)
            {
                result[index++] = str[j - 1];
            }
            // if (count == maxcount)
            // {
            //     int exists = 0;
            //     for (int i = 0; i < index; i++)
            //     {
            //         if (result[i] == str[j - 1])
            //         {
            //             exists = 1;
            //             break;
            //         }
            //     }
            //     if (!exists)
            //     {
            //         result[index++] = str[j - 1];
            //     }
            // }
            count = 1;
        }
    }

    if (len != 0)
    {
        for (int k = 0; k < index; k++)
        {
            printf("%c ", result[k]);
        }
        printf("%d\n", maxcount);
    }
}

int main(void)
{
    char str[MAX_SIZE] = {0};
    char command[30];
    char pos[10];
    char symbol;
    int k;

    while (scanf("%s", command) != EOF)
    {
        if (strcmp(command, "insert") == 0)
        {
            scanf("%s", pos);
            if (strcmp(pos, "left") == 0 || strcmp(pos, "right") == 0)
            {
                scanf(" %c", &symbol);
                insert(str, pos, symbol, 0);
            }
            else
            {
                k = atoi(pos);
                scanf(" %c", &symbol);
                insert(str, "num", symbol, k);
            }
        }
        else if (strcmp(command, "delete") == 0)
        {
            scanf("%s", pos);
            if (strcmp(pos, "left") == 0 || strcmp(pos, "right") == 0)
            {
                delete (str, pos, 0);
            }
            else
            {
                k = atoi(pos);
                delete (str, "num", k);
            }
        }
    }

    scan(str);
    // printf("%s\n", str);

    return 0;
}
