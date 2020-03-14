#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <OpenCL/opencl.h>
#include <math.h>

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


const int numOfDevice = 1;

const size_t n = 1024;
const size_t m = 1024;
const size_t p = 1024;

const size_t sizeX = 16;
const size_t sizeY = 16;


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

    cl_context context = clCreateContext(0, 1, deviceIds + numOfDevice, NULL, NULL, &errCode);
    printf("Context errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceIds[numOfDevice], CL_QUEUE_PROFILING_ENABLE,
                                                         &errCode);
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
    char *program = (char *) malloc(fileSize * sizeof(char) + 2);
    char ch = 0;
    size_t index = 0;
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

    char *buildOptions = (char *) calloc(1, 1000 * sizeof(char));
    sprintf(buildOptions, "-D LS1=%d", sizeX);

    printf("buildOptions: %s\n", buildOptions);

    errCode = clBuildProgram(clProg, 1, deviceIds + numOfDevice, buildOptions, NULL, NULL);
    printf("BuildProgram errCode %d\n", errCode);
//    if (errCode != 0) {
//        return 1;
//    }

    size_t clBuildInfoLogSize = -1;
    clGetProgramBuildInfo(clProg, deviceIds[numOfDevice], CL_PROGRAM_BUILD_LOG, 0, NULL, &clBuildInfoLogSize);
    char *buildInfoLog = (char *) malloc(clBuildInfoLogSize * sizeof(char));
    clGetProgramBuildInfo(clProg, deviceIds[numOfDevice], CL_PROGRAM_BUILD_LOG, clBuildInfoLogSize, buildInfoLog,
                          &clBuildInfoLogSize);
    printf("Compiler response: %s\n", buildInfoLog);


    const char add[] = "add";
    errCode = 0;
    cl_kernel kernel = clCreateKernel(clProg, add, &errCode);
    printf("Kernel errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }






//    const size_t arrLen = 100000;
//
//    int *a = (int *) malloc(arrLen * sizeof(int));
//    int *b = (int *) malloc(arrLen * sizeof(int));
//    int *c = (int *) malloc(arrLen * sizeof(int));
//
//    for (int i = 0; i < arrLen; i++) {
//        a[i] = i;
//        b[i] = arrLen + i;
//        c[i] = 0;
//    }



    float *matrix1 = (float *) malloc(n * m * sizeof(float));
    float *matrix2 = (float *) malloc(m * p * sizeof(float));
    float *matrix21 = (float *) malloc(m * p * sizeof(float));
    float *matrix3 = (float *) malloc(n * p * sizeof(float));
    float *matrix31 = (float *) malloc(n * p * sizeof(float));

    for (int i = 0; i < n * m; i++) {
//        matrix1[i] = (1.0 + i + 1.0 * i * n * 0.02 + 1.0 * i * m * 0.034) / (i + 1);
        matrix1[i] = i;
    }
    for (int i = 0; i < m * p; i++) {
//        matrix2[i] = (1.0 + i + 1.0 * i * n * 1.87 + 1.0 * i * m * 1.34) / (i + 1);
        matrix2[i] = i + n * m;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            matrix21[j * m + i] = matrix2[i * p + j];
        }
    }


    cl_mem buffer1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * n * m, NULL, &errCode);
    printf("Buffer1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    cl_mem buffer2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * m * p, NULL, &errCode);
    printf("Buffer2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    cl_mem buffer3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * n * p, NULL, &errCode);
    printf("Buffer3 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    errCode = clEnqueueWriteBuffer(commandQueue, buffer1, 0, 0, sizeof(int) * n * m, matrix1, 0, 0, 0);
    printf("Enqueue buffer1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clEnqueueWriteBuffer(commandQueue, buffer2, 0, 0, sizeof(int) * m * p, matrix21, 0, 0, 0);
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
    errCode = clSetKernelArg(kernel, 3, sizeof(int), &m);
    printf("Set arg4 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clSetKernelArg(kernel, 4, sizeof(int), &p);
    printf("Set arg5 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    cl_event event;
//    size_t aaa = arrLen;
    size_t dimSize[2] = {n, p};
    size_t *zero[2] = {0, 0};
    size_t dimLocal[2] = {sizeX, sizeY};
    errCode = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, dimSize, dimLocal, 0, 0, &event);
    printf("clEnqueueNDRangeKernel errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    errCode = clEnqueueReadBuffer(commandQueue, buffer3, 1, 0, sizeof(float) * n * p, matrix3, 0, 0, 0);
    printf("Enqueue read buffer errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

//    int rr = 0;
//    for (int i = 0; i < arrLen; i++){
//        rr += c[i];
//    }
    printf("Result: %f\n", matrix3[0]);
//    printMatrix(matrix1, n, m);
//    printMatrix(matrix2, m, p);
//    printMatrix(matrix3, n, p);



    cl_ulong begin;
    cl_ulong end;
    size_t tmp;
    errCode = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &begin, &tmp);
    printf("clGetEventProfilingInfo1 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, &tmp);
    printf("clGetEventProfilingInfo2 errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }

    printf("Time: %lldms\n", (end - begin) / 1000000);
    printf("Time begin: %lldms\n", begin);
    printf("Time end: %lldms\n", end);


    errCode = clReleaseKernel(kernel);
    printf("Release kernel errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clReleaseProgram(clProg);
    printf("Release program errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clReleaseCommandQueue(commandQueue);
    printf("Release commandQueue errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }
    errCode = clReleaseContext(context);
    printf("Release context errCode %d\n", errCode);
    if (errCode != 0) {
        return 1;
    }


    printf("comparing...\n");

    struct timeval stop, start;
    gettimeofday(&start, NULL);

#pragma omp parallel
    {

        // вычисления на транспонированной матрице
#pragma omp for schedule(static, 1)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++) {
                float tt = 0;
                for (int k = 0; k < m; k++) {
                    tt += matrix1[i * m + k] * matrix21[j * m + k];
                }
                matrix31[i * p + j] = tt;
            }
        }

    }

    int res = 0;
    for (int i = 0; i < n * p; i++) {
//        if (abs(matrix31[i] - matrix3[i]) >= 0.1 * ((matrix31[i] > matrix3[i]) ? matrix31[i] : matrix3[i])) {
        float diff = matrix31[i] - matrix3[i];
        if (((diff > 0 ? diff : (-diff)) >= 0.0001 * ((matrix31[i] > matrix3[i]) ? matrix31[i] : matrix3[i]))) {
            printf("someshit: %24.6f %24.6f i:%d j: %d diff: %f\n", matrix3[i], matrix31[i], i / p, i % p, diff);
            res = -1;
            break;
        }
    }

    gettimeofday(&stop, NULL);
    long long int tt = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000;

    printf("someshit: %14.6f %14.6f\n", matrix3[0], matrix31[0]);

    printf("Result of comparing: %d\n", res);
    printf("time: %lld ms\n", tt);

//    printMatrix(matrix1, n, m);
//    printMatrix(matrix2, m, p);
//    printMatrix(matrix3, n, p);
//    printMatrix(matrix31, n, p);



    return 0;
}
