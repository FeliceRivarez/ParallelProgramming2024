#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <fstream>
using namespace std;
const int tests=10;

int main()
{
    ofstream output("./matrixTrivial.txt");
    long long time[tests];
    long long average=0;
    for(int n=100,i=0;n<10000;n+=100)
    {
        for(int i1=0;i1<tests;i1+=1)
        {
            long long freq, start,End;
            int **a=new int*[n];
            for(int i=0;i<n;i+=1)
            {
                a[i]=new int[n];
            }
            int *b=new int[n];
            int *sum=new int[n];
            for(int i=0;i<n;i+=1)
            {
                for(int i1=0;i1<n;i1+=1)
                {
                    a[i][i1]=rand()%10000000;
                }
            }
            for(int i=0;i<n;i+=1)
            {
                b[i]=rand()%10000;
            }
            QueryPerformanceFrequency((LARGE_INTEGER *)&freq );
            QueryPerformanceCounter((LARGE_INTEGER *)&start );
            for(int i = 0; i < n; i++)
            {
                sum[i] = 0;
                for(int j = 0; j < n; j++)
                    sum[i] += a[j][i] * b[j];
            }
            QueryPerformanceCounter((LARGE_INTEGER *)&End );
            cout<<n<<" "<<1000*(End-start)/freq<<endl;
            time[i1]=1000*(End-start)/freq;
            for(int i=0;i<n;i+=1)
            {
                delete a[i];
            }
            delete sum;
            delete b;
        }
        for(int i=0;i<tests;i+=1)
        {
            average+=time[i];
        }
        average/=tests;
        output<<n<<" "<<average<<endl;
    }
}
