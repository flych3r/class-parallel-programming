// Correção: OK. 1,0 ponto.
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main (int argc , char *argv[]) {
    int max;
    sscanf (argv[1], "%d", &max);
    int ts = omp_get_max_threads ();
    int remainder = max % ts;
        int sums[ts];
        #pragma omp parallel
        {
            int t = omp_get_thread_num ();

            int offset_lo = (remainder * (t + 0)) / ts;
            int offset_hi = (remainder * (t + 1)) / ts;
            int lo = (max / ts) * (t + 0) + 1 + offset_lo;
            int hi = (max / ts) * (t + 1) + 0 + offset_hi;

            sums[t] = 0;
            printf("thread: %d, lo: %d, hi: %d \n", t, lo, hi);
            for (int i = lo; i <= hi; i++)
                sums[t] = sums[t] + i;
        }
    int sum = 0;
    for (int t = 0; t < ts; t++) sum = sum + sums[t];
    printf ("sum: %d\n", sum);
    return 0;
}
