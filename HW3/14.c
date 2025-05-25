#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAXN 500005

int N, Q;
int *pathArr;

typedef struct Computer
{
    long long size, parent, virus;
}Computer;

typedef struct Virus
{
    long long parent, level, count, tag;
}Virus;

typedef struct Modify
{
    long long *ptr;
    long long old_value;
}Modify;

typedef struct ModifyStack
{
    Modify *data;
    int sizee, capacity;
}ModifyStack;

Computer *com;
Virus *vir;
long long *damage;
long long *off;
long long *offV;
int *histSize;
ModifyStack history;
int opcnt;
long long *cur_id;
int tot_nodes;

void init()
{
    com = malloc(sizeof(Computer)*(N+Q+1));
    vir = malloc(sizeof(Virus)*(N+1));
    damage = malloc(sizeof(long long)*(N+Q+1));
    off = malloc(sizeof(long long)*(N+Q+1));
    offV = malloc(sizeof(long long)*(N+Q+1));
    histSize = calloc(Q+2, sizeof(int));
    history.capacity  = Q*20 + 1; 
    history.data = malloc(sizeof(Modify)*history.capacity);
    history.sizee = 0;
    opcnt = 0;
    cur_id = malloc(sizeof(long long)*(N+Q+1));
    tot_nodes = N;
    pathArr = malloc(sizeof(int)*(N+Q+1));

    for(int i = 1; i <= N+Q; i++)
    {
        if(i <= N)
        {
            com[i].parent = i;
            com[i].size = 1;
            com[i].virus = i;

            vir[i].parent = i;
            vir[i].level = 1;
            vir[i].count = 1;
            vir[i].tag = 0;  

            damage[i] = 0;
            off[i] = 0;
            offV[i] = 0;
            cur_id[i] = i;
        }
        else
        {
            com[i].parent = i;
            com[i].size = 1;
            com[i].virus = i;
            damage[i] = 0;
            off[i] = 0;
            offV[i] = 0;
            cur_id[i] = 0;
        }
    }

}

void modify(long long *ptr, long long value)
{
    if(history.sizee == history.capacity)
    {
        history.capacity *= 2;
        history.data = (Modify *)realloc(history.data, sizeof(Modify)*history.capacity);
    }
    history.data[history.sizee].ptr = ptr;
    history.data[history.sizee].old_value = *ptr;
    *ptr = value;
    history.sizee++;
    histSize[opcnt]++;
}

void revert()
{
    if(opcnt == 0) return;

    int cnt = histSize[opcnt];
    while(cnt--)
    {
        Modify m = history.data[--history.sizee];
        *m.ptr = m.old_value;
    }
    histSize[opcnt] = 0;
    opcnt--;
}

int findcom_root(int x) 
{
    while (com[x].parent != x) {
        x = com[x].parent;
    }
    return x;
}

int findcom_with_off(int x, long long *sumOff) 
{
    int top = 0, cur = x;
    long long total = 0;

    while (com[cur].parent != cur) 
    {
        pathArr[top++] = cur;
        total += off[cur];
        cur = com[cur].parent;
    }

    int root = cur;
    *sumOff = total;

    long long carry = 0;
    for (int i = top - 1; i >= 0; --i) 
    {
        int node = pathArr[i];
        modify(&off[node], off[node] + carry);
        modify(&com[node].parent, root);

        carry = off[node];
    }
    return root;
}

int findvir_root(int x) 
{
    while (vir[x].parent != x) x = vir[x].parent;
    return x;
}

int findvir_with_off(int x, long long *sumV) 
{
    int top = 0, cur = x;
    long long total = 0;

    while (vir[cur].parent != cur) 
    {
        pathArr[top++] = cur;
        total += offV[cur];
        cur = vir[cur].parent;
    }
    int root = cur;
    *sumV = total;

    long long carry = 0;
    for (int i = top - 1; i >= 0; --i) 
    {
        int node = pathArr[i];
        modify(&offV[node], offV[node] + carry);
        modify(&vir[node].parent, root);
        carry = offV[node];
    }
    return root;
}

int findcom_no_modify(int x, long long *sumOff) {
    *sumOff = 0;
    while (com[x].parent != x) {
        *sumOff += off[x];
        x = com[x].parent;
    }
    return x;
}
int findvir_no_modify(int x, long long *sumV) {
    *sumV = 0;
    while (vir[x].parent != x) {
        *sumV += offV[x];
        x = vir[x].parent;
    }
    return x;
}

