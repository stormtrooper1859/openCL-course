#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    const int n = 500;
    const int m = 600;
    const int p = 700;

    float* matrix1 = (float*)malloc(n * m * sizeof(float));
    float* matrix2 = (float*)malloc(m * p * sizeof(float));
    float* matrix3 = (float*)malloc(n * p * sizeof(float));

    for(int i = 0; i < n * m; i++){
        matrix1[i] = i;
    }
    for(int i = 0; i < m * p; i++){
        matrix2[i] = i + n * m;
    }

    struct timeval stop, start;
    gettimeofday(&start, NULL);

#pragma omp parallel
    {

#pragma omp for
        for(int i2 = 0; i2 < n * p; i2++){
            int j = i2 % n;

            float tt = 0;
            for(int k = 0; k < m; k++){
                tt += matrix1[i2 - j + k] * matrix2[k * m + j];
            }
            matrix3[i2] = tt;
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
