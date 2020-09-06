#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

float *mtxMul_mpi(float *c, float *a, float *b, int n, int size)
{
    float aux;
    for (int i = 0; i < n / size; i++)
    {
        for (int j = 0; j < n; j++)
        {
            aux = 0.0;
            for (int k = 0; k < n; k++)
                aux += a[i * n + k] * b[k + n * j];
            c[i * n + j] = aux;
        }
    }
    return 0;
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

    a = (float *) malloc((NN / size) * sizeof(float));
    b = (float *) malloc(NN * sizeof(float));
    c = (float *) malloc(NN * sizeof(float));

    if (rank == 0) {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i * N + j < (NN / size))
                    a[i * N + j] = 1.0;
                b[j * N + i] = 1.0;
                c[i * N + j] = -1.0;
            }
        }
    }

    MPI_Bcast(a, NN / size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, NN, MPI_FLOAT, 0, MPI_COMM_WORLD);

    d = (float *) malloc((NN / size) * sizeof(float));
    mtxMul_mpi(d, a, b, N, size);

    if (rank == 0) {
        for (i = 0; i < N / size; i++)
        {
            for (j = 0; j < N; j++)
            {
                c[i * N + j] = d[i * N + j];
            }
        }
        for (source = 1; source < size; source++) {
            MPI_Recv(d, (NN / size), MPI_FLOAT, source, 0, MPI_COMM_WORLD, &status);
            int k = source * N * (N / size);
            for (i = 0; i < N / size; i++)
            {
                for (j = 0; j < N; j++)
                {
                    c[i * N + j + k] = d[i * N + j];
                }
            }
        }
    }
    else
    {
        MPI_Send(d, (NN / size), MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime() - start;
    MPI_Reduce(&finish, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("With N = %d.\n", N);
        // print_matrix_linemajor(c, N, 1);
        printf("Elapsed time for %d: %lf seconds\n", size, maxtime);
    }

    free(a);
    free(b);
    free(c);
    free(d);

    MPI_Finalize();
    return 0;
}
