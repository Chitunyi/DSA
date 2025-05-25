#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char name[65];
    int count;
    char ingred[11][65];
} Food;

int findFood(char *name, Food foods[], int n){
    for (int i = 0; i < n; i++){
        if (strcmp(name, foods[i].name) == 0){
            return i;
            break;
        }
    }

    return -1;
}

int sortingred(const void *a, const void *b){
    return strcmp((char *)a, (char *)b);
}

void findcommon(Food *food1, Food *food2, char common[][65], int *numcommon){
    *numcommon = 0;

    for (int i = 0; i < food1->count; i++){
        for (int j = 0; j < food2->count; j++){
            if (strcmp(food1->ingred[i], food2->ingred[j]) == 0){
                strcpy(common[*numcommon], food1->ingred[i]);
                (*numcommon)++;
            }
        }
    }
}

int main(void)
{
    int n;
    scanf("%d", &n);
    Food foods[101];

    for (int i = 0; i < n; i++)
    {
        scanf("%s %d", foods[i].name, &foods[i].count);
        for (int j = 0; j < foods[i].count; j++)
        {
            scanf("%s", foods[i].ingred[j]);
        }
    }

    int q;
    scanf("%d", &q);

    while (q--){
        char food1[65], food2[65];
        scanf("%s %s", food1, food2);
        int index1 = findFood(food1, foods, n);
        int index2 = findFood(food2, foods, n);

        if (index1 == -1 || index2 == -1){
            printf("nothing\n");
            continue;
        }

        char common[11][65];
        int numcommen = 0;
        findcommon(&foods[index1], &foods[index2], common, &numcommen);

        if (numcommen == 0){
            printf("nothing\n");
        }
        else{
            qsort(common, numcommen, sizeof(common[0]), sortingred);

            for (int i = 0; i < numcommen; i++){
                printf("%s", common[i]);
                if (i < numcommen - 1)
                    printf(" ");
            }

            printf("\n");
        }
    }

    return 0;
}
