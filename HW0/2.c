#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int N;

int putin(uint64_t *data, int id)
{
    uint8_t arr[8];

    for (int i = 0; i < 8; i++)
    {
        arr[i] = (*data >> (56 - 8 * i)) & 0xFF;
    }

    int found = -1;
    for (int i = 0; i < 8; i++)
    {
        if (arr[i] == (uint8_t)id)
        {
            found = i;
            break;
        }
    }

    if (found != -1)
    {
        for (int i = found; i < 7; i++)
        {
            arr[i] = arr[i + 1];
        }
        arr[7] = (uint8_t)id;
    }
    else
    {
        for (int i = 0; i < 7; i++)
        {
            arr[i] = arr[i + 1];
        }
        arr[7] = (uint8_t)id;
    }

    uint64_t newdata = 0;
    for (int i = 0; i < 8; i++)
    {
        newdata |= ((uint64_t)arr[i]) << (56 - 8 * i);
    }
    *data = newdata;
    return 0;
}

void printresult(uint64_t data, int k)
{
    if (k == 1)
    {
        printf("%llu\n", data);
    }
    else if (k == 2)
    {
        for (int j = 7; j >= 0; j--)
        {
            uint8_t byte = (data >> (8 * j)) & 0xFF;
            printf("%u ", byte);
        }
        printf("\n");
    }
}

int main()
{
    scanf("%d", &N);

    int k = 0, id = 0;
    int outputype = 0;
    uint64_t data = 0;

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &k);
        if (k == 1)
        {
            scanf("%d", &id);
            putin(&data, id);
        }
        else
        {
            scanf("%d", &outputype);
            printresult(data, outputype);
        }
    }

    return 0;
}