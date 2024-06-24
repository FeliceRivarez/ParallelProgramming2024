// g++ ./openMPRow.cpp -fopenmp -o ./openMPRow.o -std=c++11
//nvcc -o RowCUDA RowCUDA.cu -ccbin "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.39.33519\bin\Hostx64\x64\cl.exe"
#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
#include <omp.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
using namespace std;
using namespace std::chrono;
const int NUM_THREADS = 6;

__global__ void calculate_mt(char *a, char *b, int m, int n, int **mt)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int i1 = blockIdx.y * blockDim.y + threadIdx.y;
    if (i < m + 1 && i1 < n + 1)
    {
        if (a[i - 1] == b[i1 - 1])
        {
            mt[i][i1] = i1;
        }
        else
        {
            mt[i][i1] = 0;
        }
    }
}

__global__ void calculate_dp(char *a, char *b, int m, int n, int **dp, int **mt)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int i1 = blockIdx.y * blockDim.y + threadIdx.y;
    if (i < m + 1 && i1 < n + 1)
    {
        if (a[i - 1] == b[i1 - 1])
        {
            dp[i][i1] = dp[i - 1][i1 - 1];
        }
        else
        {
            int nearest = mt[i][i1];
            if (nearest == 0)
            {
                dp[i][i1] = min(min(dp[i - 1][i1], dp[i - 1][i1 - 1]), dp[i - 1][nearest] + i1 - nearest - 1) + 1;
            }
            else
            {
                dp[i][i1] = min(min(dp[i - 1][i1], dp[i - 1][i1 - 1]), dp[i - 1][nearest - 1] + i1 - nearest - 1) + 1;
            }
        }
    }
}

int main()
{
    ofstream result("result.txt");
    fstream testset("testset.txt");
    string a = "114514";
    string b = "1919810";
    int test = 10;
    long long total = 0;

    while (testset >> a)
    {
        testset >> b;
        // a="114514";
        // b="1919810";
        int distance = 0;
        for (int k = 0; k < test; k += 1)
        {
            int m = a.length();
            int n = b.length();
            int **dp = new int *[m + 1];
            for (int i = 0; i < m + 1; i += 1)
            {
                dp[i] = new int[n + 1];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                dp[i][0] = i;
            }
            for (int i = 0; i < n + 1; i += 1)
            {
                dp[0][i] = i;
            }
            auto start = system_clock::now();
            int **mt = new int *[m + 1];
            for (int i = 0; i < m + 1; i += 1)
            {
                mt[i] = new int[n + 1];
            }

            for (int i = 0; i < m + 1; i += 1)
            {
                mt[i][0] = 0;
            }
            for (int i = 0; i < n + 1; i += 1)
            {
                mt[0][i] = 0;
            }
            int i = 0;
            int i1 = 1;
            cout << "here" << endl;
            // 分配设备内存
            int *dp_device, *mt_device;
            cudaMalloc(&dp_device, (m + 1) * (n + 1) * sizeof(int)); // 分配 (m+1) * (n+1) 个 int 的空间
            cudaMalloc(&mt_device, (m + 1) * (n + 1) * sizeof(int)); // 分配 (m+1) * (n+1) 个 int 的空间

            // 循环中为每个指针分配足够的内存空间
            int **dp_host = new int *[m + 1]; // 分配 m+1 个 int* 指针的空间
            int **mt_host = new int *[m + 1]; // 分配 m+1 个 int* 指针的空间
            for (int i = 0; i < m + 1; i++)
            {
                dp_host[i] = new int[n + 1]; // 为每个指针分配 (n+1) 个 int 的空间
                mt_host[i] = new int[n + 1]; // 为每个指针分配 (n+1) 个 int 的空间
            }

            // 将数据传输到设备
            char *a_device, *b_device;
            cudaMalloc(&a_device, a.length() * sizeof(char));
            cudaMalloc(&b_device, b.length() * sizeof(char));
            cudaMemcpy(a_device, a.c_str(), a.length() * sizeof(char), cudaMemcpyHostToDevice);
            cudaMemcpy(b_device, b.c_str(), b.length() * sizeof(char), cudaMemcpyHostToDevice);

            // 启动内核
            // dim3 threadsPerBlock(16, 16);
            // dim3 blocksPerGrid((m + threadsPerBlock.x - 1) / threadsPerBlock.x, (n + threadsPerBlock.y - 1) / threadsPerBlock.y);
            // calculate_mt<<<blocksPerGrid, threadsPerBlock>>>(a_device, b_device, m, n, mt_device);
            // calculate_dp<<<blocksPerGrid, threadsPerBlock>>>(a_device, b_device, m, n, dp_device, mt_device);

            // 将结果传输回主机
            // cudaMemcpy(dp, dp_device, (m + 1) * sizeof(int *), cudaMemcpyDeviceToHost);
            // for (int i = 0; i < m + 1; i++)
            // {
            //     cudaMemcpy(dp[i], dp_device[i], (n + 1) * sizeof(int), cudaMemcpyDeviceToHost);
            // }
            cout << "here" << endl;
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            total += duration.count();
            cout << dp[m][n] << endl;
            distance = dp[m][n];

            for (int i = 0; i < m + 1; i += 1)
            {
                delete dp[i];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                delete mt[i];
            }
            for (int i = 0; i < m + 1; i++)
            {
                //cudaFree(dp_device[i]);
                //cudaFree(mt_device[i]);
            }
            cudaFree(dp_device);
            cudaFree(mt_device);
            cudaFree(a_device);
            cudaFree(b_device);
        }
        result << distance << " " << total / 10 << endl;
        total = 0;
    }
    // system("pause");
}
