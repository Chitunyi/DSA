#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "1.h"

typedef struct me
{
    int id;
    char first_name[32];
    char last_name[32];
    int boss_id;
} employee;
void readName(employee *e)
{
    scanf("%s %s", e->first_name, e->last_name);
}
int nameToIndex(employee *e, employee A[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!strcmp(A[i].first_name, e->first_name) && !strcmp(A[i].last_name, e->last_name))
            return i;
    }
    return -1;
}

int relation(Employee *employee1, Employee *employee2)
{

    int steps = 0;

    Employee *p = employee1->boss;
    while (p != NULL && steps < 33)
    {
        if (p == employee2)
        {
            return 1;
        }
        p = p->boss;
        steps++;
    }

    steps = 0;
    p = employee2->boss;
    while (p != NULL && steps < 33)
    {
        if (p == employee1)
        {
            return 2;
        }
        p = p->boss;
        steps++;
    }

    Employee *p1 = employee1->boss;
    int steps1 = 0;
    while (p1 != NULL && steps1 < 33)
    {
        Employee *p2 = employee2->boss;
        int steps2 = 0;
        while (p2 != NULL && steps2 < 33)
        {
            if (p1 == p2)
            {
                return 3;
            }
            p2 = p2->boss;
            steps2++;
        }
        p1 = p1->boss;
        steps1++;
    }

    return 4;
}

int main()
{
    int n, m;
    employee A[32];
    Employee B[32];
    const char out[4][32] = {"subordinate", "supervisor", "colleague", "unrelated"};
    while (scanf("%d", &n) == 1)
    {
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &A[i].id);
            readName(&A[i]);
            scanf("%d", &A[i].boss_id);
        }
        for (int i = 0; i < n; i++)
        {
            strcpy(B[i].first_name, A[i].first_name);
            strcpy(B[i].last_name, A[i].last_name);
            B[i].boss = NULL;
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (A[i].boss_id == A[j].id)
                    B[i].boss = &B[j];
            }
        }
        scanf("%d", &m);
        employee x, y;
        for (int i = 0; i < m; i++)
        {
            readName(&x);
            readName(&y);
            int ix = nameToIndex(&x, A, n), iy = nameToIndex(&y, A, n);
            assert(ix != -1);
            assert(iy != -1);
            printf("%d\n", relation(&B[ix], &B[iy]));
        }
    }
    return 0;
}