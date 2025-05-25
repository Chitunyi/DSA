#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "1.h"

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

    // if (employee1->boss == employee2->boss && employee1->boss != NULL)
    // {
    //     return 3;
    // }
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