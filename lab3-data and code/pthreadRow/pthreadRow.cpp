// g++ ./pthreadRow.cpp -o ./pthreadRow.o -pthread -std=c++11
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int NUM_THREADS = 8;

struct ThreadArgs
{
    int **dp;
    int **mt;
    string *a, *b;
    int x, y;
    int amount;
    ThreadArgs(){};
};

struct mtArgs
{
    int **mt;
    string *a, *b;
    int x;
    int n;
    int amount;
    mtArgs(){};
};

void *edit_distance_thread(void *args)
{

    ThreadArgs *targs = (ThreadArgs *)args;
    int **dp = targs->dp;
    int **mt = targs->mt;
    string *a = targs->a;
    string *b = targs->b;
    int x = targs->x;
    int y = targs->y;
    // cout<<x<<","<<y<<" "<<endl;
    int amount = targs->amount;
    for (int i1 = 0; i1 < amount; i1 += 1)
    {
        // cout << dp[x - 1][y] << " " << dp[x - 1][y - 1] << " " << dp[x][y - 1] << endl;
        if ((*a)[x - 1] == (*b)[y - 1])
        {
            dp[x][y] = dp[x - 1][y - 1];
        }
        else
        {
            // cout<<"(";
            if (mt[x][y] != 0)
            {
                dp[x][y] = min(min(dp[x - 1][y], dp[x - 1][y - 1]), dp[x - 1][mt[x][y] - 1] + y - mt[x][y] - 1) + 1;
            }
            else
            {
                dp[x][y] = min(min(dp[x - 1][y], dp[x - 1][y - 1]), dp[x - 1][mt[x][y]] + y - mt[x][y] - 1) + 1;
            }
            // cout<<")";
        }
        y += 1;
    }

    // cout<<endl;
    pthread_exit(NULL);
}

void *mt_thread(void *args)
{
    mtArgs *targs = (mtArgs *)args;
    int **mt = targs->mt;
    string *a = targs->a;
    string *b = targs->b;
    int x = targs->x;
    int n = targs->n;
    int amount = targs->amount;
    // cout << x << " " << amount << endl;
    for (int i = 0; i < amount; i += 1)
    {
        // cout << i << endl;
        int nearest = 0;
        for (int i1 = 1; i1 < n + 1; i1 += 1)
        {
            // cout << i1 << " " << x;
            if ((*a)[x - 1] == (*b)[i1 - 1])
            {
                nearest = i1;
            }
            // cout << "here" << endl;
            mt[x][i1] = nearest;
            //cout << mt[x][i1] << " ";
        }
        x += 1;
        //cout << endl;
    }
    // cout << x << endl;
    pthread_exit(NULL);
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
        int distance = 0;
        //a = "114514";
        //b = "1919810";
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

            mtArgs margs[NUM_THREADS];
            pthread_t threads[NUM_THREADS];
            int amount = m / NUM_THREADS;
            int leftover = m % NUM_THREADS;
            int marker = 1;
            for (int i1 = 0; i1 < NUM_THREADS; i1 += 1)
            {
                //cout << i1 <<" "<<marker<<endl;
                if (leftover > 0)
                {
                    // cout<<"!"<<endl;
                    margs[i1].mt = mt;
                    margs[i1].amount = amount + 1;
                    margs[i1].a = &a;
                    margs[i1].b = &b;
                    margs[i1].x = marker;
                    margs[i1].n = n;
                    pthread_create(&threads[i1], NULL, mt_thread, &margs[i1]);
                    leftover -= 1;
                    marker += amount + 1;
                }
                else
                {
                    margs[i1].mt = mt;
                    margs[i1].amount = amount;
                    margs[i1].a = &a;
                    margs[i1].b = &b;
                    margs[i1].x = marker;
                    margs[i1].n = n;
                    pthread_create(&threads[i1], NULL, mt_thread, &margs[i1]);
                    marker += amount;
                }
            }

            for (int i1 = 0; i1 < NUM_THREADS; ++i1)
            {
                pthread_join(threads[i1], NULL);
            }
            // cout<<"DEAD"<<endl;
            // for (int i = 1; i < m + 1; i += 1)
            // {
            //     int nearest = 0;
            //     for (int i1 = 1; i1 < n + 1; i1 += 1)
            //     {
            //         if (a[i - 1] == b[i1 - 1])
            //         {
            //             nearest = i1;
            //         }
            //         mt[i][i1] = nearest;
            //     }
            // }

            ThreadArgs targs[NUM_THREADS];
            amount = n / NUM_THREADS;
            leftover = n % NUM_THREADS;

            for (int i = 1; i < m + 1; i += 1)
            {
                int temp_left = leftover;
                int start = 1;
                for (int i1 = 0; i1 < NUM_THREADS; i1 += 1)
                {
                    // cout << i << " " << start << " " << amount << endl;
                    if (temp_left > 0)
                    {
                        targs[i1].dp = dp;
                        targs[i1].mt = mt;
                        targs[i1].amount = amount + 1;
                        targs[i1].a = &a;
                        targs[i1].b = &b;
                        targs[i1].x = i;
                        targs[i1].y = start;
                        pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                        temp_left -= 1;
                        start += amount + 1;
                    }
                    else
                    {
                        targs[i1].dp = dp;
                        targs[i1].mt = mt;
                        targs[i1].amount = amount;
                        targs[i1].a = &a;
                        targs[i1].b = &b;
                        targs[i1].x = i;
                        targs[i1].y = start;
                        pthread_create(&threads[i1], NULL, edit_distance_thread, &targs[i1]);
                        start += amount;
                    }
                }
                for (int i1 = 0; i1 < NUM_THREADS; ++i1)
                {
                    pthread_join(threads[i1], NULL);
                }
                // cout<<endl;
            }

            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout << duration.count() << endl;
            total += duration.count();
            cout << dp[m][n] << endl;
            distance = dp[m][n];
            // for (int i = 0; i < m + 1; i += 1)
            // {
            //     for (int i1 = 0; i1 < n + 1; i1 += 1)
            //     {
            //         cout << mt[i][i1] << " ";
            //     }
            //     cout << endl;
            // }

            for (int i = 0; i < m + 1; i += 1)
            {
                delete dp[i];
            }
            for (int i = 0; i < m + 1; i += 1)
            {
                delete mt[i];
            }
        }
        result << distance << " " << total / 10 << endl;
        total = 0;
    }
    // system("pause");
}
