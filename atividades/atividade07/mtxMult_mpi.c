// Nota 1,0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void mtxMul_mpi(int rank, float *c, float *a, float *b, int n, int size)
{
    float aux;
    int chunk = n / size;
    for (int ig = chunk * rank, il = 0; ig < chunk * (rank + 1); ig++, il++)
    {
        for (int j = 0; j < n; j++)
        {
            aux = 0.0;
            for (int k = 0; k < n; k++)
                aux += a[ig * n + k] * b[k + n * j];
            c[il * n + j] = aux;
        }
    }
}

void print_matrix_linemajor(float *mtx, int n, int size)
{
    for(int i = 0; i < n / size; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%f ", mtx[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int i, j, source, rank, size;
    float *a, *b, *c, *d, *e;
    int N, NN;
    double start, finish, maxtime;

    MPI_Init(&argc, &argv);
    MPI_Status status;

    start = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sscanf (argv[1], "%d", &N);
    NN = N * N;

    a = (float *) malloc(NN * sizeof(float));
    b = (float *) malloc(NN * sizeof(float));
    c = (float *) malloc(NN * sizeof(float));
    d = (float *) malloc((NN / size) * sizeof(float));

    if (rank == 0) {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                a[i * N + j] = i%2 ? 1.0 : 3.0 ;
                b[j * N + i] = i%2 ? 5.0 : 1.0;
                c[i * N + j] = 0.0;
            }
        }
    }

    // Era para distribuir A por linha.
    MPI_Bcast(a, NN, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, NN, MPI_FLOAT, 0, MPI_COMM_WORLD);

    mtxMul_mpi(rank, d, a, b, N, size);

    MPI_Gather(
        d, NN / size, MPI_FLOAT,
        c + ((NN / size) * rank), NN / size, MPI_FLOAT, 0,
        MPI_COMM_WORLD
    );

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime() - start;
    MPI_Reduce(&finish, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // printf("With N = %d.\n", N);
        print_matrix_linemajor(c, N, 1);
        // printf("Elapsed time for %d: %lf seconds\n", size, maxtime);
    }

    free(a);
    free(b);
    free(c);
    free(d);

    MPI_Finalize();
    return 0;
}
