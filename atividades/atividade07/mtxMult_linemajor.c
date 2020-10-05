// Nota 1,0
#include <stdlib.h>
#include <stdio.h>

double *mtxMul_linemajor(double *c, double *a, double *b, int n)
{
    double aux;
    for (int i = 0; i < n; i++)
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

void print_matrix_linemajor(double *mtx, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%f ", mtx[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);

    double *a, *b, *c;
    a = (double *) malloc(sizeof(double) * n * n);
    b = (double *) malloc(sizeof(double) * n * n);
    c = (double *) malloc(sizeof(double) * n * n);

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            a[i * n + j] = i%2 ? 1.0 : 3.0;
            b[j * n + i] = i%2 ? 5.0 : 1.0;
            c[i * n + j] = 0.0;
        }
    }

    mtxMul_linemajor(c, a, b, n);
    print_matrix_linemajor(c, n);

    free(a);
    free(b);
    free(c);

    return 0;
}
