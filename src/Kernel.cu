#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

__global__ void
bar(int* array) {
    int n = threadIdx.x;
    array[n] = n + 10;
}

int kernel() {

    int* device_x;
    int result;

    cudaMalloc(&device_x, sizeof(int) * 4);
    bar<<<1,4>>>(device_x);
    cudaThreadSynchronize();

    cudaMemcpy(&result, device_x+3, sizeof(int), cudaMemcpyDeviceToHost);
    
    cudaFree(device_x);
    return result;
}

