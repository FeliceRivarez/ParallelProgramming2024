#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;
const int tests=10;

int main()
{
    ofstream output("./matrixImproved.txt");
    long long time[tests];
    long long average=0;
    for(int n=100;n<10000;n+=100)
    {
        for(int i1=0;i1<tests;i1+=1)
        {
            long long start, End, freq;
            int **a=new int*[n];
            for(int i=0;i<n;i+=1)
            {
                a[i]=new int[n];
            }
            int *b=new int[n];
            int *sum=new int[n];
            QueryPerformanceFrequency((LARGE_INTEGER *)&freq );
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
            int *sum1=new int[n];
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
            cout<<n<<" "<<1000*(End-start)/freq<<endl;
            time[i1]=1000*(End-start)/freq;
            //system("pause");
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
