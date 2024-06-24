#include <iostream>
#include <cuda_runtime.h> // Include the CUDA runtime library
#include"device_launch_parameters.h"


// CUDA kernel function for vector addition
__global__ void vector_add(float* out, float* a, float* b, int n) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < n) {
        out[index] = a[index] + b[index];
    }
}

int main() {
    const int N = 114514; // 1 million elements
    size_t size = N * sizeof(float);

    // Allocate host memory
    float* h_a = (float*)malloc(size);
    float* h_b = (float*)malloc(size);
    float* h_out = (float*)malloc(size);

    // Initialize host arrays
    for (int i = 0; i < N; ++i) {
        h_a[i] = 1.0f;
        h_b[i] = 2.0f;
    }

    // Allocate device memory
    float* d_a, * d_b, * d_out;
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_out, size);

    // Copy data from host to device
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // Launch the kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    vector_add  <<<blocksPerGrid, threadsPerBlock>>>(d_out, d_a, d_b, N);

    // Copy result back to host
    cudaMemcpy(h_out, d_out, size, cudaMemcpyDeviceToHost);

    // Verify result
    float sum = 0.0f;
    for (int i = 0; i < N; ++i) {
        sum += h_out[i];
    }
    std::cout << "Sum of array elements: " << sum << std::endl;
 
    // Clean up
    free(h_a);
    free(h_b);
    free(h_out);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_out);

    return 0;
}
