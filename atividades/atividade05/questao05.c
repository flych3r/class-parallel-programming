#include <stdio.h>
#include <mpi.h>

#define ECHO 0

int main(int argc, char *argv[]) {
    int rank, size, source, destination, N, tag=0;

    sscanf(argv[1], "%d", &N);

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    source = (rank + size - 1) % size;
    destination = (rank + 1) % size;

    while (1) {
        if (rank == 0) {
            MPI_Send(&N, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
            if (ECHO) printf("%d (%d) -> %d\n", rank, N, destination);
            if (N == 0)
                break;
            MPI_Recv(&N, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            N--;
        } else {
            MPI_Recv(&N, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(&N, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
            if (ECHO) printf("%d (%d) -> %d\n", rank, N, destination);
            if (N == 0)
                break;
        }
    }

    MPI_Finalize();

    return 0;
}
