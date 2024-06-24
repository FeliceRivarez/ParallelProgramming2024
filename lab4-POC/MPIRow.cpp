// mpic++ ./MPIRow.cpp -o ./MPIRow.o -std=c++11
#include <iostream>
#include <fstream>
#include <chrono>
#include <string.h>
#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    cout << "hello" << endl;
    ofstream result("./result.txt");
    fstream testset("./testset.txt");
    string a = "114514";
    string b = "1919810";
    int test = 10;
    long long total = 0;
    long long start, End, freq;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        int end_flag = 0;
        while (testset >> a)
        {
            // cout<<"!"<<endl;
            testset >> b;
            int m = a.length();
            int n = b.length();

            end_flag = 0;
            // MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
            int segment_length = b.length() / (size - 1);
            // 遍历原始字符串，每次找到分隔点
            for (int i = 0; i < size - 1; ++i)
            {
                // cout<<"!"<<endl;
                //  计算当前分段的开始位置
                int start = i * segment_length;
                // 计算当前分段的结束位置
                int end = (i + 1) * segment_length;
                // 如果最后一个分段的长度不足，则调整结束位置
                if (i == size - 2)
                {
                    end = b.length();
                }
                // 创建当前分段的子字符串
                string segment = b.substr(start, end - start);
                // cout<<segment<<endl;
                // string segment = b;
                int seg_len = strlen(segment.c_str());
                MPI_Send(&seg_len, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
                char content[seg_len + 1];
                segment.copy(content, segment.length(), 0);
                content[seg_len] = '\0';
                MPI_Send(&content, seg_len + 1, MPI_CHAR, i + 1, 1, MPI_COMM_WORLD);

                int seg_len_a = strlen(a.c_str());
                MPI_Send(&seg_len_a, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
                char content_a[seg_len_a + 1];
                a.copy(content_a, a.length(), 0);
                content_a[seg_len_a] = '\0';
                MPI_Send(&content_a, seg_len_a + 1, MPI_CHAR, i + 1, 1, MPI_COMM_WORLD);

                // 获取偏移量
                MPI_Send(&start, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD);

                // 向进程传递a的长度
                MPI_Send(&m, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
            }
            // // 向进程1发布计算MT数组的命令
            // int signal = 1;
            // MPI_Send(&signal, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

            // 每轮循环完整地计算完MT和DP数组的一行，计算a.length()次后得到最终结果
            for (int i = 0; i < m; i += 1)
            {
                // 向进程一传递MT数组的nearest值
                int nearest = 0;
                MPI_Send(&nearest, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

                MPI_Status status;
                // 接收最后一个进程的信息，说明MT数组计算完毕
                MPI_Recv(&nearest, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                cout << "END" << endl;
                
            }
        }
        
        end_flag = 1;
        MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        int end_flag = 0;
        while (!end_flag)
        {
            // cout << "end_flag:" << end_flag << endl;
            //  MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

            // 获取字符串b
            int seg_length;
            MPI_Status status;
            MPI_Recv(&seg_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            char *b = new char[seg_length + 1];
            MPI_Recv(b, seg_length + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            string b1(b);
            cout << b1 << endl;

            // 获取字符串a
            MPI_Recv(&seg_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            char *a = new char[seg_length + 1];
            MPI_Recv(a, seg_length + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            string a1(a);
            cout << a1 << endl;

            // 获取偏移量
            int start;
            MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            // 预备工作，初始化MT[0]和DP[0]、DP_prev[0]
            int *mt = new int[b1.length()];
            int *dp = new int[b1.length()];
            int *dp_prev = new int[b1.length()];
            for (int i = 0; i < b1.length(); i += 1)
            {
                dp_prev[i] = i + start;
            }
            // 接收a的长度
            int m;
            MPI_Recv(&m, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

            // 注意此后不能再用seglength
            // 每轮循环完整地计算完MT和DP数组的一行，计算a.length()次后得到最终结果
            for (int i = 0; i < m; i += 1)
            {
                // // 接收来自上一个进程的指令
                // int signal;
                // MPI_Recv(&signal, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);

                // 接收来自上一个进程的nearest值
                int nearest;
                MPI_Recv(&nearest, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                cout << "rank:" << rank << endl;
                // 计算MT数组
                for (int i1 = 0; i1 < b1.length(); i1 += 1)
                {
                    //cout << a1[i] << "compared to" << b1[i1] << "start" << start << "nearest" << nearest << "result:";
                    if (a1[i] == b1[i1])
                    {
                        nearest = i1 + start;
                    }
                    mt[i1] = nearest;
                    cout << mt[i1] << " ";
                }
                if (rank != size - 1)
                {
                    MPI_Send(&nearest, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                }
                else
                {
                    MPI_Send(&nearest, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                    cout << endl;
                }
            }
        }
        cout << "!" << endl;
    }
    MPI_Finalize();
    // while (testset >> a)';
    // {
    //     testset >> b;
    //     int distance = 0;
    //     for (int k = 0; k < test; k += 1)
    //     {
    //         int m = a.length();
    //         int n = b.length();
    //         int **dp = new int *[m + 1];
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             dp[i] = new int[n + 1];
    //         }
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             dp[i][0] = i;
    //         }
    //         for (int i = 0; i < n + 1; i += 1)
    //         {
    //             dp[0][i] = i;
    //         }
    //         auto start = system_clock::now();
    //         int **mt = new int *[m + 1];
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             mt[i] = new int[n + 1];
    //         }
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             mt[i][0] = 0;
    //         }
    //         for (int i = 0; i < n + 1; i += 1)
    //         {
    //             mt[0][i] = 0;
    //         }
    //         for (int i = 1; i < m + 1; i += 1)
    //         {
    //             int nearest = 0;
    //             for (int i1 = 1; i1 < n + 1; i1 += 1)
    //             {
    //                 if (a[i - 1] == b[i1 - 1])
    //                 {
    //                     nearest = i1;
    //                 }
    //                 mt[i][i1] = nearest;
    //             }
    //         }
    //         for (int i = 1; i < m + 1; i += 1)
    //         {
    //             for (int i1 = 1; i1 < n + 1; i1 += 1)
    //             {
    //                 if (a[i - 1] == b[i1 - 1])
    //                 {
    //                     dp[i][i1] = dp[i - 1][i1 - 1];
    //                 }
    //                 else
    //                 {
    //                     if (mt[i][i1] != 0)
    //                     {
    //                         dp[i][i1] = min(min(dp[i - 1][i1], dp[i - 1][i1 - 1]), dp[i - 1][mt[i][i1] - 1] + i1 - mt[i][i1] - 1) + 1;
    //                     }
    //                     else
    //                     {
    //                         dp[i][i1] = min(min(dp[i - 1][i1], dp[i - 1][i1 - 1]), dp[i - 1][mt[i][i1]] + i1 - mt[i][i1] - 1) + 1;
    //                     }
    //                 }
    //                 // cout<<dp[i][i1]<<" ";
    //             }
    //             // cout<<endl;
    //         }
    //         auto duration = duration_cast<microseconds>(system_clock::now() - start);
    //         cout << duration.count() << endl;
    //         total += duration.count();
    //         cout << dp[m][n] << endl;
    //         distance = dp[m][n];
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             delete dp[i];
    //         }
    //         for (int i = 0; i < m + 1; i += 1)
    //         {
    //             delete mt[i];
    //         }
    //     }
    //     result << distance << " " << total / 10 << endl;
    //     total = 0;
    // }
    // system("pause");
}
