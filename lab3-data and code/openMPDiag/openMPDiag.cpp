// g++ ./openMPDiag.cpp -fopenmp -o ./openMPDiag.o -std=c++11
// #include <iostream>
// #include <queue>
// #include <fstream>
// #include <chrono>
// #include <omp.h>
// #include <pthread.h>
// using namespace std;
// using namespace std::chrono;
// const int NUM_THREADS = 4;
// int main()
// {
//     ofstream result("result.txt");
//     fstream testset("testset.txt");
//     string a = "114514";
//     string b = "1919810";
//     int test = 10;
//     long long total = 0;

//     while (testset >> a)
//     {
//         testset >> b;
//         //a="114514";
//         //b="1919810";
//         int distance = 0;
//         for (int k = 0; k < test; k += 1)
//         {
//             int DP_cols = a.length() + 1;
//             int DP_rows = b.length() + 1;
//             int *DP = new int[DP_cols * DP_rows];

//             int i, j, iter;
//             for (i = 0; i < DP_rows; i++)
//             {
//                 DP[i * DP_cols] = i;
//             }
//             for (j = 0; j < DP_cols; j++)
//             {
//                 DP[j] = j;
//             }
//             int P = 6, tid = 0;
//             auto start = system_clock::now();
// #pragma omp parallel num_threads(P) firstprivate(DP_rows, DP_cols, a, b, P) shared(DP) private(iter, tid, i, j)
//             {
//                 for (iter = 1; iter < (DP_cols + DP_rows); iter++)
//                 {
//                     // cout<<iter<<endl;
//                     tid = omp_get_thread_num();
//                     for (j = tid + 1; j <= iter; j += P)
//                     {
//                         i = iter - j + 1;
//                         if (i >= DP_rows || j >= DP_cols)
//                             continue;
//                         DP[i * DP_cols + j] = std::min(std::min((DP[(i - 1) * DP_cols + j] + 1), (DP[i * DP_cols + (j - 1)] + 1)), (DP[(i - 1) * DP_cols + (j - 1)] + ((a[j - 1] != b[i - 1]) ? 1 : 0)));
//                     }
// #pragma omp barrier
//                 }
//             }
//             auto duration = duration_cast<microseconds>(system_clock::now() - start);
//             cout << duration.count() << endl;
//             cout << DP[DP_cols * DP_rows-1] << endl;
//             total += duration.count();
//             //cout << "here" << endl;
//         }
//         result << distance << " " << total / 10 << endl;
//         total = 0;
//         // system("pause");
//     }
// }

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>
using namespace std::chrono;

void printMatrix(int *matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i * cols + j] << "\t";
        }
        std::cout << std::endl;
    }
}
void initializeMatrix(int *&matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i * cols + j] = 100;
        }
    }
}

void testResult(std::string X, std::string Y, int *DP, int DP_rows, int DP_cols)
{
    int i, j;
    int *serialMatrix = new int[DP_rows * DP_cols];
    for (i = 0; i < DP_rows; i++)
    {
        serialMatrix[i * DP_cols] = i;
    }
    for (j = 0; j < DP_cols; j++)
    {
        serialMatrix[j] = j;
    }
    for (i = 1; i < DP_rows; i++)
    {
        for (j = 1; j < DP_cols; j++)
        {
            serialMatrix[i * DP_cols + j] = std::min(std::min((serialMatrix[(i - 1) * DP_cols + j] + 1),
                                                              (serialMatrix[i * DP_cols + (j - 1)] + 1)),
                                                     (serialMatrix[(i - 1) * DP_cols + (j - 1)] + ((X[j - 1] != Y[i - 1]) ? 1 : 0)));
        }
    }
    int flag = 0;
    for (i = 0; i < DP_rows; i++)
    {
        for (j = 0; j < DP_cols; j++)
        {
            if (serialMatrix[i * DP_cols + j] != DP[i * DP_cols + j])
                flag = -1;
        }
    }
    if (flag == -1)
        printf("ERROR!!!! Please check the code. The serial and parallel DP matrix are not the same.\n");
}
int editDistanceOpenmp(std::string X, std::string Y, int *DP, int DP_rows, int DP_cols)
{
    int i, j, iter;
    for (i = 0; i < DP_rows; i++)
    {
        DP[i * DP_cols] = i;
    }
    for (j = 0; j < DP_cols; j++)
    {
        DP[j] = j;
    }
    int P = 4, tid = 0;
    auto start = system_clock::now();
#pragma omp parallel num_threads(P) firstprivate(DP_rows, DP_cols, X, Y, P) shared(DP) private(iter, tid, i, j)
    {
        // P = omp_get_num_threads();
        // std::cout<<P<<std::endl;
        for (iter = 1; iter < (DP_cols + DP_rows); iter++)
        {
            tid = omp_get_thread_num();
            for (j = tid + 1; j <= iter; j += P)
            {
                i = iter - j + 1;
                if (i >= DP_rows || j >= DP_cols)
                    continue;
                DP[i * DP_cols + j] = std::min(std::min((DP[(i - 1) * DP_cols + j] + 1), (DP[i * DP_cols + (j - 1)] + 1)), (DP[(i - 1) * DP_cols + (j - 1)] + ((X[j - 1] != Y[i - 1]) ? 1 : 0)));
            }
#pragma omp barrier
        }
    }
    auto duration = duration_cast<microseconds>(system_clock::now() - start);
}
int main(int argc, char *argv[])
{
    std::string X = "";
    std::string Y = "";
    std::fstream testset("testset.txt");
    std::ofstream result("result.txt");
    // testset >> X;
    // testset >> Y;
    std::cout << X.size() << std::endl;

    double start, end, diff_parallel;
    int average = 0;
    int test = 10;
    while (testset >> X)
    {
        testset >> Y;
        // X = "114514";
        // Y = "1919810";
        int distance = 0;
        for (int i = 0; i < 10; i++)
        {
            int N = X.length();
            int M = Y.length();
            int DP_rows = M + 1;
            int DP_cols = N + 1;
            int *DP = new int[DP_rows * DP_cols];
            initializeMatrix(DP, DP_rows, DP_cols);
            diff_parallel = 0.0;

            int duration = editDistanceOpenmp(X, Y, DP, DP_rows, DP_cols);

            std::cout << DP[DP_rows * DP_cols - 1] << std::endl;
            std::cout << duration << std::endl;
            // printMatrix(DP, DP_rows, DP_cols);
            distance = DP[DP_rows * DP_cols - 1];
            average += duration;
            delete DP;
        }
        average /= test;
        result << distance << " " << average << std::endl;
        average = 0;
    }

    // testResult(X, Y, DP, DP_rows, DP_cols);
    // std::cout << X << "\t" << X.length() << std::endl << Y << "\t" << Y.length() << std::endl;
    // printMatrix(DP, DP_rows, DP_cols);
    printf("Parallel Execution Time=%f\n", average);

    return 0;
}