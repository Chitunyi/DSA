#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int n, m;
int u[191], v[191];
int foundset = 0;

int checkset(int people[], int size)
{
    for (int i = 0; i < m; i++)
    {
        int check = 0;
        for (int j = 0; j < size; j++)
        {
            if (people[j] == u[i] || people[j] == v[i])
            {
                check = 1;
                break;
            }
        }

        if (check == 0)
        {
            return 0;
        }
    }

    return 1;
}

void search(int a, int b, int p, int people[])
{
    if (foundset == 1)
    {
        return;
    }

    if (b == p)
    {
        if (checkset(people, p) == 1)
        {
            for (int i = 0; i < p; i++)
            {
                printf("%d\n", people[i]);
            }
            foundset = 1;
        }
    }

    for (int j = a; j < n; j++)
    {
        people[b] = j;
        search(j + 1, b + 1, p, people);

        if (foundset == 1)
        {
            return;
        }
    }
}

int main()
{

    if (scanf("%d %d", &n, &m) != 2)
    {
        return 1;
    }

    if (m == 0)
    {
        return 0;
    }

    for (int i = 0; i < m; i++)
    {
        scanf("%d %d", &u[i], &v[i]);
    }

    int people[21];
    for (int j = 1; j <= n; j++)
    {
        search(0, 0, j, people);

        if (foundset == 1)
        {
            break;
        }
    }

    return 0;
}
