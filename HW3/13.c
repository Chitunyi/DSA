#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAXN 100005
#define MAXM 1000005

int N, M, mode;

typedef struct Edge
{
    int dest;
    int next;
} Edge;

Edge edges[MAXM * 2];
int head[MAXN], edge_cnt = 0;

void addEdge(int u, int v)
{
    edges[edge_cnt].dest = v;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt++;
}

int dfn[MAXN], low[MAXN], parent[MAXN], timer = 0;
bool isArt[MAXN];
int stack_u[MAXM], stack_v[MAXM], stack_top = 0;
int bridge_count = 0;

typedef struct Comp
{
    int *nodes;
    int size;
}Comp;

Comp comps[MAXN];
int comp_count = 0;

int cmp_int(const void *a, const void *b) 
{
    return (*(int *)a - *(int *)b);
}

int comp_cmp(const void *a, const void *b) 
{
    const Comp *A = (const Comp *)a;
    const Comp *B = (const Comp *)b;
    for (int i = 0; i < A->size && i < B->size; i++) 
    {
        if (A->nodes[i] != B->nodes[i])
            return A->nodes[i] - B->nodes[i];
    }
    return A->size - B->size;
}

void dfs(int u) {
    dfn[u] = low[u] = ++timer;
    int children = 0;
    for (int ei = head[u]; ei != -1; ei = edges[ei].next) 
    {
        int v = edges[ei].dest;
        if (!dfn[v]) 
        {
            children++;
            parent[v] = u;

            /* Store the endpoint of the edge */
            stack_u[stack_top] = u;
            stack_v[stack_top] = v;
            stack_top++;

            dfs(v);

            /* Count Bridge */
            if (low[v] > dfn[u])
                {
                    bridge_count++;
                }

            /* Check Artpoint */
            if ((parent[u] != -1 && low[v] >= dfn[u]) || (parent[u] == -1 && children > 1))
                {
                    isArt[u] = true;
                }
            
            /* Set low */
            low[u] = low[u] < low[v] ? low[u] : low[v];

            /* Check BCC */
            if (low[v] >= dfn[u]) {
                int tmp_cap = stack_top * 2;
                int *tmp = malloc(tmp_cap * sizeof(int));
                int tmp_len = 0;
                while (1) {
                    stack_top--;
                    int uu = stack_u[stack_top];
                    int vv = stack_v[stack_top];
                    tmp[tmp_len++] = uu;
                    tmp[tmp_len++] = vv;
                    if (uu == u && vv == v)
                        break;
                }
                qsort(tmp, tmp_len, sizeof(int), cmp_int);
                int unique_len = 0;
                for (int i = 0; i < tmp_len; i++) {
                    if (i == 0 || tmp[i] != tmp[i - 1])
                        tmp[unique_len++] = tmp[i];
                }
                comps[comp_count].size = unique_len;
                comps[comp_count].nodes = malloc(unique_len * sizeof(int));
                for (int i = 0; i < unique_len; i++)
                    comps[comp_count].nodes[i] = tmp[i];
                comp_count++;
                free(tmp);
            }
        } 

        /*  */
        else if (v != parent[u] && dfn[v] < dfn[u]) 
        {
            stack_u[stack_top] = u;
            stack_v[stack_top] = v;
            stack_top++;
            low[u] = low[u] < dfn[v] ? low[u] : dfn[v];
        }
    }
}

int main()
{
    scanf("%d %d %d", &N, &M, &mode);
    for(int i = 0; i < N; i++){
        head[i] = -1;
        dfn[i] = 0; 
        low[i] = 0;
        parent[i] = -1;
        isArt[i] = false;
    }
    for (int j = 0; j < M; j++)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(u, v);
        addEdge(v, u);
    }

    dfs(0);

    if(mode == 1)
    {
        int art_cnt = 0;
        for (int i = 0; i < N; i++)
        {
            if (isArt[i]) art_cnt++;
        }
        printf("%d\n", art_cnt);
        for (int i = 0; i < N; i++)
        {
            if (isArt[i]) printf("%d ", i);
        }
        printf("\n");
    }
    else if(mode == 2)
    {
        printf("%d\n", bridge_count);
    }
    else
    {
        qsort(comps, comp_count, sizeof(Comp), comp_cmp);
        printf("%d\n", comp_count);
        for (int i = 0; i < comp_count; i++) 
        {
            for (int j = 0; j < comps[i].size; j++)
                printf("%d ", comps[i].nodes[j]);
            printf("\n");
        }
    }
    return 0;
}