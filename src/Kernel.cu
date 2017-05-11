#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>
#include "LocklessMap.h"
#include "board.h"

__device__ int 
compute_thing_kernel(int input) {
    int acc = 0;
    for (int i = 0; i < 1000; i++) {
        acc += i % (input+1);
    }
    return acc;
}

__device__ int
get(Board& b, int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    return b.state[row][col];
}

__device__ int
score_board(Board& b) {
    int sum = 0;
    compute_thing_kernel(50);
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int p = b.state[row][col];
            if (p == 0)
                continue;
            int length = 1;
            while (get(b, row,col+length) == p) {
                length += 1;
            }
            if (length >= 4)
                return p*INF;
            sum += p*length;
            length = 1;
            while (get(b, row+length, col) == p) {
                length += 1;
            }
            if (length >= 4)
                return p*INF;
            sum += p*length;
            length = 1;
            while (get(b, row+length, col+length) == p) {
                length += 1;
            }
            if (length >= 4)
                return p*INF;
            sum += p*length;
            length = 1;
            while (get(b, row+length, col-length) == p) {
                length += 1;
            }
            if (length >= 4)
                return p*INF;
            sum += p*length;
        }
    }
    return sum;
}

__global__ void
score_kernel(Board* device_array, int count, int* result_array) {
    int i = blockIdx.x * 256 + threadIdx.x;
    if (i >= count)
        return;

    int s = score_board(device_array[i]);
    result_array[i] = s;
}

void score_base_cuda (Frontier& input, int player, LocklessMap& result) {
    int count = input.count;
    Board* device_array;
    int* result_array;
    cudaMalloc(&device_array, sizeof(Board) * count);
    cudaMalloc(&result_array, sizeof(int) * count);
    cudaMemcpy(device_array, input.buffer, sizeof(Board) * count, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int numBlocks = (count+threadsPerBlock-1)/threadsPerBlock;
    score_kernel<<<numBlocks, threadsPerBlock>>>(device_array, count, result_array);
    cudaThreadSynchronize();

    int* host_result = new int[count];
    cudaMemcpy(host_result, result_array, sizeof(int) * count, cudaMemcpyDeviceToHost);

    for (int i = 0; i < count; i++) {
        Board& b = input.buffer[i];
        Key k = b.getKey();
        result.put_seq(k, host_result[i]);
    }

    cudaFree(device_array);
    cudaFree(result_array);
    delete [] host_result;
    return;
}

