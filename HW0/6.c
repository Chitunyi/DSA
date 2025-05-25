#include "6.h"

void prepare_array(int buffer[], int *array[], int row, int column[])
{
    int ptr = 0;
    for (int i = 0; i < row; i++)
    {
        array[i] = buffer + ptr;
        ptr += column[i];
    }
}
