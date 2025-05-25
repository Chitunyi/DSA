#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 9

int matrix1[N][N];
int matrix2[N][N];
int found = 0;

void printresult()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
}

int check(int i, int j)
{
    int sum = 0;
    for (int p = -1; p <= 1; p++)
    {
        for (int q = -1; q <= 1; q++)
        {
            if (i + p >= 0 && i + p < N && j + q >= 0 && j + q < N)
            {
                sum += matrix2[i + p][j + q];
            }
        }
    }

    return sum == matrix1[i][j];
}

int checkpart()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int finished = 1, sum = 0, unknown = 0;
            for (int p = -1; p <= 1; p++)
            {
                for (int q = -1; q <= 1; q++)
                {
                    if (i + p >= 0 && i + p < N && j + q >= 0 && j + q < N)
                    {
                        if (matrix2[i + p][j + q] != 0 && matrix2[i + p][j + q] != 1)
                        {
                            finished = 0;
                            unknown++;
                        }
                        else
                        {
                            sum += matrix2[i + p][j + q];
                        }
                    }
                }
            }
            if (finished && sum != matrix1[i][j])
            {
                return 0;
            }
            if (finished && (sum > matrix1[i][j] || sum + unknown < matrix1[i][j]))
            {
                return 0;
            }
        }
    }

    return 1;
}

void pilay(int coor)
{
    if (coor == N * N)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (!check(i, j))
                    return;
            }
        }

        found = 1;
        printresult();
        return;
    }

    int i = coor / N;
    int j = coor % N;

    matrix2[i][j] = -1;
    for (int k = 0; k <= 1; k++)
    {
        matrix2[i][j] = k;
        if (checkpart())
        {
            pilay(coor + 1);
            if (found)
            {
                return;
            }
        }
    }
    matrix2[i][j] = -1;
}

int main()
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            scanf("%d", &matrix1[i][j]);
            matrix2[i][j] = -1;
        }
    }

    pilay(0);

    if (!found)
    {
        printf("no solution\n");
    }

    return 0;
}
