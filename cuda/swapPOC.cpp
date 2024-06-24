#include <iostream>
using namespace std;

int main()
{
    int *dp=new int[10];
    int *dp_prev=new int[10];
    for(int i=0;i<10;i+=1)
    {
        dp[i]=2;
        dp_prev[i]=1;
    }
    int *temp;
    temp=dp;
    dp=dp_prev;
    dp_prev=temp;
    for(int i=0;i<10;i+=1)
    {
        cout<<dp[i]<<" ";
    }
    cout<<endl;
}