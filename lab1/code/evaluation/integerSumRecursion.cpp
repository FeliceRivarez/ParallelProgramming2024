#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;
const int tests = 10;
int power(int n, int power)
{
    if (power == 0)
    {
        return 1;
    }
    int result = 1;
    for (int i = 0; i < power; i += 1)
    {
        result *= n;
    }
    return result;
}
int main()
{
    int n = power(2, 29);
    double *a = new double[n];
    double result = 0;
    for (int i = 0; i < n; i += 1)
    {
        a[i] = double(rand() % 100000000) / 1000000;
        // cout<<a[i]<<" ";
    }
    cout << endl;
    long long freq, start, End;
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    QueryPerformanceCounter((LARGE_INTEGER *)&start);
    for (int m = n; m > 1; m /= 2)
    {
        for (int i = 0; i < m / 2; i += 1)
        {
            a[i] += a[m / 2 + i];
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&End);
    cout << "b" << 1000 * (End - start) / freq << "ms" << endl;
    cout << a[0] << endl;
    delete a;
}
