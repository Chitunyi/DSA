#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAXL 65
#define INIT_R 2


int N;

typedef struct Node
{
    char str[MAXL];
    // unsigned long long hash;
    uint64_t tau;
    struct Node* next;
} Node;

typedef struct Bucket
{
    Node *head, *tail;
    int size;
} Bucket;

uint64_t rabinkarp(const char *s) 
{
    const uint64_t d = 26;
    uint64_t h = 0;
    for (; *s; ++s) 
    {
        h = h * d + (uint64_t)(*s - 'a');
    }
    return h;
}

uint64_t lsb(uint64_t x, int n) 
{
    return (uint32_t)(x & ((1ULL << n) - 1));
}

void bucket_size(Bucket **B, int *cap, int dem) 
{
    if (dem <= *cap) return;
    int new_cap = dem;
    *B = realloc(*B, new_cap * sizeof(Bucket));
    for (int i = *cap; i < new_cap; ++i) 
    {
        (*B)[i].head = (*B)[i].tail = NULL;
        (*B)[i].size = 0;
    }
    *cap = new_cap;
}

void bucket_push(Bucket *B, const char *s, uint64_t tau) 
{
    Node *node = (Node*)malloc(sizeof(Node));
    strcpy(node->str, s);
    node->tau = tau;
    // node->next = B->head;
    node->next = NULL;
    // B->head = NULL;
    if(B->head == NULL)
    {
        B->head = B->tail = node;
    } 
    else 
    {
        B->tail->next = node;
        B->tail = node;
    }
    B->size++;
}

void split_bucket(Bucket *B, int idx, int new_idx, int r) 
{
    Node *curr = B[idx].head, *prev = NULL;
    while (curr) 
    {
        Node *next = curr->next;
        uint32_t h = lsb(curr->tau, r + 1);

        if (h == (uint32_t)new_idx) 
        {
            if (prev) 
                prev->next = next;
            else
                B[idx].head = next;
            B[idx].size--;

            /* 插到新 bucket */
            // curr->next = B[new_idx].head;
            curr->next = NULL;
            if(B[new_idx].head == NULL)
            {
                B[new_idx].head = B[new_idx].tail = curr;
            } 
            else 
            {
                B[new_idx].tail->next = curr;
                B[new_idx].tail = curr;
            }
            // B[new_idx].head = curr;
            B[new_idx].size++;

            curr = next;
        } 
        else 
        {
            prev = curr;
            curr = next;
        }
    }

    if(B[idx].head == NULL)
    {
        B[idx].tail = NULL;
    }
    else
    {
        B[idx].tail = prev;
    }
}


int main()
{
    scanf("%d", &N);
    int r = INIT_R, q = 0;
    int bucket_cap = (1 << r);
    Bucket *B = (Bucket*)calloc(bucket_cap, sizeof(Bucket));

    char s[MAXL];
    for (int i = 0; i < N; i++) 
    {
        scanf("%64s", s);
        uint64_t tau = rabinkarp(s);

        uint32_t h_r = lsb(tau, r);
        uint32_t target = (h_r < (uint32_t)q) ? lsb(tau, r + 1) : h_r;

        int active_cnt = (1 << r) + q;
        bucket_size(&B, &bucket_cap, active_cnt);

        bucket_push(&B[target], s, tau);

        if (B[target].size > 2) 
        {
            q++;
            int new_bucket_idx = (1 << r) + q - 1;
            bucket_size(&B, &bucket_cap, new_bucket_idx + 1);

            split_bucket(B, q - 1, new_bucket_idx, r);

            if (q == (1 << r)) 
            {
                r++;
                q = 0;
            }
        }
    }

    int total_buckets = (1 << r) + q;
    for (int i = 0; i < total_buckets; i++) 
    {
        // int stack_size = 0;
        // Node* stack[1024];
        // for (Node* p = B[i].head; p; p = p->next) stack[stack_size++] = p;

        // if (stack_size == 0) printf("-1\n");

        // for (int k = stack_size - 1; k >= 0; k--) {
        //     printf("%s%c", stack[k]->str, k ? ' ' : '\n');
        // }
        if (B[i].head == NULL) { puts("-1"); }
        else 
        {
        for (Node *p = B[i].head; p; p = p->next)
            printf("%s%c", p->str, p->next ? ' ' : '\n');
        }
    }

    return 0;
}