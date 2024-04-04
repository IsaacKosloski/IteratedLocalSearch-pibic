#include <bits/stdc++.h>
using namespace std;
using ::string;

int main(int argc,char **argv)
{
    int i;

    ofstream outputFile(argv[1]);
    if (outputFile.is_open())
    {
        for (i = 0; i < 30; i++)
        {
            outputFile << "./TspPar ../Benchmarks/" << argv[2] << ".tsp/" << argv[2] << ".tsp ../Solutions/Parallel/" << argv[2] << "/B/" << argv[2] << "i" << argv[3] << "GDBO2B" << i << ".sol" << endl;
        }

    }
    outputFile.close();

    return 0;
}