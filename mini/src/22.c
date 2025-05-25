// mini_f

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int parent(int i)
{
    return (i / 2);
}

int heapincreasekey(int *arr, int i, int key)
{

    // if (key < arr[i])
    // {
    //     return -1;
    // }

    arr[i] = key;

    while (i > 1 && arr[parent(i)] > arr[i])
    {
        // swap
        int temp = arr[i];
        arr[i] = arr[parent(i)];
        arr[parent(i)] = temp;
        i = parent(i);
    }

    return 0;
}

void minheapinsert(int *arr, int *size, int key)
{
    (*size)++;
    // arr[*size] = key;
    heapincreasekey(arr, *size, key);
}

int main()
{
    int N;
    scanf("%d", &N);
    int *heap = (int *)malloc((N + 1) * sizeof(int));

    int size = 0;
    int x;

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &x);
        minheapinsert(heap, &size, x);
    }

    for (int j = 1; j <= N; j++)
    {

        printf("%d%s", heap[j], (j < N) ? " " : "\n");
    }

    free(heap);

    return 0;
}