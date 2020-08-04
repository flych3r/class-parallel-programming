#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX 100

int main(int argc, char *argv[]) {
    int rank, total_num, tag = 0, np;
    // int source = 0, destination = 1, numbers[MAX];
    int destination = 0, numbers[MAX];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (rank == destination) {
        for (int i = 0; i < np - 1; i++)
        {
            MPI_Recv(numbers, MAX, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &total_num);

            printf("Process %d received %d numbers. Source = %d, tag = %d\n", destination, total_num, status.MPI_SOURCE, status.MPI_TAG);
        }
    } else {
        srand(time(0));
        total_num = ((rand() / rank) / (float) RAND_MAX) * MAX;

        MPI_Send(numbers, total_num, MPI_INT, destination, tag, MPI_COMM_WORLD);

        printf("Process %d sent %d numbers to %d\n", rank, total_num, destination);
    }

    MPI_Finalize();

    return 0;
}