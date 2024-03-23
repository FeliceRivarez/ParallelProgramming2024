#include <iostream>
#include<windows.h>
#include <fstream>
using namespace std;
const int tests=10;
int power(int n,int power)
{
    if(power==0)
    {
        return 1;
    }
    int result=1;
    for(int i=0;i<power;i+=1)
    {
        result*=n;
    }
    return result;
}
int main()
{
    ofstream output("./integerSumImproved.txt");
    long long average=0;
    long long time[tests];
    for(int i2=0;i2<30;i2+=1)
    {
        int n=power(2,i2);
        for(int i1=0;i1<tests;i1+=1)
        {
            double *a=new double[n];
            double result=0;
            for(int i=0;i<n;i+=1)
            {
                a[i]=double(rand()%100000000)/1000000;
                //cout<<a[i]<<" ";
            }
            cout<<endl;
            long long freq,start,End;
            QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&start);
            double sum1=0;
            double sum2=0;
            result=0;
            for(int i=0;i<n;i+=2)
            {
                sum1+=a[i];
                sum2+=a[i+1];
            }
            result=sum1+sum2;
            QueryPerformanceCounter((LARGE_INTEGER*)&End);
            cout<<"b"<<1000*(End-start)/freq<<"ms"<<endl;
            cout<<result<<endl;
            time[i1]=1000*(End-start)/freq;
            delete a;
        }
        for(int i=0;i<tests;i+=1)
        {
            average+=time[i];
        }
        average/=tests;
        output<<i2<<" "<<n<<" "<<average<<endl;
    }
}




    