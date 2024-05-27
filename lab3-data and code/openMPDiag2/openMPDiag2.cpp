// g++ ./openMPDiag2.cpp -fopenmp -o ./openMPDiag2.o -std=c++11
#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
#include <pthread.h>
using namespace std;
using namespace std::chrono;
const int num_threads = 2;
struct ThreadArgs
{
    int **dp;
    string *a, *b;
    int x, y;
    int amount;
    ThreadArgs(){};
};

void *edit_distance_thread(void *args)
{

    ThreadArgs *targs = (ThreadArgs *)args;
    int **dp = targs->dp;
    string *a = targs->a;
    string *b = targs->b;
    int x = targs->x;
    int y = targs->y;
    // cout<<x<<","<<y<<" "<<endl;
    int amount = targs->amount;
    for (int i1 = 0; i1 < amount; i1 += 1)
    {

        // cout<<dp[x-1][y]<<" "<<dp[x-1][y-1]<<" "<<dp[x][y-1]<<endl;
        if ((*a)[x - 1] != (*b)[y - 1])
        {
            dp[x][y] = min(min(dp[x - 1][y], dp[x - 1][y - 1]), dp[x][y - 1]) + 1;
        }
        else
        {
            dp[x][y] = dp[x - 1][y - 1];
        }
        x += 1;
        y -= 1;
    }

    // cout<<endl;
    pthread_exit(NULL);
}

int main()
{
    string a = "114514";
    string b = "1919810";
    fstream testset("testset.txt");
    ofstream result("result.txt");
    int test = 10;
    int total = 0;
    int distance = 0;
    while (testset >> a)
    {
        testset >> b;
        // a = "114514";
        // b = "1919810";
        for (int k = 0; k < test; k += 1)
        {
            int **dp = new int *[a.length() + 1];
            for (int i = 0; i < a.length() + 1; i += 1)
            {
                dp[i] = new int[b.length() + 1];
                // cout<<"!"<<endl;
            }
            for (int i = 0; i < a.length() + 1; i += 1)
            {
                dp[i][0] = i;
            }
            for (int i = 0; i < b.length() + 1; i += 1)
            {
                dp[0][i] = i;
            }

            ThreadArgs targs[num_threads];

            auto start = system_clock::now();
            queue<pair<int, int>> candidate;
            for (int i = 1; i < b.length() + 1; i += 1)
            {
                int begin_x = 1;
                int begin_y = i;
                int len = a.length();
                int end_y = max(begin_x + begin_y - len - 1, 0);
                int total = begin_y - end_y;
                int range = total / num_threads;
                int leftover = total % num_threads;
                int i1;
#pragma omp parallel num_threads(num_threads), private(i1), default(shared)
#pragma omp for
                for (int i1 = 0; i1 < total; i1 += 1)
                {
                    // cout<<dp[x-1][y]<<" "<<dp[x-1][y-1]<<" "<<dp[x][y-1]<<endl;
                    // cout << begin_x << " " << begin_y << " " << dp[begin_y - 1][begin_y] << " " << dp[begin_x - 1][begin_y - 1] << " " << dp[begin_x][begin_y - 1] << endl;
                    if (a[1 + i1 - 1] != b[i - i1 - 1])
                    {
                        dp[1 + i1][i - i1] = min(min(dp[1 + i1 - 1][i - i1], dp[1 + i1 - 1][i - i1 - 1]), dp[1 + i1][i - i1 - 1]) + 1;
                    }
                    else
                    {
                        dp[1 + i1][i - i1] = dp[1 + i1 - 1][i - i1 - 1];
                    }
                }
            }
            for (int i = 2; i < a.length() + 1; i += 1)
            {
                int begin_x = i;
                int begin_y = b.length();
                int len = a.length();
                int end_y = max(begin_x + begin_y - len - 1, 0);
                int total = begin_y - end_y;
                int range = total / num_threads;
                int leftover = total % num_threads;
                int i1;
#pragma omp parallel num_threads(num_threads), private(i1), default(shared)
#pragma omp for
                for (int i1 = 0; i1 < total; i1 += 1)
                {
                    // cout<<dp[x-1][y]<<" "<<dp[x-1][y-1]<<" "<<dp[x][y-1]<<endl;
                    // cout << begin_x << " " << begin_y << " " << dp[begin_y - 1][begin_y] << " " << dp[begin_x - 1][begin_y - 1] << " " << dp[begin_x][begin_y - 1] << endl;
                    if (a[i + i1 - 1] != b[b.length() - i1 - 1])
                    {
                        dp[i + i1][b.length() - i1] = min(min(dp[i + i1 - 1][b.length() - i1], dp[i + i1 - 1][b.length() - i1 - 1]), dp[i + i1][b.length() - i1 - 1]) + 1;
                    }
                    else
                    {
                        dp[i + i1][b.length() - i1] = dp[i + i1 - 1][b.length() - i1 - 1];
                    }
                }
            }
            // for (int i = 0; i < a.length() + 1; i += 1)
            // {
            //     for (int i1 = 0; i1 < b.length() + 1; i1 += 1)
            //     {
            //         cout << dp[i][i1] << " ";
            //     }
            //     cout << endl;
            // }
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            cout << dp[a.length()][b.length()] << endl;
            distance = dp[a.length()][b.length()];
            total += duration.count();
            for(int i=0; i<a.length()+1; i+=1)
            {
                delete dp[i];
            }
        }

        total = total / test;
        result << distance << " " << total << endl;
        total = 0;
        distance = 0;
    }

    // system("pause");
}
