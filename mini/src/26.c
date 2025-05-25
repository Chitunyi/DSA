// mini_j

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAXN 1000005

int N, M;

typedef struct Node
{
    int parent;
    int rank;
} Node;

Node nodes[MAXN];

void printPath(int x)
{
    int k = x;
    printf("%d", k);
    while(nodes[k].parent != k)
    {
        k = nodes[k].parent;
        printf(" %d", k);
    }
    printf("\n");
}

void link(int x, int y)
{
    if(nodes[x].rank > nodes[y].rank)
    {
        nodes[y].parent = x;
    }
    else
    {
        nodes[x].parent = y;
        if(nodes[x].rank == nodes[y].rank)
        {
            nodes[y].rank++;
        }
    }
}

int findSet(int x)
{
    if(nodes[x].parent != x)
    {
        nodes[x].parent = findSet(nodes[x].parent);
    }
    return nodes[x].parent;
}

void unionSet(int x, int y)
{
    int v = findSet(x);
    int w = findSet(y);
    if(v != w)
    {
        link(v, w);
    }
}

void makeSet(int x)
{
    nodes[x].parent = x;
    nodes[x].rank = 0;
}

int main()
{
    scanf("%d", &N);
    scanf("%d", &M);

    for(int i = 1; i <= N; i++) makeSet(i);

    for(int i = 0; i < M; i++)
    {
        char c;
        int x, y;
        scanf(" %c", &c);
        if(c == 'F')
        {
            scanf("%d", &x);
            findSet(x);

        }
        else if (c == 'U')
        {
            scanf("%d %d", &x, &y);
            unionSet(x, y);
        }
        else
        {
            scanf("%d", &x);
            printPath(x);
        }
    }

    return 0;
}