#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main (int argc , char *argv[]) {
    int max;
    int chunk;
    sscanf (argv[1], "%d", &max);
    sscanf (argv[2], "%d", &chunk);
    int ts = omp_get_max_threads ();
    int sums[ts];
    int remainder = max % ts;
        #pragma omp parallel
        {
            int t = omp_get_thread_num ();

            int lo = chunk * (t + 0) + 1;
            int hi;
            int step = chunk * ts;

            sums[t] = 0;
            for (int i = lo; i <= max; i+=step) {
                int j = 0;
                hi = j + i;
                while (j < chunk && hi <= max) {
                    sums[t] = sums[t] + hi;
                    j++;
                    hi = j + i;
                }
                hi--;
                if(hi <= max)
                    printf("thread: %d, lo: %d, hi: %d \n", t, i, hi);
            }
        }
    int sum = 0;
    for (int t = 0; t < ts; t++) sum = sum + sums[t];
    printf ("sum: %d\n", sum);
    return 0;
}
