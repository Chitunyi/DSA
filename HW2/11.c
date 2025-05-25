#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    int id;
    int priority;
} Job;

typedef struct
{
    Job *heap;
    int size;
    int capacity;
} MinMaxHeap;

// MinMaxHeap結構
// 奇數層是min 小於底下
// 偶數層是max 大於底下
// Compare時必須一次跨兩層
// Assume 1-based index
MinMaxHeap *createHeap(int capacity)
{
    MinMaxHeap *h = (MinMaxHeap *)malloc(sizeof(MinMaxHeap));
    h->heap = (Job *)malloc(sizeof(Job) * (capacity + 1));
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void destroyHeap(MinMaxHeap *h)
{
    free(h->heap);
    free(h);
}

void swap(Job *a, Job *b)
{
    Job temp = *a;
    *a = *b;
    *b = temp;
}

// root at level 0
int getLevel(int index)
{
    int level = 0;
    while (index > 1)
    {
        index /= 2;
        level++;
    }
    return level;
}

void bubbleupMin(MinMaxHeap *h, int i)
{
    while (i > 3)
    {
        int grandparent = i / 4;
        if (h->heap[i].priority < h->heap[grandparent].priority)
        {
            swap(&h->heap[i], &h->heap[grandparent]);
            i = grandparent;
        }
        else
        {
            break;
        }
    }
}

void bubbleupMax(MinMaxHeap *h, int i)
{
    while (i > 3)
    {
        int grandparent = i / 4;
        if (h->heap[i].priority > h->heap[grandparent].priority)
        {
            swap(&h->heap[i], &h->heap[grandparent]);
            i = grandparent;
        }
        else
        {
            break;
        }
    }
}

void bubbleup(MinMaxHeap *h, int i)
{
    if (i == 1)
        return;

    int parent = i / 2;
    int level = getLevel(i);
    if (level % 2 == 0)
    {
        if (h->heap[i].priority > h->heap[parent].priority)
        {
            swap(&h->heap[i], &h->heap[parent]);
            bubbleupMax(h, parent);
        }
        else
        {
            bubbleupMin(h, i);
        }
    }
    else
    {
        if (h->heap[i].priority < h->heap[parent].priority)
        {
            swap(&h->heap[i], &h->heap[parent]);
            bubbleupMin(h, parent);
        }
        else
        {
            bubbleupMax(h, i);
        }
    }
}

void insertHeap(MinMaxHeap *h, int id, int priority)
{
    if (h->size + 1 >= h->capacity)
    {
        h->capacity *= 2;
        h->heap = realloc(h->heap, sizeof(Job) * (h->capacity + 1));
    }
    h->size++;
    h->heap[h->size].id = id;
    h->heap[h->size].priority = priority;
    bubbleup(h, h->size);
}

void pushdownMin(MinMaxHeap *h, int i)
{
    int size = h->size;
    while (1)
    {
        int m = i;
        int left = 2 * i, right = 2 * i + 1;

        if (left <= size && h->heap[left].priority < h->heap[m].priority)
            m = left;
        if (right <= size && h->heap[right].priority < h->heap[m].priority)
            m = right;

        for (int j = 0; j < 4; j++)
        {
            int child = 4 * i + j;
            if (child <= size && h->heap[child].priority < h->heap[m].priority)
                m = child;
        }
        if (m == i)
            break;

        if (m >= 4 * i && m <= 4 * i + 3)
        {
            swap(&h->heap[i], &h->heap[m]);
            int parent = m / 2;
            if (h->heap[m].priority > h->heap[parent].priority)
                swap(&h->heap[m], &h->heap[parent]);
            i = m;
        }
        else
        {
            swap(&h->heap[i], &h->heap[m]);
            break;
        }
    }
}

void pushdownMax(MinMaxHeap *h, int i)
{
    int size = h->size;
    while (1)
    {
        int m = i;
        int left = 2 * i, right = 2 * i + 1;

        if (left <= size && h->heap[left].priority > h->heap[m].priority)
            m = left;
        if (right <= size && h->heap[right].priority > h->heap[m].priority)
            m = right;

        for (int j = 0; j < 4; j++)
        {
            int child = 4 * i + j;
            if (child <= size && h->heap[child].priority > h->heap[m].priority)
                m = child;
        }
        if (m == i)
            break;

        if (m >= 4 * i && m <= 4 * i + 3)
        {
            swap(&h->heap[i], &h->heap[m]);
            int parent = m / 2;
            if (h->heap[m].priority < h->heap[parent].priority)
                swap(&h->heap[m], &h->heap[parent]);
            i = m;
        }
        else
        {
            swap(&h->heap[i], &h->heap[m]);
            break;
        }
    }
}

void pushdown(MinMaxHeap *h, int i)
{
    int level = getLevel(i);
    if (level % 2 == 0)
        pushdownMin(h, i);
    else
        pushdownMax(h, i);
}

Job ExtractMin(MinMaxHeap *h)
{
    Job minJob = h->heap[1];
    h->heap[1] = h->heap[h->size];
    h->size--;
    if (h->size > 0)
        pushdown(h, 1);
    return minJob;
}

Job ExtractMax(MinMaxHeap *h)
{
    if (h->size == 1)
    {
        return ExtractMin(h);
    }
    int maxIndex;
    if (h->size == 2)
        maxIndex = 2;
    else
    {
        maxIndex = (h->heap[2].priority > h->heap[3].priority) ? 2 : 3;
    }
    Job maxJob = h->heap[maxIndex];
    h->heap[maxIndex] = h->heap[h->size];
    h->size--;

    int level = getLevel(maxIndex);
    if (level % 2 == 0)
        pushdownMin(h, maxIndex);
    else
        pushdownMax(h, maxIndex);
    return maxJob;
}

int main()
{
    int N;
    scanf("%d", &N);
    MinMaxHeap *heap = createHeap(N + 1);

    for (int i = 0; i < N; i++)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int id, priority;
            scanf("%d %d", &id, &priority);
            insertHeap(heap, id, priority);
            printf("%d jobs waiting\n", heap->size);
        }
        else if (op == 2)
        {
            if (heap->size == 0)
                printf("no job in queue\n");
            else
            {
                Job job = ExtractMax(heap);
                printf("job %d with priority %d completed\n", job.id, job.priority);
            }
        }
        else if (op == 3)
        {
            if (heap->size == 0)
                printf("no job in queue\n");
            else
            {
                Job job = ExtractMin(heap);
                printf("job %d with priority %d dropped\n", job.id, job.priority);
            }
        }
    }

    destroyHeap(heap);
    return 0;
}
