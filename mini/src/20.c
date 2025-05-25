// mini_d

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int n;

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// Floyd-Cycle-Detection(head):
//     hare = head
//     tortoise = head
//     while (hare != nil and hare.next != nil):
//         hare = hare.next.next
//         tortoise = tortoise.next
//         if hare = tortoise:
//             return True
//     return False

void floydcycle(Node *head, int record[], int *count, bool *cycle)
{
    *count = 0;
    if (head == NULL)
    {
        return;
    }

    record[(*count)++] = head->data;
    Node *hare = head;
    Node *tortoise = head;

    while (hare != NULL && hare->next != NULL)
    {
        if (hare->next->next == NULL)
        {
            *cycle = false;
            return;
        }

        hare = hare->next->next;
        record[(*count)++] = hare->data;
        tortoise = tortoise->next;

        if (hare == tortoise)
        {
            *cycle = true;
            return;
        }
    }
    *cycle = false;
}

void printresult(int record[], int count)
{

    for (int i = 0; i < count; i++)
    {
        printf("%d ", record[i]);
    }
    printf("\n");
}

int main()
{

    scanf("%d", &n);

    Node **nodes = (Node **)malloc(n * sizeof(Node *));

    for (int i = 0; i < n; i++)
    {
        nodes[i] = (Node *)malloc(sizeof(Node));
    }

    int *nextIndices = (int *)malloc(n * sizeof(int));

    for (int j = 0; j < n; j++)
    {
        int data, nextIndex;
        scanf("%d %d", &data, &nextIndex);
        nodes[j]->data = data;
        nextIndices[j] = nextIndex;
        nodes[j]->next = NULL;
    }

    for (int k = 0; k < n; k++)
    {
        if (nextIndices[k] > 0 && nextIndices[k] <= n)
            nodes[k]->next = nodes[nextIndices[k] - 1];
        else
            nodes[k]->next = NULL;
    }

    free(nextIndices);

    Node *head = nodes[0];

    int *record = (int *)malloc(n * sizeof(int));
    int count = 0;
    bool iscycle;

    floydcycle(head, record, &count, &iscycle);

    printresult(record, count);

    for (int i = 0; i < n; i++)
    {
        free(nodes[i]);
    }
    free(nodes);
    free(record);

    return 0;
}