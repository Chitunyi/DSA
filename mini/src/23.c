// mini_g

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int coda = 0;
int N, B;

void printresult(int arr[], int n, int comp)
{
    printf("The array is ");
    for (int i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
        if (i < n - 1)
        {
            printf(" ");
        }
    }
    printf(" after %d swaps.\n", comp);
}

void budget(int arr[], int n, int B)
{
    if (coda == B)
    {
        printresult(arr, n, B);
        exit(0);
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (i < j)
    {
        while (i <= high && arr[i] <= pivot)
        {
            i++;
        }
        while (j >= low && arr[j] > pivot)
        {
            j--;
        }
        if (i < j)
        {
            swap(&arr[i], &arr[j]);
            coda++;
            if (coda == B)
            {
                printresult(arr, N, coda);
                exit(0);
            }
        }
    }

    if (low < j && arr[low] > arr[j])
    {
        coda++;
        swap(&arr[low], &arr[j]);
        if (coda == B)
        {
            printresult(arr, N, coda);
            exit(0);
        }
    }

    return j;
}

void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        if (coda == B)
        {
            budget(arr, N, B);
        }
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int main()
{
    scanf("%d %d", &N, &B);

    int *arr = (int *)malloc(sizeof(int) * (N + 1));

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &arr[i]);
    }

    if (B == 0)
    {
        printresult(arr, N, 0);
        free(arr);
        return 0;
    }

    quicksort(arr, 0, N - 1);
    printresult(arr, N, coda);
    free(arr);

    return 0;
}