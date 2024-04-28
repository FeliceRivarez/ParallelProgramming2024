#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

int main()
{
    ofstream result("result.txt");
    fstream testset("testset.txt");
    string a="114514";
    string b="1919810";
    int test=10;
    long long total=0;
    long long start,End,freq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    while(testset>>a)
    {
        testset>>b;
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
            QueryPerformanceCounter((LARGE_INTEGER *)&start);
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
                for(int i1=1; i1<n+1; i1+=1)
                {
                    if(a[i-1]==b[i1-1])
                    {
                        dp[i][i1]=dp[i-1][i1-1];
                    }
                    else
                    {
                        dp[i][i1]=min(min(dp[i-1][i1],dp[i-1][i1-1]),dp[i-1][mt[i][i1]-1]+i1-mt[i][i1]-1)+1;
                    }
                    //cout<<dp[i][i1]<<" ";
                }
                //cout<<endl;
            }
            QueryPerformanceCounter((LARGE_INTEGER *)&End);
            cout<<End-start<<endl;
            cout<<dp[m][n]<<endl;
            distance=dp[m][n];
            total+=End-start;
            for(int i=0;i<m+1;i+=1)
            {
                delete dp[i];
            }
            for(int i=0;i<m+1;i+=1)
            {
                delete mt[i];
            }
        }
        result<<distance<<" "<<total/10<<endl;
        total=0;
    }
    system("pause");
}
