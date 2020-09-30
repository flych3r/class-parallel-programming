// Nota: 0,3. A questão pede MPI_Send/MPI_Recv
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

double scalarProduct(float * A, float * B, size_t n) {
    double return_val = 0;

    for (size_t i = 0; i < n; i++)
    {
        return_val += A[i] * B[i];
    }

    return return_val;
}

int main(int argc, char *argv[]) {
    int rank, size, N, tag=0;
    double start, finish, maxtime, global_product;
    float valorA, valorB;

    sscanf(argv[1], "%d", &N);

    if (argc < 4) {
        srand(time(0));
        valorA = (rand() % 100);
        valorB = (rand() % 100);
    } else {
        sscanf(argv[2], "%f", &valorA);
        sscanf(argv[3], "%f", &valorB);
    }

    MPI_Status status;

    MPI_Init(&argc, &argv);
    start = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double taskprod;
    int n = (N / size);

    if (rank == 0) {
        float * A = (float *) malloc(n * sizeof(float));
        float * B = (float *) malloc(n * sizeof(float));

        for (size_t i = 0; i < n; i++)
        {
            A[i] = valorA;
            B[i] = valorB;
        }

        taskprod = scalarProduct(A, B, n);

        global_product += taskprod;
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&taskprod, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            global_product += taskprod;
        }

    } else {
        float * A = (float *) malloc(n * sizeof(float));
        float * B = (float *) malloc(n * sizeof(float));

        for (size_t i = 0; i < n; i++)
        {
            A[i] = valorA;
            B[i] = valorB;
        }

        taskprod = scalarProduct(A, B, n);

        MPI_Send(&taskprod, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }


    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime() - start;

    // A questão pede o uso de MPI_Send/MPI_Recv
    MPI_Reduce(&finish, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("With N = %d.\n", N);
        printf("Scalar product = %lf \n", global_product);
        printf("Elapsed time for %d: %lf seconds\n", size, maxtime);
    }

    MPI_Finalize();

    return 0;
}
