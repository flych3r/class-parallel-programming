// Cálculo das Médias
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int i, j, rank, size;
    int **input_array;
    int **output_array;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    input_array = (int **)malloc(size * sizeof(int *));
    output_array = (int **)malloc(size * sizeof(int *));

    for (i = 0; i < size; i++)
    {
        input_array[i] = (int *) malloc(2 * sizeof(int));
        output_array[i] = (int *) malloc(2 * sizeof(int));
    }

    for (i = 0; i < size; i++)
    {
        input_array[i][0] = rank;
        input_array[i][1] = -rank;
    }

    printf("Input for process %d\n", rank);
    for (i = 0; i < size; i++)
        printf("[%d %d]", input_array[i][0], input_array[i][1]);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Alltoall(input_array, 2, MPI_INT, output_array, 2, MPI_INT, MPI_COMM_WORLD);

    printf("Output for process %d\n", rank);
    for (i = 0; i < size; i++)
        printf("[%d %d]", output_array[i][0], output_array[i][1]);
    printf("\n");

    // free(input_array[0]);
    // free(input_array);
    // free(output_array[0]);
    // free(output_array);
    MPI_Finalize();
    return 0;
}
