#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

int main()
{
    ofstream testset("testset.txt");
    srand(time(NULL));

    for(int i1=4; i1<=94; i1+=5)
    {
        char *a=new char[10000];
        cout<<i1<<endl;
        for(int i=0; i<10000; i+=1)
        {
            a[i]=33+rand()%i1;
            testset<<a[i];
        }
        testset<<endl;
        for(int i=0; i<10000; i+=1)
        {
            if(1==1)
            {
                a[i]=33+rand()%i1;
            }
            testset<<a[i];
        }
        testset<<endl;
    }
    system("pause");
}
