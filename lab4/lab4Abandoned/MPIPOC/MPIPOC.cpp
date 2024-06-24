#include <iostream>
#include <fstream>
#include <chrono>
#include <mpi.h>
using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    ofstream result("result.txt");
    fstream testset("testset.txt");
    string a = "114514";
    string b = "1919810";
    int test = 10;
    long long total = 0;
    long long start, End, freq;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
}