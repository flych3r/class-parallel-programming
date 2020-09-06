#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int numprocs, org_rank, new_size, new_rank;
    MPI_Comm mult2_comm, mult3_comm, mult2_3_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &org_rank);

    int m2 = org_rank % 2 == 0;
    int m3 = org_rank % 3 == 0;
    int m2_3 = m2 && m3;

    MPI_Comm_split(MPI_COMM_WORLD, m2 ? 0 : MPI_UNDEFINED, 0, &mult2_comm);
    MPI_Comm_split(MPI_COMM_WORLD, m3 ? 0 : MPI_UNDEFINED, 0, &mult3_comm);
    MPI_Comm_split(MPI_COMM_WORLD, m2_3 ? 0 : MPI_UNDEFINED, 0, &mult2_3_comm);

    if (mult2_comm != MPI_COMM_NULL) {
        MPI_Comm_size(mult2_comm, &new_size);
        MPI_Comm_rank(mult2_comm, &new_rank);
        printf("'MPI_COMM_WORLD' processo rank/quantidade %d/%d tem rank/quantidade %d/%d em 'mult2_comm '\n", org_rank, numprocs, new_rank, new_size);
        MPI_Comm_free(&mult2_comm);
    }

    if (mult3_comm != MPI_COMM_NULL) {
        MPI_Comm_size(mult3_comm, &new_size);
        MPI_Comm_rank(mult3_comm, &new_rank);
        printf("'MPI_COMM_WORLD' processo rank/quantidade %d/%d tem rank/quantidade %d/%d em 'mult3_comm '\n", org_rank, numprocs, new_rank, new_size);
        MPI_Comm_free(&mult3_comm);
    }

    if (mult2_3_comm != MPI_COMM_NULL) {
        MPI_Comm_size(mult2_3_comm, &new_size);
        MPI_Comm_rank(mult2_3_comm, &new_rank);
        printf("'MPI_COMM_WORLD' processo rank/quantidade %d/%d tem rank/quantidade %d/%d em 'mult2_3_comm '\n", org_rank, numprocs, new_rank, new_size);
        MPI_Comm_free(&mult2_3_comm);
    }

    MPI_Finalize();
    return 0;
}