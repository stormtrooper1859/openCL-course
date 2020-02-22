#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <OpenCL/opencl.h>

void printMatrix(float* matrix, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%5.0f ", matrix[i * m + j]);
        }
        printf("\n");
    }
    printf("-----\n");
}


const size_t maxsize = 1000;

int main() {
    cl_uint platform_nums = -1;
    int response01 = clGetPlatformIDs(0, NULL, &platform_nums);
    cl_platform_id* platforms = (cl_platform_id*)malloc(platform_nums * sizeof(cl_platform_id));
    int response1 = clGetPlatformIDs(maxsize, platforms, &platform_nums);
    printf("Platforms: %d %d\n", response1, platform_nums);

    if (platform_nums <= 0) {
        printf("Platforms not founds\n");
        return 1;
    }

    cl_uint deviceNums = -1;
    int response02 = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceNums);
    cl_device_id* deviceIds = (cl_device_id*)malloc(deviceNums * sizeof(cl_device_id));
    int response2 = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, maxsize, deviceIds, &deviceNums);
    printf("Devices: %d %d\n", response2, deviceNums);

    if (deviceNums <= 0) {
        printf("Devices not founds\n");
        return 1;
    }

    for (int i = 0; i < deviceNums; i++) {

        size_t clDeviceNameSize = -1;
        int response03 = clGetDeviceInfo(deviceIds[i], CL_DEVICE_NAME, 0, NULL, &clDeviceNameSize);
        char* clDeviceName = (char*)malloc(clDeviceNameSize * sizeof(char));
        int response3 = clGetDeviceInfo(deviceIds[i], CL_DEVICE_NAME, maxsize, clDeviceName, &clDeviceNameSize);
        printf("Device %d: %d %s\n", i, response3, clDeviceName);
    }



    return 0;

    const int n = 10;
    const int m = 20;
    const int p = 30;

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
