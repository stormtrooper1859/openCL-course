#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

int main() {
    int num;
    scanf("%d", &num);

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int answ2 = 1;

    int answ[100];
    for(int i = 0; i<100; i++) answ[i] = 0;

#pragma omp parallel
    {
        int answ3 = 0;
        int thread_number = omp_get_thread_num();
        int number_of_threads = omp_get_num_threads();
        int low, high;

        low = 3 + thread_number * 2;
        high = num;
        int step = number_of_threads * 2;

#pragma omp for schedule(dynamic, 10)
        for (int i = 3; i <= high; i += 2) {
//        for (int i = low; i <= high; i += step) {
            int prime = 1;
            for (int t = 3; t * t <= i; t += 2) {
                if (i % t == 0) {
                    prime = 0;
                    break;
                }
            }
            if (prime) {
//#pragma omp atomic
                answ3++;
            }
        }

#pragma omp critical
        answ2+=answ3;
    }

    for(int i = 0; i<100; i++) answ2+=answ[i];

    printf("%d\n", answ2);

    gettimeofday(&stop, NULL);
    long long int tt = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000;
    printf("time: %lld ms\n", tt);

    return 0;
}
