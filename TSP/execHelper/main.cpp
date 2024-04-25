#include <bits/stdc++.h>
#include <iostream>
#include <string>


using namespace std;
using ::string;

int main(int argc,char **argv)
{
    int i, exec = 3;

    string dimension (argv[2]);

    string outputFile1 = "../Solutions/Parallel/" + dimension + "/C/" + dimension + "i3GDBO2B" + to_string(exec) + ".sol" ;
    char const *outputFileC = outputFile1.c_str();

    ofstream outputFile(argv[1]);
    if (outputFile.is_open())
    {
        for (i = 0; i < 30; i++)
        {
            outputFile << outputFileC << endl; //"./TspPar ../Benchmarks/" << argv[2] << ".tsp/" << argv[2] << ".tsp " << outputFileC << endl; //../Solutions/Parallel/" << argv[2] << "/B/" << argv[2] << "i" << argv[3] << "GDBO2B" << i << ".sol" << endl;
        }

    }
    outputFile.close();

    return 0;
}