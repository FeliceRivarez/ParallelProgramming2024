// mpic++ ./MPIRowOpenMP.cpp -o ./MPIRowOpenMP.o -std=c++11 -fopenmp
#include <iostream>
#include <fstream>
#include <chrono>
#include <string.h>
#include <mpi.h>
using namespace std;
using namespace std::chrono;
const int NUM_THREADS=4;
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
            auto start = system_clock::now();
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

                // 向进程传递b的长度
                MPI_Send(&n, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
            }
            // // 向进程1发布计算MT数组的命令
            // int signal = 1;
            // MPI_Send(&signal, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            int *dp_prev = new int[b.length()];
            // 每轮循环完整地计算完MT和DP数组的一行，计算a.length()次后得到最终结果
            for (int i = 0; i < m; i += 1)
            {
                MPI_Status status;

                // 向进程一传递MT数组的nearest值
                int nearest = 0;
                MPI_Send(&nearest, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

                // 接收最后一个进程的信息，说明MT数组计算完毕
                MPI_Recv(&nearest, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                //cout << "END" << endl;
                MPI_Barrier(MPI_COMM_WORLD); // MT数组计算完毕
                // 向第一个进程传递end_value和last_match
                int local_end=i;
                int last_match=0;
                MPI_Send(&local_end, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                MPI_Send(&last_match, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);// 依赖项传递完毕

                // for (int i1 = 1; i1 < size; i1 += 1)
                // {
                //     cout << "SYNCING" << i1 << endl;
                //     int offset;
                //     int amount;
                //     MPI_Recv(&offset, 1, MPI_INT, i1, i1, MPI_COMM_WORLD, &status);
                //     MPI_Recv(&amount, 1, MPI_INT, i1, i1, MPI_COMM_WORLD, &status);
                //     cout << offset << " " << amount << endl;
                //     int *temp = new int[amount];
                //     MPI_Recv(&temp, amount, MPI_INT, i1, i1, MPI_COMM_WORLD, &status);
                //     for (int i2 = 0; i2 < amount; i2 += 1)
                //     {
                //         dp_prev[i2 + offset] = temp[i2];
                //         cout << temp[i2] << endl;
                //     }
                // }
                // MPI_Barrier(MPI_COMM_WORLD);
                // // for (int i1 = 0; i1 < b.length(); i1 += 1)
                // // {
                // //     cout<<dp_prev[i1]<<" ";
                // // }
                // cout << endl;
                // // 除第一轮循环外，从最后一个进程接收dp_prev
                // MPI_Recv(&dp_prev, n, MPI_INT, size - 1, 1, MPI_COMM_WORLD, &status);
                // MPI_Recv(&dp_prev, n, MPI_INT, size - 1, 1, MPI_COMM_WORLD, &status);
                // cout << "FINALIZED" << endl;
                // cout << endl;
                // MPI_Barrier(MPI_COMM_WORLD);
                // cout << "FINALIZED" << endl;
                // // 向各个进程发送dp_prev
                // for (int i1 = 1; i1 < size - 1; i1 += 1)
                // {

                // }
            }
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout<<duration.count()<<endl;
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
            // MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

            // 获取字符串b
            int seg_length;
            MPI_Status status;
            MPI_Recv(&seg_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            char *b = new char[seg_length + 1];
            MPI_Recv(b, seg_length + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            string b1(b);
            //cout << b1 << endl;

            // 获取字符串a
            MPI_Recv(&seg_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            char *a = new char[seg_length + 1];
            MPI_Recv(a, seg_length + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            string a1(a);
            //cout << a1 << endl;

            // 获取偏移量
            int start;
            MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

            // 接收b的长度
            int n;
            MPI_Recv(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

            // 预备工作，初始化MT[0]和DP[0]、DP_prev[0]
            int *mt = new int[b1.length()];
            int *dp = new int[b1.length()];
            int *dp_prev = new int[b1.length()];
            for (int i = 0; i < b1.length(); i += 1)
            {
                dp_prev[i] = i+1;
            }
            // for (int i1 = 0; i1 < n; i1 += 1)
            // {
            //     cout << dp_prev[i1] << " ";
            // }

            // 注意此后不能再用seglength
            // 每轮循环完整地计算完MT和DP数组的一行，计算a.length()次后得到最终结果
            for (int i = 0; i < a1.length(); i += 1)
            {

                // 接收来自上一个进程的nearest值
                int nearest;
                MPI_Recv(&nearest, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                int last_nearest = nearest;
                // cout << "rank:" << rank << endl;
                // 计算MT数组
                for (int i1 = 0; i1 < b1.length(); i1 += 1)
                {
                    // cout << a1[i] << "compared to" << b1[i1] << "start" << start << "nearest" << nearest << "result:";
                    if (a1[i] == b1[i1])
                    {
                        nearest = i1 + start + 1;
                    }
                    mt[i1] = nearest;
                    //cout << mt[i1] << " ";
                }
                if (rank != size - 1)
                {
                    MPI_Send(&nearest, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                }
                else
                {
                    MPI_Send(&nearest, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                    //cout << endl;
                }
                MPI_Barrier(MPI_COMM_WORLD); // MT数组计算完后进行一次同步

                // 拿到end_value和last_match
                
                int end_value, last_match;
                MPI_Recv(&end_value, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(&last_match, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                // cout << "rank:" << rank << endl;
                // cout<<"received:"<<end_value<<" "<<last_match<<endl;
                int local_end = dp_prev[b1.length() - 1];
                if (rank != size - 1)
                {
                    // cout<<"nearest"<<nearest<<endl;
                    // cout<<"last_nearest"<<last_nearest<<endl;
                    if (nearest == last_nearest)
                    {
                        MPI_Send(&local_end, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                        MPI_Send(&last_match, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                    }
                    else
                    {
                        MPI_Send(&local_end, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                        int local_nearest;
                        if (nearest - 1 != start)
                        {
                            local_nearest = dp_prev[nearest - 1 - start];
                        }
                        else
                        {
                            local_nearest = end_value;
                        }
                        MPI_Send(&local_nearest, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                    }
                }
                //cout<<"SYNCING"<<endl;
                MPI_Barrier(MPI_COMM_WORLD); // 所有依赖项传递完后进行二次同步
                
                int i1;
                #pragma omp parallel num_threads(NUM_THREADS), private(i1), default(shared)
                #pragma omp for
                for (i1 = 0; i1 < b1.length(); i1 += 1)
                {
                    if (a1[i] == b[i1])
                    {
                        if (i1 == 0)
                        {
                            dp[i1] = end_value;
                        }
                        else
                        {
                            dp[i1] = dp_prev[i1 - 1];
                        }
                    }
                    else
                    {
                        if (i1 == 0)
                        {
                            dp[i1] = min(min(dp_prev[i1], end_value), last_match + i1 + start - mt[i1] - 1) + 1;
                        }
                        else
                        {
                            dp[i1] = min(min(dp_prev[i1], dp_prev[i1 - 1]), last_match + i1 + start - mt[i1] - 1) + 1;
                        }
                    }
                    // cout << "start + i1" << start + i1 << endl;
                    // cout<<"dp_prev[i1 + start]:"<<dp_prev[i1 + start]<<endl;
                    // cout<<"dp_prev[i1 - 1 + start]:"<<dp_prev[i1 - 1 + start]<<endl;
                    // cout<<"dp_prev[mt[i1] - 1] + i1+start- mt[i1]:"<<dp_prev[mt[i1] - 1] + i1+start- mt[i1];
                    //cout << dp[i1] << " ";
                }

                for (int i1 = 0; i1 < b1.length(); i1 += 1)
                {
                    dp_prev[i1] = dp[i1];
                }
                MPI_Barrier(MPI_COMM_WORLD); 
            }
        }
    }
    MPI_Finalize();
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


// 本轮DP计算完后进行三次同步
                // cout << "rank:" << rank << endl;
                // // 除了第一轮循环以外，接收来自主进程的dp_prev
                // if (i != 0)
                // {
                //     MPI_Recv(&dp_prev, a1.length(), MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
                // }
                // for (int i1 = 0; i1 < b1.length(); i1 += 1)
                // {
                //     if (a1[i] == b[i1])
                //     {
                //         dp[i1] = dp_prev[i1 - 1 + start];
                //     }
                //     else
                //     {
                //         if (mt[i1] != 0)
                //         {
                //             dp[i1] = min(min(dp_prev[i1 + start], dp_prev[i1 - 1 + start]), dp_prev[mt[i1] - 1] + i1 + start - mt[i1] - 1) + 1;
                //         }
                //         else
                //         {
                //             dp[i1] = min(min(dp_prev[i1 + start], dp_prev[i1 - 1 + start]), dp_prev[0] + i1 + start - 0 - 1) + 1;
                //         }
                //     }
                //     // cout << "start + i1" << start + i1 << endl;
                //     dp_result[start + i1] = dp[i1];
                //     // cout<<"dp_prev[i1 + start]:"<<dp_prev[i1 + start]<<endl;
                //     // cout<<"dp_prev[i1 - 1 + start]:"<<dp_prev[i1 - 1 + start]<<endl;
                //     // cout<<"dp_prev[mt[i1] - 1] + i1+start- mt[i1]:"<<dp_prev[mt[i1] - 1] + i1+start- mt[i1];
                //     cout << dp[i1] << " ";
                // }

                // cout << "SYNC" << endl;
                // // 除进程1以外，均等待上一个进程发送结果之后再发出dp_result
                // if (rank == 1)
                // {
                //     MPI_Send(&dp_result, n, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                //     cout << "rank 1 sent" << endl;
                // }
                // else
                // {
                //     MPI_Recv(&dp_result, n, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
                //     cout << "rank" << rank << "recved" << endl;
                //     if (rank == size - 1)
                //     {
                //         cout << "rank" << rank << "sent" << endl;
                //         MPI_Send(&dp_result, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
                //         cout << endl;
                //     }
                //     else
                //     {
                //         cout << "rank" << rank << "sent" << endl;
                //         MPI_Send(&dp_result, n, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
                //     }
                // }
                // cout << "done" << endl;
                // MPI_Barrier(MPI_COMM_WORLD);
                // if (rank == 1)
                // {
                //     int amount = b1.length();
                //     cout << "sending" << rank << endl;
                //     MPI_Send(&start, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
                //     MPI_Send(&amount, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
                //     MPI_Send(&dp, amount, MPI_INT, 0, rank, MPI_COMM_WORLD);
                //     int sig=1;
                //     MPI_Send(&sig, 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD);
                // }
                // else
                // {
                //     int sig;
                //     MPI_Recv(&sig, 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD, &status);
                //     int amount = b1.length();
                //     cout << "sending" << rank << endl;
                //     MPI_Send(&start, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
                //     MPI_Send(&amount, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
                //     MPI_Send(&dp, amount, MPI_INT, 0, rank, MPI_COMM_WORLD);
                // }

                // MPI_Barrier(MPI_COMM_WORLD);
                // cout << "!" << endl;