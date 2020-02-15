#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
//    int num;
//    scanf("%d", &num);

    const int n = 400;
    const int m = 400;
    const int p = 400;

    float* matrix1 = (float*)malloc(n * m * sizeof(float));
    float* matrix2 = (float*)malloc(m * p * sizeof(float));
    float* matrix3 = (float*)malloc(n * p * sizeof(float));

//    int matrix1[n][m] = {{1, 2},{3, 4}};
//    int matrix2[m][p] = {{5, 6},{7, 8}};
//    int matrix3[n][p];

    for(int i = 0; i < n * m; i++){
        matrix1[i] = i;
    }
    for(int i = 0; i < m * p; i++){
        matrix2[i] = i + n * m;
    }

    struct timeval stop, start;
    gettimeofday(&start, NULL);

//    int answ2 = 1;

//    int answ[100];
//    for(int i = 0; i<100; i++) answ[i] = 0;

#pragma omp parallel
    {
//        int thread_number = omp_get_thread_num();
//        int number_of_threads = omp_get_num_threads();

#pragma omp for schedule(static, 1)
        for(int i = 0; i < n; i++){
            for(int j = 0; j < p; j++){

                int tt = 0;
                for(int k = 0; k < m; k++){
                    tt += matrix1[i * n + k] * matrix2[k * m + j];
                }
                matrix3[i * n + j] = tt;
            }
        }
    }

    gettimeofday(&stop, NULL);
    long long int tt = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000;

//    for(int i = 0; i < n; i++){
//        for(int j = 0; j < p; j++){
//            printf("%d ", matrix3[i * n + j]);
//        }
//        printf("\n");
//    }

    printf("time: %lld ms\n", tt);

    return 0;
}
