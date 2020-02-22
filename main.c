#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

void printMatrix(float* matrix, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%5.0f ", matrix[i * m + j]);
        }
        printf("\n");
    }
    printf("-----\n");
}

int main() {
    const int n = 900;
    const int m = 1000;
    const int p = 1100;

    float* matrix1 = (float*)malloc(n * m * sizeof(float));
    float* matrix2 = (float*)malloc(m * p * sizeof(float));
    float* matrix21 = (float*)malloc(m * p * sizeof(float));
    float* matrix3 = (float*)malloc(n * p * sizeof(float));

    for(int i = 0; i < n * m; i++){
        matrix1[i] = i;
    }
    for(int i = 0; i < m * p; i++){
        matrix2[i] = i + n * m;
    }

    struct timeval stop, start;
    gettimeofday(&start, NULL);


//    printMatrix(matrix2, m, p);
//
//    for(int i = 0; i < m * p; i++){
//        int n1 = i / p;
//        int n2 = i % p;
//
//        int prev = n2 * m + n1;
//
////        for(int j = i + 1; j < p; j++){
//            if(n1 > n2){
//                float temp = matrix2[i];
//                matrix2[i] = matrix2[prev];
//                matrix2[prev] = temp;
//            }
////        }
//    }


    for(int i = 0; i < m; i++){
        for(int j = 0; j < p; j++){
            matrix21[j * m + i] = matrix2[i * p + j];
        }
    }

//    printMatrix(matrix2, p, m);

#pragma omp parallel
    {

#pragma omp for schedule(static, 1)
        for(int i2 = 0; i2 < n * p; i2++){
            int j = i2 % p;

            float tt = 0;
//            float* cur_matrix0 = matrix1 + i2 - j;
//            float* cur_matrix1 = matrix21 + m * j;
            for(int k = 0; k < m; k++){
//                tt += cur_matrix0[k] * cur_matrix1[k];
                int in2 = k + m * j;
                tt += matrix1[i2 - j + k] * matrix21[in2];
//                int in2 = k * p + j;
//                tt += matrix1[i2 - j + k] * matrix2[in2];
            }
            matrix3[i2] = tt;
        }
    }

    gettimeofday(&stop, NULL);
    long long int tt = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000;

    printf("someshit: %5.0f\n", matrix3[0]);

    printf("time: %lld ms\n", tt);

    return 0;
}
