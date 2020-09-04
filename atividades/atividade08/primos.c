#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define N 1e6
#define ECHO 0

int primo (int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 0 || n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i <= (sqrt(n)+1); i+=2) {
        if (n%i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size;
    int root = 0, tag_chunk = 1, tag_result = 2;
    int *primes = (int *) malloc(sizeof(int) * N + 1);
    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // printf("%d\n", rank);
    if (rank == root) {
        int p = 0;
        int chunk[2];
        int chunk_size = N / (size - 1) + 1;
        for(int i = 1; i < N; i+=chunk_size)
        {
            if (i + chunk_size >= N) chunk_size = N - i;
            chunk[0] = i;
            chunk[1] = chunk_size;
            // printf("%d %d %d\n", p + 1, i, chunk_size);
            MPI_Isend(chunk, 2, MPI_INT, p + 1, tag_chunk, MPI_COMM_WORLD, &request);
            MPI_Irecv(primes + i, chunk_size, MPI_INT, p + 1, tag_result, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            p++;
            p %= size - 1;
        }
    } else {
        int chunk[2];
        MPI_Irecv(chunk, 2, MPI_INT, root, tag_chunk, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        int start = chunk[0];
        int chunk_size = chunk[1];
        int *primos = (int *) malloc(sizeof(int) * chunk_size + 1);
        for(int i = 0, j = start; i < chunk_size; i++, j++)
        {
            primos[i] = primo(j);
            // printf("%d %d\n", j, primos[i]);
        }
        MPI_Isend(primos, chunk_size, MPI_INT, root, tag_result, MPI_COMM_WORLD, &request);
    }

    if (rank == root && ECHO)
    {
        for (int i = 1; i <= N; i++)
        {
            if (primes[i]) printf("%d ", i);
        }
        printf("\n");
    }

    free(primes);
    MPI_Finalize();

    return 0;
}
