#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "9.h"

struct Node *head = NULL;
struct Node *tail = NULL;
int next_node_id = 1;

int list_length = 0;

void Insert_After(struct Node *node, struct Node *prev)
{
    struct Node *next = Next_Node(node, prev);
    struct Node *new_node = New_XOR_Node((struct Node *)((uintptr_t)next ^ (uintptr_t)node));
    if (next != NULL)
    {
        next->neighbors = (struct Node *)((uintptr_t)next->neighbors ^ (uintptr_t)node ^ (uintptr_t)new_node);
    }
    node->neighbors = (struct Node *)((uintptr_t)prev ^ (uintptr_t)new_node);
}

void Remove_Here(struct Node *node, struct Node *prev)
{
    struct Node *next = Next_Node(node, prev);
    if (prev != NULL)
    {
        prev->neighbors = (struct Node *)((uintptr_t)prev->neighbors ^ (uintptr_t)node ^ (uintptr_t)next);
    }
    if (next != NULL)
    {
        next->neighbors = (struct Node *)((uintptr_t)next->neighbors ^ (uintptr_t)node ^ (uintptr_t)prev);
    }
    free(node);
}

void Reverse(struct Node *prev, struct Node *begin, struct Node *end, struct Node *next)
{
    if (prev != NULL)
    {
        prev->neighbors = (struct Node *)((uintptr_t)prev->neighbors ^ (uintptr_t)begin ^ (uintptr_t)end);
    }
    begin->neighbors = (struct Node *)((uintptr_t)begin->neighbors ^ (uintptr_t)prev ^ (uintptr_t)next);
    end->neighbors = (struct Node *)((uintptr_t)end->neighbors ^ (uintptr_t)prev ^ (uintptr_t)next);
    if (next != NULL)
    {
        next->neighbors = (struct Node *)((uintptr_t)next->neighbors ^ (uintptr_t)begin ^ (uintptr_t)end);
    }
}

int type_0(int k)
{
    struct Node *curr = head, *prev = NULL, *next;

    for (int i = 1; i < k; i++)
    {
        next = Next_Node(curr, prev);
        prev = curr;
        curr = next;
    }

    return curr->data;
}

void type_1(void)
{
    if (head == NULL)
    {
        head = tail = New_XOR_Node(NULL);
        list_length = 1;
    }
    else
    {
        struct Node *oldhead = head;
        struct Node *new_node = New_XOR_Node(oldhead);
        oldhead->neighbors = (struct Node *)((uintptr_t)oldhead->neighbors ^ (uintptr_t)new_node);
        head = new_node;
        list_length++;
    }
}

void type_2(int k)
{
    struct Node *curr = head, *prev = NULL, *next;

    for (int i = 1; i < k; i++)
    {
        next = Next_Node(curr, prev);
        prev = curr;
        curr = next;
    }

    if (Next_Node(curr, prev) == NULL)
    {
        Insert_After(curr, prev);
        tail = (struct Node *)((uintptr_t)curr->neighbors ^ (uintptr_t)prev);
    }
    else
    {
        Insert_After(curr, prev);
    }
    list_length++;
}

void type_3(int k)
{
    int target = list_length - k + 1;
    struct Node *curr, *prev, *next;

    if (target > list_length / 2)
    {
        int goback = list_length - target;
        curr = tail;
        next = NULL;
        prev = (struct Node *)((uintptr_t)curr->neighbors ^ (uintptr_t)next);

        for (int i = 0; i < goback; i++)
        {
            next = curr;
            curr = prev;
            prev = (struct Node *)((uintptr_t)curr->neighbors ^ (uintptr_t)next);
        }
    }
    else
    {
        curr = head;
        prev = NULL;
        for (int i = 1; i < target; i++)
        {
            next = Next_Node(curr, prev);
            prev = curr;
            curr = next;
        }
    }

    int istail = (curr == tail);
    Insert_After(curr, prev);
    list_length++;
    if (istail)
    {
        tail = Next_Node(curr, prev);
    }

    // if (Next_Node(curr, prev) == NULL)
    // {
    //     tail = curr->neighbors;
    // }
}

void type_4(int k)
{
    struct Node *curr = head, *prev = NULL, *next;

    for (int i = 1; i < k; i++)
    {
        next = Next_Node(curr, prev);
        prev = curr;
        curr = next;
    }

    struct Node *temp = Next_Node(curr, prev);
    Remove_Here(curr, prev);
    list_length--;
    if (prev == NULL)
    {
        head = temp;
    }
    if (temp == NULL)
    {
        tail = prev;
    }
}

void type_5(int k)
{
    int target = list_length - k + 1;
    struct Node *curr, *prev, *next;

    if (target > list_length / 2)
    {
        int goback = list_length - target;
        curr = tail;
        next = NULL;
        prev = (struct Node *)((uintptr_t)curr->neighbors ^ (uintptr_t)next);

        for (int i = 0; i < goback; i++)
        {
            next = curr;
            curr = prev;
            prev = (struct Node *)((uintptr_t)curr->neighbors ^ (uintptr_t)next);
        }
    }
    else
    {
        curr = head;
        prev = NULL;
        for (int i = 1; i < target; i++)
        {
            next = Next_Node(curr, prev);
            prev = curr;
            curr = next;
        }
    }

    struct Node *temp = Next_Node(curr, prev);
    Remove_Here(curr, prev);
    list_length--;

    if (prev == NULL)
    {
        head = temp;
    }
    if (temp == NULL)
    {
        tail = prev;
    }
}

void type_6(int k)
{
    struct Node *curr = head, *prev = NULL, *next;
    struct Node *A, *prev_A, *next_A;

    for (int i = 1; i < k; i++)
    {
        next = Next_Node(curr, prev);
        prev = curr;
        curr = next;
    }

    A = curr;
    prev_A = prev;
    next_A = Next_Node(A, prev_A);

    int target = list_length - k + 1;
    struct Node *B, *prev_B, *next_B;
    curr = head;
    prev = NULL;

    if (target > list_length / 2)
    {
        int goback = list_length - target;
        B = tail;
        next_B = NULL;
        prev_B = (struct Node *)((uintptr_t)B->neighbors ^ (uintptr_t)next_B);
        for (int i = 0; i < goback; i++)
        {
            next_B = B;
            B = prev_B;
            prev_B = (struct Node *)((uintptr_t)B->neighbors ^ (uintptr_t)next_B);
        }
    }
    else
    {
        B = head;
        prev_B = NULL;
        for (int i = 1; i < target; i++)
        {
            next_B = Next_Node(B, prev_B);
            prev_B = B;
            B = next_B;
        }
        next_B = Next_Node(B, prev_B);
    }

    if (A == B)
    {
        return;
    }

    if (k <= target)
    {
        Reverse(prev_A, A, B, next_B);
        if (prev_A == NULL)
        {
            head = B;
        }
        if (next_B == NULL)
        {
            tail = A;
        }
    }
    else
    {
        Reverse(prev_B, B, A, next_A);
        if (prev_B == NULL)
        {
            head = A;
        }
        if (next_A == NULL)
        {
            tail = B;
        }
    }

    // if (prev_A == NULL)
    // {
    //     head = B;
    // }
    // if (next_B == NULL)
    // {
    //     tail = A;
    // }
}