//Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//g++ node.cpp scanner.cpp functions.cpp mainTemp.cpp -o TspSeq -Wall -pedantic
//./TspSeq ../Benchmarks/a280.tsp/a280.tsp ../Solutions/Sequential/a280.sol
//./TspSeq ../Benchmarks/d198.tsp/d198.tsp ../Solutions/Sequential/d198.sol
//./TspSeq ../Benchmarks/u1060.tsp/u1060.tsp ../Solutions/Sequential/u1060.sol
//./TspSeq ../Benchmarks/fl1577.tsp/fl1577.tsp ../Solutions/Sequential/fl1577.sol

#include "scanner.h"
#include "functions.h"
#include <chrono>
#define MAX_ITERATION 1000
#define MAX_TIME_MS 3600000 //120000

/* First section: Global variables */

/* Second  section: main function */
int main(int argc,char **argv)
{
    /*First subsection: variables and data structs declaration (and some default definition)*/
    int iteration = 0;
    Node *initialSolution, *perturbedSolution, *bestSolution;


    /* Second subsection: data input*/
    Scanner *tsp = new Scanner(argv[1]);

    //Memory allocation
    initialSolution = new Node[tsp->dimensionOfNodes];
    perturbedSolution = new Node[tsp->dimensionOfNodes];
    bestSolution = new Node[tsp->dimensionOfNodes];

    //Get the current time before the algorithm starts
    auto start = chrono::high_resolution_clock::now();

    //Set the maximum execution time in milliseconds
    const int max_time_ms = MAX_TIME_MS;
    chrono::milliseconds duration(max_time_ms);

    /*Third subsection: Generate Initial Solution by greedy algorithm*/
    greedyTSP(tsp, initialSolution);
    /*Fourth subsection: Apply a local search*/
    linKernighan(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);
    //twoOpt(tsp, initialSolution, bestSolution, tsp->dimensionOfNodes);
    while (iteration < MAX_ITERATION && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start) < duration)
    {
        /*Fifth subsection: Change de route, by apply a perturbation*/
        doubleBridgeMove2(bestSolution, perturbedSolution , tsp->dimensionOfNodes);
        doubleBridgeMove2(bestSolution, perturbedSolution , tsp->dimensionOfNodes);
        doubleBridgeMove2(bestSolution, perturbedSolution , tsp->dimensionOfNodes);
        doubleBridgeMove2(bestSolution, perturbedSolution , tsp->dimensionOfNodes);
        doubleBridgeMove2(bestSolution, perturbedSolution , tsp->dimensionOfNodes);

        linKernighan(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes);
        //twoOpt(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes);
        /*Sixth subsection: Check if the new route is better than the previous one*/
        better(tsp, perturbedSolution, bestSolution, tsp->dimensionOfNodes);
        iteration++;
    }

    //Get the current time after the algorithm ends
    auto finish = chrono::high_resolution_clock::now();

    /*Seventh subsection: Print the results*/
    cout << "Best solution cost: "<< solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes) << endl;
    cout << "Best solution rout: ";
    for (int i = 0; i < tsp->dimensionOfNodes; i++)
        cout << "[" << bestSolution[i].ID << "] ";

    //Calculate the time duration by subtracting the start time from the finish time
    chrono::duration<double> elapsed = finish - start;

    //Print the time duration
    cout << "Elapsed time: " << elapsed.count() << " s\n";
    cout << "Iterations: " << iteration << endl;

    double bestSolutionCost = solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes);
    printResult(bestSolution, argv[2], elapsed.count(), bestSolutionCost, tsp->dimensionOfNodes, iteration);

    //Memory deallocation
    delete[] bestSolution;
    delete[] perturbedSolution;
    delete[] initialSolution;
    delete[] tsp->nodes;
    delete[] tsp->nodesDistance;
    delete tsp;

    return 0;
}


