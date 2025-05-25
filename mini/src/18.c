// mini_b

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
    printf(" after %d back-moves.\n", comp);
}

void budget(int arr[], int n)
{
    if (coda == B)
    {
        printresult(arr, n, B);
        exit(0);
    }
}

void insertbigger(int arr[], int m)
{
    int data = arr[m];
    int i = m - 1;
    while (i >= 0 && arr[i] < data)
    {
        coda++;
        arr[i + 1] = arr[i];
        i--;
        if (coda == B)
        {
            break;
        }
    }
    arr[i + 1] = data;
    if (coda == B)
    {
        budget(arr, N);
    }
}

void insertionsort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        insertbigger(arr, i);
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

    if (B == 0)
    {
        printresult(arr, N, 0);
        free(arr);
        return 0;
    }

    insertionsort(arr, N);

    printresult(arr, N, coda);
    free(arr);
    return 0;
}