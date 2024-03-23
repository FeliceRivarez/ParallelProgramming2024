#include <iostream>
#include<windows.h>
using namespace std;

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
    int n=power(2,3);
    int *a=new int[n];
    int result=0;
    for(int i=0;i<n;i+=1)
    {
        a[i]=rand()%100;
    }
    long long freq,start,End;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&start);
    for(int i=0;i<n;i+=1)
    {
        result+=a[i];
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&End);
    cout<<"a"<<1000*(End-start)/freq<<"ms"<<endl;
    cout<<result<<endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&start);
    int sum1=0;
    int sum2=0;
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
    QueryPerformanceCounter((LARGE_INTEGER*)&start);
    for(int m=n;m>1;m/=2)
    {
        for(int i=0;i<m/2;i+=1)
        {
            a[i]+=a[m/2+i];
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&End);
    cout<<"c"<<1000*(End-start)/freq<<"ms"<<endl;
    cout<<a[0]<<endl;
    system("pause");
}