void NetworkMerge(int x, int y)
{
    int u = cur_id[x];
    int v = cur_id[y];
    long long offU, offV;
    int rootx = findcom_with_off(u, &offU);
    int rooty = findcom_with_off(v, &offV);
    if(rootx == rooty) return;

    long long offA, offB;
    int va = findvir_no_modify(com[rootx].virus, &offA);
    int vb = findvir_no_modify(com[rooty].virus, &offB);

    // if (va == vb) 
    // {
    //     modify(&com[rooty].parent, rootx);
    //     modify(&com[rootx].size, com[rootx].size + com[rooty].size);
    //     return;
    // }

    long long winnerV = (vir[va].level >= vir[vb].level ? va : vb);
    long long loserV  = (winnerV == va ? vb : va);
    int winnerRoot = (winnerV == va ? rootx : rooty);
    int loserRoot  = (winnerRoot == rootx ? rooty : rootx);

    // if (winnerV == loserV) 
    // {
    //     modify(&com[loserRoot].parent, winnerRoot);
    //     modify(&com[winnerRoot].size, com[winnerRoot].size + com[loserRoot].size);
    //     return;
    // }

    long long offWinRoot, offLoseRoot;
    int rootVirWin = findvir_no_modify(com[winnerRoot].virus, &offWinRoot);
    int rootVirLose = findvir_no_modify(com[loserRoot].virus,  &offLoseRoot);
    long long full_tag_win   = vir[rootVirWin].tag - offWinRoot;//(winnerV==va? offA : offB);
    long long full_tag_lose  = vir[rootVirLose].tag - offLoseRoot;//(loserV==va? offA : offB);

    modify(&off[loserRoot], off[loserRoot] + /* (vir[winnerV].tag - vir[loserV].tag) */ (full_tag_win - full_tag_lose));

    modify(&com[loserRoot].parent, winnerRoot);
    modify(&com[winnerRoot].size, com[winnerRoot].size + com[loserRoot].size);
    modify(&com[loserRoot].virus, rootVirWin);


    long long moved = com[loserRoot].size;
    modify(&vir[rootVirWin].count, vir[rootVirWin].count + moved);
    modify(&vir[rootVirLose].count, vir[rootVirLose].count - moved);
}

void VirusFusion(int x, int y)
{

    long long offX, offY;
    int rootx = findvir_no_modify(x, &offX);
    int rooty = findvir_no_modify(y, &offY);
    if(rootx == rooty) return;

    if (vir[rootx].count < vir[rooty].count) 
    {
        int tmp = rootx; 
        rootx = rooty; 
        rooty = tmp;
    }

    long long tx = vir[rootx].tag; // - offX;
    long long ty = vir[rooty].tag; // - offY;

    modify(&vir[rootx].count, vir[rootx].count + vir[rooty].count);
    modify(&vir[rootx].level, vir[rooty].level + vir[rootx].level);

    modify(&vir[rooty].parent, rootx);
    modify(&offV[rooty], tx - ty);
    modify(&vir[rooty].count, 0);
}

void reinstall(int k, int s)
{
    int old_id = cur_id[k];
    long long co;
    int comk = findcom_with_off(old_id, &co);
    modify(&com[comk].size, com[comk].size - 1);

    long long vo;
    int virk = findvir_no_modify(com[comk].virus, &vo);
    modify(&vir[virk].count, vir[virk].count - 1);

    int new_id = ++tot_nodes;
    modify(&cur_id[k], new_id);

    int newV = findvir_no_modify(s, &vo);
    modify(&com[new_id].virus, newV);
    long long vir_tag = vir[newV].tag;
    // vo is new 
    modify(&damage[new_id], - vir_tag);

    modify(&com[new_id].parent, new_id);
    modify(&com[new_id].size, 1LL);
    modify(&off[new_id], 0LL);

    modify(&vir[newV].count, vir[newV].count + 1);
}

int main()
{
    scanf("%d %d", &N, &Q);

    init();

    for(int i = 0; i < Q; i++)
    {
        int op;
        scanf("%d", &op);
        if(op == 1)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            // Network merge
            opcnt++;
            int u = cur_id[x];
            int v = cur_id[y];
            int rx = findcom_root(u);
            int ry = findcom_root(v);
            if(rx != ry) NetworkMerge(x, y);
            else histSize[opcnt] = 0;
        }
        else if(op == 2)
        {
            int t;
            scanf("%d", &t);
            // Virus evolution
            opcnt++;
            int virt = findvir_root(t);
            modify(&vir[virt].level, vir[virt].level + 1);
        }
        else if(op == 3)
        {
            int t;
            scanf("%d", &t);
            // Virus attack
            opcnt++;
            int virt = findvir_root(t);
            modify(&vir[virt].tag, vir[virt].tag + vir[virt].level);
        }
        else if(op == 4)
        {
            int k, s;
            scanf("%d %d", &k, &s);
            // Reinstall computer
            opcnt++;
            reinstall(k, s);
        }
        else if(op == 5)
        {
            int a, b;
            scanf("%d %d", &a, &b);
            // Virus fusion
            opcnt++;
            int ra = findvir_root(a);
            int rb = findvir_root(b);
            if(ra != rb) VirusFusion(a, b);
            else histSize[opcnt] = 0;
        }
        else if(op == 6)
        {
            int k;
            scanf("%d", &k);   
            // Query computer status
            int id = cur_id[k];
            long long offCsum;
            int comk = findcom_no_modify(id, &offCsum);
            long long offVsum;
            int virt = findvir_no_modify(com[comk].virus, &offVsum);
            long long vir_tag = vir[virt].tag;
            long long dd = damage[id] + (vir_tag - offVsum) - offCsum;
            printf("%lld %lld %lld\n", dd, vir[virt].level, vir[virt].count);
        }
        else if(op == 7)
        {
            // Revert
            revert();
        }
    }
    return 0;
}
