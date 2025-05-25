// mini_a

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int coda = 0;
int B;
int N;

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
    printf(" after %d comparisons.", comp);
}

void budget(int arr[], int n)
{
    if (coda == B)
    {
        printresult(arr, n, B);
        exit(0);
    }
}

int getminindex(int arr[], int m, int n)
{
    int min = arr[m];
    int minindex = m;

    for (int i = m + 1; i < n; i++)
    {
        budget(arr, n);
        coda++;
        if (arr[i] < min)
        {
            min = arr[i];
            minindex = i;
        }
    }

    return minindex;
}

void selectionsort(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int minindex = getminindex(arr, i, n);
        if (minindex != i)
        {
            int temp = arr[i];
            arr[i] = arr[minindex];
            arr[minindex] = temp;
        }
    }
}

int main()
{
    scanf("%d %d", &N, &B);
    int *arr = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &arr[i]);
    }

    selectionsort(arr, N);

    printresult(arr, N, coda);

    return 0;
}