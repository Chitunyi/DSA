#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAXL 10005
int Q;
char buf[MAXL];
char outbuf[MAXL];
char *all_strings[MAXL];
int str_cnt = 0;

typedef struct Node
{
    struct Node *children[ALPHABET_SIZE];
    bool isEnd;
    int prefixcount;
} Node;

typedef struct Trie
{
    Node *root;
} Trie;

Node *createNode()
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->isEnd = false;
    newNode->prefixcount = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        newNode->children[i] = NULL;
    return newNode;
}

void initTrie(Trie *trie)
{
    trie->root = createNode();
}

void Insert(Trie *trie, const char *s) 
{
    Node *node = trie->root;
    node->prefixcount++;

    for (int i = 0; s[i]; i++) 
    {
        int idx = s[i] - 'a';
        if (node->children[idx] == NULL) 
        {
            node->children[idx] = createNode();
        }
        node = node->children[idx];
        node->prefixcount++;
    }
    node->isEnd = true;

    all_strings[str_cnt++] = strdup(s);
}

void Check(Trie *trie, const char *s) 
{
    Node *node = trie->root;
    for (int i = 0; s[i]; i++) 
    {
        int idx = s[i] - 'a';
        if (node->children[idx] == NULL) 
        {
            printf("NO\n");
            return;
        }
        node = node->children[idx];
    }
    if (node->isEnd) 
    {
        printf("YES\n");
    } 
    else 
    {
        printf("NO\n");
    }
}

void PrefixSearch(Trie *trie, const char *s) 
{
    Node *node = trie->root;
    for (int i = 0; s[i]; i++) 
    {
        int idx = s[i] - 'a';
        if (node->children[idx] == NULL) 
        {
            printf("0\n");
            return;
        }
        node = node->children[idx];
    }
    printf("%d\n", node->prefixcount);
}

bool dfs(Node *node, int depth) 
{
    if (node->isEnd) 
    {
        outbuf[depth] = '\0';
        return true;
    }
    for (int c = 0; c < ALPHABET_SIZE; c++) 
    {
        if (node->children[c]) 
        {
            outbuf[depth] = 'a' + c;
            if (dfs(node->children[c], depth + 1))
                return true;
        }
    }
    return false;
}

void lcp(Trie *trie, const char *s)
{
    Node *node = trie->root;
    
    int i = 0;
    while (s[i]) 
    {
        int idx = s[i] - 'a';
        if (node->children[idx] == NULL)
            break;
        node = node->children[idx];
        i++;
    }

    for (int j = 0; j < i; j++) 
    {
        outbuf[j] = s[j];
    }

    dfs(node, i);
    printf("%s\n", outbuf);
}

void Score(Trie *trie, const char *s)
{
    long long score = 0;
    for (int i = 0; i < str_cnt; i++) 
    {
        const char *q = all_strings[i];
        int j = 0;
        while (s[j] && q[j] && s[j] == q[j]) j++;
        score += j;
    }
    printf("%lld\n", score);
}

typedef struct Heap
{
    int size, capacity;
    int *a;
    long long sum;
}Heap;

Heap *newHeap(int capacity)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->a = (int *)malloc(sizeof(int) * capacity);
    heap->sum = 0;
    return heap;
}

void push(Heap *heap, int x)
{
    if (heap->size == heap->capacity) 
    {
        heap->capacity <<= 1;
        heap->a = (int *)realloc(heap->a, sizeof(int) * heap->capacity);
    }
    int i = heap->size++;
    heap->a[i] = x; 
    heap->sum += x;
    while (i && heap->a[(i-1)/2] < heap->a[i]) {
        int p = (i-1)/2;
        int tmp = heap->a[p]; 
        heap->a[p] = heap->a[i]; 
        heap->a[i] = tmp;
        i = p;
    }
}

int pop(Heap *heap)
{
    int ret = heap->a[0];
    heap->sum -= ret;
    heap->a[0] = heap->a[--heap->size];
    int i = 0;
    while (1)
    {
        int l = i*2+1, r = i*2+2, largest = i;
        if (l < heap->size && heap->a[l] > heap->a[largest]) largest = l;
        if (r < heap->size && heap->a[r] > heap->a[largest]) largest = r;
        if (largest == i) break;
        int tmp = heap->a[i]; 
        heap->a[i] = heap->a[largest]; 
        heap->a[largest] = tmp;
        i = largest;
    }
    return ret;
}

int max(Heap *heap)
{
    return heap->a[0];
}

Heap *merge(Heap *a, Heap *b) 
{
    if (a->size < b->size) { Heap *tmp=a; a=b; b=tmp; }
    for (int i=0;i<b->size;i++) push(a, b->a[i]);
    free(b->a); free(b);
    return a;
}

Heap *dfs_compress(Node *node, int depth) 
{
    Heap *h = newHeap(2);
    bool used_here = false;
    for (int c = 0; c < ALPHABET_SIZE; c++) 
    {
        if (node->children[c]) 
        {
            Heap *child = dfs_compress(node->children[c], depth+1);
            h = merge(h, child);
        }
    }
    if (node->isEnd) 
    {                        
        // 這個節點本身是某字串結尾
        push(h, depth);
        used_here = true;
    }
    /* depth==0 代表 root，空字串不能被選作前綴 */
    if (!used_here && depth>0 && h->size>0 && max(h) > depth) 
    {
        pop(h);
        push(h, depth);
        used_here = true;
    }
    return h;
}

void Compress(Trie *trie)
{
    Heap *h = dfs_compress(trie->root, 0);
    printf("%lld\n", h->sum);
    free(h->a);
    free(h);
}

int main()
{
    scanf("%d", &Q);

    Trie trie;
    initTrie(&trie);

    for (int i = 0; i < Q; i++)
    {
        int op;
        scanf("%d", &op);
        if(op == 6)
        {
            Compress(&trie);
        }
        else
        {
            scanf("%s", buf);
            switch (op)
            {
                case 1:  // Insert
                    Insert(&trie, buf);
                    break;
                case 2:  // Check
                    Check(&trie, buf);
                    break;
                case 3:  // Prefix_Search
                    PrefixSearch(&trie, buf);
                    break;
                case 4:  // LCP
                    lcp(&trie, buf);
                    break;
                case 5:  // Score
                    Score(&trie, buf);
                    break;
            }
        }
    }
    return 0;
}
