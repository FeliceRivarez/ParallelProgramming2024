//g++ ./openMPRow.cpp -fopenmp -o ./openMPRow.o -std=c++11
#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
#include <omp.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
using namespace std;
using namespace std::chrono;
const int NUM_THREADS=6;
int main()
{
    ofstream result("result.txt");
    fstream testset("testset.txt");
    string a="114514";
    string b="1919810";
    int test=10;
    long long total=0;

    while(testset>>a)
    {
        testset>>b;
        //a="114514";
        //b="1919810";
        int distance=0;
        for(int k=0; k<test; k+=1)
        {
            int m=a.length();
            int n=b.length();
            int **dp=new int*[m+1];
            for(int i=0; i<m+1; i+=1)
            {
                dp[i]=new int[n+1];
            }
            for(int i=0; i<m+1; i+=1)
            {
                dp[i][0]=i;
            }
            for(int i=0; i<n+1; i+=1)
            {
                dp[0][i]=i;
            }
            auto start = system_clock::now();
            int**mt=new int*[m+1];
            for(int i=0; i<m+1; i+=1)
            {
                mt[i]=new int[n+1];
            }

            for(int i=0; i<m+1; i+=1)
            {
                mt[i][0]=0;
            }
            for(int i=0; i<n+1; i+=1)
            {
                mt[0][i]=0;
            }
            int i=0;
            int i1=1;
            #pragma omp parallel num_threads(NUM_THREADS), private(i,i1), default(shared)
            #pragma omp for
            for(int i=1; i<m+1; i+=1)
            {
                int nearest=0;
                for(int i1=1; i1<n+1; i1+=1)
                {
                    if(a[i-1]==b[i1-1])
                    {
                        nearest=i1;
                    }
                    mt[i][i1]=nearest;
                }
            }
            for(int i=1; i<m+1; i+=1)
            {
                int i1;
                #pragma omp parallel num_threads(NUM_THREADS), private(i1), default(shared)
                #pragma omp for
                for(int i1=1; i1<n+1; i1+=1)
                {
                    if(a[i-1]==b[i1-1])
                    {
                        dp[i][i1]=dp[i-1][i1-1];
                    }
                    else
                    {
                        if(mt[i][i1]==0)
                        {
                            dp[i][i1]=min(min(dp[i-1][i1],dp[i-1][i1-1]),dp[i-1][mt[i][i1]]+i1-mt[i][i1]-1)+1;
                        }
                        else
                        {
                            dp[i][i1]=min(min(dp[i-1][i1],dp[i-1][i1-1]),dp[i-1][mt[i][i1]-1]+i1-mt[i][i1]-1)+1;
                        }    
                    }
                    //cout<<dp[i][i1]<<" ";
                }
                //cout<<endl;
            }
            auto duration = duration_cast<microseconds>(system_clock::now() - start);
            cout<<duration.count()<<endl;
            total+=duration.count();
            cout<<dp[m][n]<<endl;
            distance=dp[m][n];

            for(int i=0; i<m+1; i+=1)
            {
                delete dp[i];
            }
            for(int i=0; i<m+1; i+=1)
            {
                delete mt[i];
            }
        }
        result<<distance<<" "<<total/10<<endl;
        total=0;
    }
    //system("pause");
}
