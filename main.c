#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <OpenCL/opencl.h>

void printMatrix(float *matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%5.0f ", matrix[i * m + j]);
        }
        printf("\n");
    }
    printf("-----\n");
}


const size_t maxsize = 1000;
//char program[50000];

int main() {
    cl_int errCode = -1;
    cl_uint platformNums;
    errCode = clGetPlatformIDs(0, NULL, &platformNums);
    cl_platform_id *platforms = (cl_platform_id *) malloc(platformNums * sizeof(cl_platform_id));
    errCode = clGetPlatformIDs(maxsize, platforms, &platformNums);
    printf("Platforms: %d %d\n", errCode, platformNums);

    if (platformNums <= 0) {
        printf("Platforms not founds\n");
        return 1;
    }

    size_t clPlatformNameSize;
    errCode = clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, 0, NULL, &clPlatformNameSize);
    char *clPlatformName = (char *) malloc(clPlatformNameSize * sizeof(char));
    errCode = clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, maxsize, clPlatformName, &clPlatformNameSize);
    printf("Platform %d: %d %s\n", 0, errCode, clPlatformName);

    cl_uint deviceNums;
    errCode = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceNums);
    cl_device_id *deviceIds = (cl_device_id *) malloc(deviceNums * sizeof(cl_device_id));
    errCode = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, maxsize, deviceIds, &deviceNums);
    printf("Devices: %d %d\n", errCode, deviceNums);

    if (deviceNums <= 0) {
        printf("Devices not founds\n");
        return 1;
    }

    for (int i = 0; i < deviceNums; i++) {
        size_t clDeviceNameSize = -1;
        errCode = clGetDeviceInfo(deviceIds[i], CL_DEVICE_NAME, 0, NULL, &clDeviceNameSize);
        char *clDeviceName = (char *) malloc(clDeviceNameSize * sizeof(char));
        errCode = clGetDeviceInfo(deviceIds[i], CL_DEVICE_NAME, maxsize, clDeviceName, &clDeviceNameSize);
        printf("Device %d: %d %s\n", i, errCode, clDeviceName);
    }

//    size_t deviceNum = 1;

    cl_context context = clCreateContext(0, 1, deviceIds, NULL, NULL, &errCode);
    printf("Context errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceIds[0], CL_QUEUE_PROFILING_ENABLE, &errCode);
    printf("CommandQueue errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    // TODO сделать нормальное чтение
    FILE *fp = fopen("./program.cl", "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *program = (char *) malloc(fileSize * sizeof(char));
    char ch = 0;
    int index = 0;
    while ((ch = fgetc(fp)) != EOF) {
        program[index++] = ch;
    }
    program[index] = 0;
    index++;
    fclose(fp);
//    printf("%s\n", program);


    cl_program clProg = clCreateProgramWithSource(context, 1, &program, &index, &errCode);
    printf("CreateProgramWithSource errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    errCode = clBuildProgram(clProg, 1, deviceIds, NULL, NULL, NULL);
    printf("BuildProgram errCode %d\n", errCode);
//    if (errCode != 0) {
//        return 1;
//    }

    size_t clBuildInfoLogSize = -1;
    clGetProgramBuildInfo(clProg, deviceIds[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &clBuildInfoLogSize);
    char *buildInfoLog = (char *) malloc(clBuildInfoLogSize * sizeof(char));
    clGetProgramBuildInfo(clProg, deviceIds[0], CL_PROGRAM_BUILD_LOG, clBuildInfoLogSize, buildInfoLog,
                          &clBuildInfoLogSize);
    printf("Compiler response: %s\n", buildInfoLog);


    const char add[] = "add";
    errCode = 0;
    cl_kernel kernel = clCreateKernel(clProg, add, &errCode);
    printf("Kernel errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }






    const size_t arrLen = 100000;





    cl_mem buffer1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * arrLen, NULL, &errCode);
    printf("Buffer1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    cl_mem buffer2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * arrLen, NULL, &errCode);
    printf("Buffer2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    cl_mem buffer3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * arrLen, NULL, &errCode);
    printf("Buffer3 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

//    int a = 1;
//    int b = 2;


    int *a = (int *) malloc(arrLen * sizeof(int));
    int *b = (int *) malloc(arrLen * sizeof(int));
    int *c = (int *) malloc(arrLen * sizeof(int));

    for (int i = 0; i < arrLen; i++) {
        a[i] = i;
        b[i] = arrLen + i;
        c[i] = 0;
    }



    errCode = clEnqueueWriteBuffer(commandQueue, buffer1, 0, 0, sizeof(int) * arrLen, a, 0, 0, 0);
    printf("Enqueue buffer1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clEnqueueWriteBuffer(commandQueue, buffer2, 0, 0, sizeof(int) * arrLen, b, 0, 0, 0);
    printf("Enqueue buffer2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    errCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer1);
    printf("Set arg1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer2);
    printf("Set arg2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer3);
    printf("Set arg3 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    cl_event event;
//    size_t aaa = arrLen;
    size_t one = 1;
    errCode = clEnqueueNDRangeKernel(commandQueue, kernel, 1, 0, &arrLen, 0, 0, 0, &event);
    printf("clEnqueueNDRangeKernel errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    errCode = clEnqueueReadBuffer(commandQueue, buffer3, 1, 0, sizeof(int) * arrLen, c, 0, 0, 0);
    printf("Enqueue read buffer errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    int rr = 0;
    for (int i = 0; i < arrLen; i++){
        rr += c[i];
    }
    printf("Result: %d\n", rr);



    cl_ulong begin;
    cl_ulong end;
    size_t tmp;
    errCode = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(begin), &begin, &tmp);
    printf("clGetEventProfilingInfo1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, &tmp);
    printf("clGetEventProfilingInfo2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    printf("Time: %lld\n", end - begin);

    return 0;


    const int n = 10;
    const int m = 20;
    const int p = 30;

    float *matrix1 = (float *) malloc(n * m * sizeof(float));
    float *matrix2 = (float *) malloc(m * p * sizeof(float));
    float *matrix21 = (float *) malloc(m * p * sizeof(float));
    float *matrix3 = (float *) malloc(n * p * sizeof(float));

    for (int i = 0; i < n * m; i++) {
        matrix1[i] = i;
    }
    for (int i = 0; i < m * p; i++) {
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


    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            matrix21[j * m + i] = matrix2[i * p + j];
        }
    }

//    printMatrix(matrix2, p, m);

#pragma omp parallel
    {

#pragma omp for schedule(static, 1)
        for (int i2 = 0; i2 < n * p; i2++) {
            int j = i2 % p;

            float tt = 0;
//            float* cur_matrix0 = matrix1 + i2 - j;
//            float* cur_matrix1 = matrix21 + m * j;
            for (int k = 0; k < m; k++) {
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
