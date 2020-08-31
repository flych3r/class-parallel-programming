#include <stdio.h>
#include <stdlib.h>

int main()
{
    int size = 4;
    int **input_array = (int **)malloc(size * sizeof(int *));
    int **output_array = (int **)malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++)
    {
        input_array[i] = (int *) malloc(2 * sizeof(int));
        output_array[i] = (int *) malloc(2 * sizeof(int));
    }

    for (int i = 0; i < size; i++)
    {
        input_array[i][0] = 1;
        input_array[i][1] = -1;
    }

    return 0;
}
