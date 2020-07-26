#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main (int argc , char *argv[]) {
    int max;
    int c = 4;
    sscanf (argv[1], "%d", &max);
    int ts = omp_get_max_threads ();
    int sum = 0;
    {
        #pragma omp parallel for reduction(+:sum) schedule(static,c)
        for (int i = 1; i <= max; i++){
            printf ("thread: %d, iteration: %2d\n", omp_get_thread_num(), i);
            sum = sum + i;
        }
    }
    printf ("%d\n", sum);
    return 0;
}
