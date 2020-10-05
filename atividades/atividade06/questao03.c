// Nota 0,3
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    int i, rank, size;
    float *v1, *v2;
    float e1, e2;
    int N;
    double local_scalar_prod, global_scalar_prod;
    double start, finish, maxtime;

    MPI_Init(&argc, &argv);
    start = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sscanf (argv[1], "%f", &e1);
    sscanf (argv[2], "%f", &e2);
    sscanf (argv[3], "%d", &N);
    N = N / size;

    v1 = (float *) malloc(N * sizeof(float));
    v2 = (float *) malloc(N * sizeof(float));

    if (rank == 0) {
        for (size_t i = 0; i < N; i++)
        {
            v1[i] = e1;
            v2[i] = e2;
        }

    }

    // Você está copiando os vetores por completo? Não seria uma Scatter?
    MPI_Bcast(v1, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(v2, N, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cada processo calcula o valor do produto escalar completo?
    local_scalar_prod = scalarProduct(v1, v2, N);

    MPI_Reduce(
        &local_scalar_prod, &global_scalar_prod,
         1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD
    );

    free(v1);
    free(v2);

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime() - start;
    MPI_Reduce(&finish, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("With N = %d.\n", N * size);
        printf("Scalar product = %lf \n", global_scalar_prod);
        printf("Elapsed time for %d: %lf seconds\n", size, maxtime);
    }

    MPI_Finalize();
    return 0;
}
