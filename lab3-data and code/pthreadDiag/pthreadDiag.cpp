// g++ ./pthreadDiag.cpp -o ./pthreadDiag.o -pthread -std=c++11
#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
#include <pthread.h>
using namespace std;
using namespace std::chrono;

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

            const int num_threads = 2;
            pthread_t threads[num_threads];
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
                if (range != 0)
                {
                    // cout<<"----------------------------"<<endl;
                    // cout<<range<<endl;
                    for (int i1 = 0; i1 < num_threads; ++i1)
                    {
                        // cout<<begin_x<<","<<begin_y<<" "<<endl;
                        if (leftover != 0)
                        {
                            targs[i1].dp = dp;
                            targs[i1].amount = range + 1;
                            targs[i1].a = &a;
                            targs[i1].b = &b;
                            targs[i1].x = begin_x;
                            targs[i1].y = begin_y;
                            pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                            leftover -= 1;
                            begin_x += range + 1;
                            begin_y -= range + 1;
                        }
                        else
                        {
                            targs[i1].dp = dp;
                            targs[i1].amount = range;
                            targs[i1].a = &a;
                            targs[i1].b = &b;
                            targs[i1].x = begin_x;
                            targs[i1].y = begin_y;
                            pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                            begin_x += range;
                            begin_y -= range;
                        }
                    }
                    for (int i1 = 0; i1 < num_threads; ++i1)
                    {
                        pthread_join(threads[i1], NULL);
                    }
                }
                else
                {
                    // cout<<"here"<<endl;
                    int temp_i = i;
                    for (int i1 = 1; i1 < a.length() + 1 && temp_i >= 1; i1 += 1)
                    {

                        // cout<<i1<<","<<temp_i<<" ";
                        if (a[i1 - 1] != b[temp_i - 1])
                        {
                            dp[i1][temp_i] = min(min(dp[i1 - 1][temp_i], dp[i1 - 1][temp_i - 1]), dp[i1][temp_i - 1]) + 1;
                        }
                        else
                        {
                            dp[i1][temp_i] = dp[i1 - 1][temp_i - 1];
                        }
                        temp_i -= 1;
                    }
                }
                // cout<<endl;
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
                if (range != 0)
                {
                    // cout<<"----------------------------"<<endl;
                    // cout<<range<<endl;
                    for (int i1 = 0; i1 < num_threads; ++i1)
                    {
                        // cout<<begin_x<<","<<begin_y<<" "<<endl;
                        if (leftover != 0)
                        {
                            targs[i1].dp = dp;
                            targs[i1].amount = range + 1;
                            targs[i1].a = &a;
                            targs[i1].b = &b;
                            targs[i1].x = begin_x;
                            targs[i1].y = begin_y;
                            pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                            leftover -= 1;
                            begin_x += range + 1;
                            begin_y -= range + 1;
                        }
                        else
                        {
                            targs[i1].dp = dp;
                            targs[i1].amount = range;
                            targs[i1].a = &a;
                            targs[i1].b = &b;
                            targs[i1].x = begin_x;
                            targs[i1].y = begin_y;
                            pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                            begin_x += range;
                            begin_y -= range;
                        }
                    }
                    for (int i1 = 0; i1 < num_threads; ++i1)
                    {
                        pthread_join(threads[i1], NULL);
                    }
                }
                else
                {
                    // cout<<"here"<<endl;
                    int temp_i = i;
                    for (int i1 = b.length(); i1 >= 1 && temp_i < a.length() + 1; i1 -= 1)
                    {

                        if (a[temp_i - 1] != b[i1 - 1])
                        {
                            dp[temp_i][i1] = min(min(dp[temp_i - 1][i1], dp[temp_i - 1][i1 - 1]), dp[temp_i][i1 - 1]) + 1;
                        }
                        else
                        {
                            dp[temp_i][i1] = dp[temp_i - 1][i1 - 1];
                        }
                        temp_i += 1;
                    }
                }
            }
            /*for(int i=0; i<a.length()+1; i+=1)
            {
                for(int i1=0; i1<b.length()+1; i1+=1)
                {
                    cout<<dp[i][i1]<<" ";
                }
                cout<<endl;
            }*/
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            cout << dp[a.length()][b.length()] << endl;
            distance = dp[a.length()][b.length()];
            total += duration.count();
        }
        total = total / test;
        result << distance << " " << total << endl;
        total = 0;
        distance = 0;
    }

    // system("pause");
}
