// Cálculo das Médias
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define NELEM 1024 * 1024

int main(int argc, char *argv[]) {
    int i, rank, size;
    float *random_numbers;
    float avg, dif_quad_local = 0, sum_local = 0;
    float sum_global, dif_quad_global, std_dev;
    int root = 0, tag = 0;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cria um vetor de números aleatórios em cada processo.
    srand(MPI_Wtime() * rank);
    random_numbers = (float *) malloc(sizeof(float) * NELEM);
    for(i = 0; i < NELEM; i++)
        random_numbers[i] = (rand()/(float)RAND_MAX);

    for(i = 0; i < NELEM; i++)
        sum_local += random_numbers[i];

    printf("Process %d: local sum %f, local average %f .\n", rank, sum_local, sum_local/NELEM);

    // Média Global
    if (rank == root) {
        sum_global = sum_local;
        for (int source = 1; source < size; source++) {
            MPI_Recv(&sum_local, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
            sum_global += sum_local;
        }
        avg = sum_global / (NELEM * size);
        for (int source = 1; source < size; source++)
            MPI_Send(&avg, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD);
    } else {
        MPI_Send(&sum_local, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD);
        MPI_Recv(&avg, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD, &status);
    }

    // Soma local do Quadrado das diferenças
    for(i = 0; i < NELEM; i++)
        dif_quad_local += (random_numbers[i]- avg) * (random_numbers[i] - avg);

    // Soma Global do Quadrado das diferenças
    MPI_Reduce(&dif_quad_local, &dif_quad_global, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std_dev = sqrt(dif_quad_global/(NELEM * size));
        printf("Average %f, Standard Deviation %f\n", avg, std_dev);
    }

    free(random_numbers);
    MPI_Finalize();
    return 0;
}