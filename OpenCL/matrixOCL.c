#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

const char *kernelSource = R"(
__kernel void matrixMultiply(__global float* A, 
                             __global float* B, 
                             __global float* C, 
                             const int N) {
    int row = get_global_id(0);
    int col = get_global_id(1);
    
    float sum = 0.0;
    for (int k = 0; k < N; k++) {
        sum += A[row * N + k] * B[k * N + col];
    }
    C[row * N + col] = sum;
}
)";

void checkError(cl_int ret, const char *operation) {
    if (ret != CL_SUCCESS) {
        printf("Error during operation '%s': %d\n", operation, ret);
        exit(1);
    }
}

int main() {
    const int N = 1000; // Matrix dimension
    size_t bytes = N * N * sizeof(float);

    // Host data
    float *A = (float*)malloc(bytes);
    float *B = (float*)malloc(bytes);
    float *C = (float*)malloc(bytes);

    // Initialize matrices
    for (int i = 0; i < N * N; i++) {
        A[i] = 1.0f; 
        B[i] = 1.0f; 
        C[i] = 0.0f;
    }

    // OpenCL setup
    cl_int ret;
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;

    // Get platform and device
    ret = clGetPlatformIDs(1, &platform_id, NULL);
    checkError(ret, "clGetPlatformIDs");

    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    checkError(ret, "clGetDeviceIDs");

    // Create context and command queue
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    checkError(ret, "clCreateContext");

    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
    checkError(ret, "clCreateCommandQueue");

    // Create buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &ret);
    checkError(ret, "clCreateBuffer A");

    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &ret);
    checkError(ret, "clCreateBuffer B");

    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &ret);
    checkError(ret, "clCreateBuffer C");

    // Copy data to device
    ret = clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0, bytes, A, 0, NULL, NULL);
    checkError(ret, "clEnqueueWriteBuffer A");

    ret = clEnqueueWriteBuffer(command_queue, bufferB, CL_TRUE, 0, bytes, B, 0, NULL, NULL);
    checkError(ret, "clEnqueueWriteBuffer B");

    // Create program from source and build it
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &ret);
    checkError(ret, "clCreateProgramWithSource");

    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char*)malloc(log_size);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build Log:\n%s\n", log);
        free(log);
        exit(1);
    }

    // Create kernel
    kernel = clCreateKernel(program, "matrixMultiply", &ret);
    checkError(ret, "clCreateKernel");

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(int), &N);

    // Set work group and NDRange size
    size_t globalSize[] = {N, N};
    size_t localSize[] = {16, 16};

    // Execute kernel
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
    checkError(ret, "clEnqueueNDRangeKernel");

    // Read back result
    ret = clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0, bytes, C, 0, NULL, NULL);
    checkError(ret, "clEnqueueReadBuffer");

    //Uncomment to print result
     printf("Result Matrix:\n");
     for (int i = 0; i < N; i++) {
         for (int j = 0; j < N; j++) {
             printf("%f ", C[i * N + j]);
         }
         printf("\n");
     }


    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    free(A);
    free(B);
    free(C);

    return 0;
}
