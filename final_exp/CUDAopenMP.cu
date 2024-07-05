//nvcc -o CUDAopenMP CUDAopenMP.cu -ccbin "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.39.33519\bin\Hostx64\x64\cl.exe" -Xcompiler " -openmp" 
#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
// #include <omp.h>
using namespace std;
using namespace std::chrono;
const int NUM_THREADS = 6;
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void levenshtein_kernel(int *dp, int *dp_prev, int *mt_row, int m, int n)
{
    int i1 = blockIdx.x * blockDim.x + threadIdx.x;

    if (i1 < n + 1)
    {
        int i = blockIdx.y * blockDim.y + threadIdx.y + 1;
        int nearest = mt_row[i1];
        if (nearest == 0)
        {
            dp[i1] = min(dp_prev[i1], min(dp_prev[i1 - 1], dp_prev[nearest] + i1 - nearest - 1)) + 1;
        }
        else
        {
            dp[i1] = min(dp_prev[i1], min(dp_prev[i1 - 1], dp_prev[nearest - 1] + i1 - nearest - 1)) + 1;
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
        // a = "114514";
        // b = "1919810";
        int distance = 0;
        for (int k = 0; k < test; k += 1)
        {
            int m = a.length();
            int n = b.length();
            int *dp = new int[n + 1];
            int *dp_prev = new int[n + 1];

            // int **dp=new int*[m+1];
            // for(int i=0; i<m+1; i+=1)
            // {
            //     dp[i]=new int[n+1];
            // }
            // for(int i=0; i<m+1; i+=1)
            // {
            //     dp[i][0]=i;
            // }
            for (int i = 0; i < n + 1; i += 1)
            {
                dp_prev[i] = i;
            }
            dp[0] = 1;
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
            cout << "!" << endl;
            int i = 0;
            int i1 = 1;
#pragma omp parallel num_threads(NUM_THREADS), private(i, i1), default(shared)
#pragma omp for
            for (int i = 1; i < m + 1; i += 1)
            {
                int nearest = 0;
                for (int i1 = 1; i1 < n + 1; i1 += 1)
                {
                    if (a[i - 1] == b[i1 - 1])
                    {
                        nearest = i1;
                    }
                    mt[i][i1] = nearest;
                }
            }
            auto duration1=duration_cast<microseconds>(system_clock::now() - start);
            int *d_dp, *d_dp_prev, *d_mt_row;
            cudaMalloc((void **)&d_dp, (n + 1) * sizeof(int));
            cudaMalloc((void **)&d_dp_prev, (n + 1) * sizeof(int));
            cudaMalloc((void **)&d_mt_row, (n + 1) * sizeof(int));

            // 初始化dp和dp_prev
            int *h_dp = new int[n + 1];
            int *h_dp_prev = new int[n + 1];
            for (int i1 = 0; i1 < n + 1; i1++)
            {
                h_dp_prev[i1] = i1;
            }
            cudaMemcpy(d_dp_prev, h_dp_prev, (n + 1) * sizeof(int), cudaMemcpyHostToDevice);
            int memcpy=0;
            int vacancy=0;
            for (int i = 1; i < m + 1; i += 1)
            {
                auto duration2=duration_cast<microseconds>(system_clock::now() - start);
                // 将当前行的mt数组传输到GPU
                cudaMemcpy(d_mt_row, mt[i], (n + 1) * sizeof(int), cudaMemcpyHostToDevice);
                auto duration3=duration_cast<microseconds>(system_clock::now() - start);
                memcpy+=duration3.count()-duration2.count();

                // 调用CUDA核函数
                dim3 blockDim(128, 1);                              // 根据需要调整线程块大小
                dim3 gridDim((n + blockDim.x - 1) / blockDim.x, 1); // 根据需要调整网格大小
                levenshtein_kernel<<<gridDim, blockDim>>>(d_dp, d_dp_prev, d_mt_row, m, n);

                // 将结果传输回主机
                cudaMemcpy(h_dp, d_dp, (n + 1) * sizeof(int), cudaMemcpyDeviceToHost);
                auto duration4=duration_cast<microseconds>(system_clock::now() - start);
                vacancy+=duration4.count()-duration3.count();
                // 交换dp和dp_prev
                int *temp = h_dp;
                h_dp = h_dp_prev;
                h_dp_prev = temp;
                h_dp_prev[0] = i; // 设置dp[0]
                h_dp[0] = i + 1;
                // for (int i1 = 0; i1 < n + 1; i1 += 1)
                // {
                //     cout << h_dp_prev[i1] << " ";
                // }
                // cout << endl;
                // 将新的dp_prev拷贝回GPU
                // if (i == m)
                // {
                //     cout << h_dp_prev[n] << endl;
                // }
                distance=h_dp_prev[n];
                cudaMemcpy(d_dp_prev, h_dp_prev, (n + 1) * sizeof(int), cudaMemcpyHostToDevice);
            }

            // 计算距离
            //cudaMemcpy(&distance, h_dp_prev + n, sizeof(int), cudaMemcpyDeviceToHost);

            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            cout<<"mt time:"<<duration1.count()<<endl;
            cout<<"memcpy:"<<memcpy<<endl;
            cout<<"vacancy:"<<vacancy<<endl;
            total += duration.count();
            cout << distance << endl;
            memcpy=0;
            vacancy=0;
            // 释放内存
            delete[] h_dp;
            delete[] h_dp_prev;
            for (int i = 0; i < m + 1; i++)
            {
                delete[] mt[i];
            }
            delete[] mt;
            cudaFree(d_dp);
            cudaFree(d_dp_prev);
            cudaFree(d_mt_row);
        }
        result << distance << " " << total / 10 << endl;
        total = 0;
    }
    // system("pause");
}
