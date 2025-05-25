#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int p;
    int q;
    int num;
} Map;

typedef struct
{
    char name[65];
    int **board;
    int *rowcount;
    int *colcount;
    int diagcount1;
    int diagcount2;
    Map *mapping;
    int mapsize;
} Player;

int size;
int people;

int compareMap(const void *a, const void *b)
{
    return ((Map *)a)->num - ((Map *)b)->num;
}

bool numposition(Player *player, int num)
{
    Map key;
    key.num = num;
    Map *find = bsearch(&key, player->mapping, player->mapsize, sizeof(Map), compareMap);
    if (find != NULL)
    {
        int i = find->p;
        int j = find->q;
        if (player->board[i][j] != 0)
        {
            player->board[i][j] = 0;
            player->rowcount[i]--;
            player->colcount[j]--;
            if (i == j)
                player->diagcount1--;
            if (i == size - 1 - j)
                player->diagcount2--;
            return true;
        }
    }
    return false;
}

bool checkbingo(Player *player)
{
    for (int i = 0; i < size; i++)
    {
        if (player->rowcount[i] == 0)
            return true;
        if (player->colcount[i] == 0)
            return true;
    }

    if (player->diagcount1 == 0 || player->diagcount2 == 0)
        return true;

    return false;
}

void printresult(int indice, char winner[][65], int winnum)
{
    printf("%d ", indice);
    for (int i = 0; i < winnum; i++)
    {
        printf("%s ", winner[i]);
    }
    printf("\n");
}

int main()
{

    scanf("%d %d", &people, &size);

    Player *players = (Player *)malloc(people * sizeof(Player));

    for (int i = 0; i < people; i++)
    {
        scanf("%s", players[i].name);

        players[i].board = (int **)malloc(size * sizeof(int *));

        players[i].rowcount = (int *)malloc(size * sizeof(int));
        players[i].colcount = (int *)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++)
        {
            players[i].rowcount[j] = size;
            players[i].colcount[j] = size;
        }
        players[i].diagcount1 = size;
        players[i].diagcount2 = size;

        players[i].mapsize = size * size;
        players[i].mapping = (Map *)malloc(size * size * sizeof(Map));

        for (int p = 0; p < size; p++)
        {
            players[i].board[p] = (int *)malloc(size * sizeof(int));
            for (int q = 0; q < size; q++)
            {
                scanf("%d", &players[i].board[p][q]);
                players[i].mapping[p * size + q].p = p;
                players[i].mapping[p * size + q].q = q;
                players[i].mapping[p * size + q].num = players[i].board[p][q];
            }
        }

        qsort(players[i].mapping, size * size, sizeof(Map), compareMap);
    }

    int *num = (int *)malloc(size * size * sizeof(int));
    for (int l = 0; l < size * size; l++)
    {
        scanf("%d", &num[l]);
    }

    int indice = -1;
    char winner[people][65];
    int winnum = 0;

    for (int l = 0; l < size * size; l++)
    {
        for (int m = 0; m < people; m++)
        {
            numposition(&players[m], num[l]);
        }

        for (int m = 0; m < people; m++)
        {
            if (checkbingo(&players[m]))
            {
                bool exists = false;
                for (int n = 0; n < winnum; n++)
                {
                    if (strcmp(winner[n], players[m].name) == 0)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    strcpy(winner[winnum], players[m].name);
                    winnum++;
                }
            }
        }
        if (winnum > 0)
        {
            indice = num[l];
            break;
        }
    }

    printresult(indice, winner, winnum);

    for (int s = 0; s < people; s++)
    {
        for (int t = 0; t < size; t++)
        {
            free(players[s].board[t]);
        }
        free(players[s].rowcount);
        free(players[s].colcount);
        free(players[s].board);
        free(players[s].mapping);
    }
    free(players);
    free(num);

    return 0;
}