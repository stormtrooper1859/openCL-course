#include <stdio.h>
#include <omp.h>

int main() {
    int num;
    scanf("%d", &num);

    int answ2 = 1;

    int answ[100];
    for(int i = 0; i<100; i++) answ[i] = 0;

#pragma omp parallel
    {
        int thread_number = omp_get_thread_num();
        int number_of_threads = omp_get_num_threads();
        int low, high;

        low = 3 + thread_number * 2;
        high = num;
        int step = number_of_threads * 2;

        for (int i = low; i <= high; i += step) {
            int prime = 1;
            for (int t = 3; t * t <= i; t += 2) {
                if (i % t == 0) {
                    prime = 0;
                    break;
                }
            }
            if (prime) {
                answ[thread_number]++;
            }
        }
    }

    for(int i = 0; i<100; i++) answ2+=answ[i];


    printf("%d\n", answ2);

    return 0;
}
