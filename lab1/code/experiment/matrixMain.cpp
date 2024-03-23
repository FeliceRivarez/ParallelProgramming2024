#include <iostream>
#include <windows.h>
using namespace std;


int main()
{
    int n=2000;
    long long start, End, freq;
    int **a=new int*[n];
    for(int i=0;i<n;i+=1)
    {
        a[i]=new int[n];
    }
    int *b=new int[n];
    int *sum=new int[n];
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq );
    QueryPerformanceCounter((LARGE_INTEGER *)&start );
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
    for(int i = 0; i < n; i++)
    {
        sum[i] = 0;
        for(int j = 0; j < n; j++)
            sum[i] += a[j][i] * b[j];
        //cout<<sum[i]<<" ";
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&End );
    cout<<1000*(End-start)/freq<<endl;
    int *sum1=new int[n];
    End=0;
    start=0;
    QueryPerformanceCounter((LARGE_INTEGER *)&start );
    for(int i=0;i<n;i+=1)
    {
        sum1[i]=0;
    }
    for(int i=0;i<n;i+=1)
    {
        for(int i1=0;i1<n;i1+=1)
        {
            sum1[i1]+=a[i][i1]*b[i];
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&End );
    for(int i=0;i<n;i+=1)
    {
        if(sum1[i]!=sum[i])
        {
            cout<<"ERROR:"<<sum[i]<<" "<<sum1[i]<<endl;
        }
    }
    cout<<1000*(End-start)/freq;
    system("pause");
}
