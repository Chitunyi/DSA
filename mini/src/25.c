#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int time;

typedef struct Node
{
    int color; /* white = 0, gray = 1, black = -1*/
    int d_t;
    int f_t;
    int pi;
    int deg;
    int *adj;
} Node;

typedef struct Graph
{
    int V;
    Node *nodes;
} Graph;

void dfs_visit(Graph *G, int u)
{
    Node *U = &G->nodes[u];
    time++;
    G->nodes[u].d_t = time;
    G->nodes[u].color = 1;
    for (int j = 0; j < U->deg; j++)
    {
        int v = U->adj[j];
        Node *V = &G->nodes[v];
        if (V->color == 0)
        {
            V->pi = u;
            dfs_visit(G, v);
        }
    }
    U->color = -1;
    time++;
    U->f_t = time;
}

void dfs(Graph *G)
{
    for (int u = 1; u <= G->V; u++)
    {
        G->nodes[u].color = 0;
        G->nodes[u].pi = -1;
    }

    time = 0;
    for (int u = 1; u <= G->V; u++)
    {
        if (G->nodes[u].color == 0)
        {
            dfs_visit(G, u);
        }
    }
}

void printresult(Graph *G)
{
    for (int u = 1; u <= G->V; u++)
    {
        printf("%d ", u);
        for (int j = 0; j < G->nodes[u].deg; j++)
        {
            int v = G->nodes[u].adj[j];
            if (G->nodes[v].pi == u)
            {
                printf("T");
            }
            else if (G->nodes[v].d_t < G->nodes[u].d_t && G->nodes[v].f_t > G->nodes[u].f_t)
            {
                printf("B");
            }
            else if (G->nodes[v].d_t > G->nodes[u].d_t && G->nodes[v].f_t < G->nodes[u].f_t)
            {
                printf("F");
            }
            else
            {
                printf("C");
            }
        }
        printf("\n");
    }
}

int main()
{
    Graph G;
    scanf("%d", &G.V);
    G.nodes = malloc((G.V + 1) * sizeof(Node));

    for (int u = 1; u <= G.V; u++)
    {
        scanf("%d", &G.nodes[u].deg);
        if (G.nodes[u].deg > 0)
        {
            G.nodes[u].adj = malloc(G.nodes[u].deg * sizeof(int));
            for (int j = 0; j < G.nodes[u].deg; j++)
            {
                scanf("%d", &G.nodes[u].adj[j]);
            }
        }
        else
        {
            G.nodes[u].adj = NULL;
        }
    }

    dfs(&G);
    printresult(&G);
    return 0;
}