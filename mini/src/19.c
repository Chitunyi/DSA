// mini_c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int N;
int target;
int pos;
int found = 0;

void printreresult(int pos)
{
    if (!found)
    {
        printf("Not found.\n");
    }
    else
    {
        printf("Found at index %d.\n", pos + 1);
    }
}

void binarysearch(int arr[], int l, int r)
{
    printf("Searching %d in range [%d, %d].\n", target, l + 1, r + 1);

    if (l > r)
    {
        return;
    }

    int mid = (l + r) / 2;

    if (arr[mid] == target)
    {
        found = 1;
        pos = mid;
        return;
    }
    else if (arr[mid] > target)
    {
        binarysearch(arr, l, mid - 1);
    }
    else
    {
        binarysearch(arr, mid + 1, r);
    }

    return;
}

int main()
{
    scanf("%d %d", &N, &target);

    int arr[N];
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &arr[i]);
    }

    binarysearch(arr, 0, N - 1);
    printreresult(pos);

    return 0;
}