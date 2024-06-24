// mpic++ ./MPIRow.cpp -o ./MPIRow.o -std=c++11
#include <iostream>
#include <fstream>
#include <chrono>
#include <mpi.h>
using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    // ofstream result("./result.txt");
    // fstream testset("./testset.txt");
    // string a = "114514";
    // string b = "1919810";
    // int test = 10;
    // long long total = 0;
    // long long start, End, freq;
    // MPI_Init(&argc, &argv);
    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // if (rank == 0)
    // {
    //     int end_flag = 0;
    //     while (testset >> a)
    //     {
    //         //cout<<"!"<<endl;
    //         testset >> b;
    //         end_flag = 0;
    //         MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //         int segment_length = b.length() / size;
    //         // 遍历原始字符串，每次找到分隔点
    //         for (int i = 0; i < size; ++i)
    //         {
    //             //cout<<"!"<<endl;
    //             // 计算当前分段的开始位置
    //             int start = i * segment_length;
    //             // 计算当前分段的结束位置
    //             int end = (i + 1) * segment_length;
    //             // 如果最后一个分段的长度不足，则调整结束位置
    //             if (i == 9)
    //             {
    //                 end = b.length();
    //             }
    //             // 创建当前分段的子字符串
    //             std::string segment = b.substr(start, end - start);
    //             int seg_len = segment.length();
    //             MPI_Send(&seg_len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    //             //cout<<"!"<<endl;
    //             const char *content = segment.c_str();
    //             MPI_Send(&content, segment.length(), MPI_CHAR, i, 1, MPI_COMM_WORLD);
    //         }
    //     }
    // }
    // else
    // {
    //     int end_flag = 0;
    //     while (!end_flag)
    //     {
    //         MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //         int seg_length;
    //         MPI_Recv(&seg_length,1,MPI_INT,0,0,MPI_COMM_WORLD,NULL);
    //         string b(seg_length, ' ');
    //         MPI_Recv(&b,seg_length,MPI_CHAR,0,1,MPI_COMM_WORLD,NULL);
    //         cout<<b<<endl;
    //     }
    // }

    // while (testset >> a)
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
