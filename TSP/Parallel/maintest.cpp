//Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//Using OpenMP for parallel computation
// g++ node.cpp scanner.cpp functions.cpp maintest.cpp -o TspPar -fopenmp -Wall -pedantic
// ./TspPar a280.tsp a280.res
// ./TspPar bench-res-test/d198.tsp bench-res-test/d198.res



#include "scanner.h"
#include "functions.h"
#include <omp.h>

#define MAX_ITERATION 3

/* First section: Global variables */


/* Second  section: main function */
int main(int argc,char **argv)
{
    /*First subsection: variables and data structs declaration (and some default definition)*/
    int iteration = 0, i;
    Node *initialSolution, *perturbedSolution, *bestSolution, *threadsSolution;


    /* Second subsection: data input*/
    Scanner *tsp = new Scanner(argv[1]);

    //Memory allocation
    initialSolution = new Node[tsp->dimensionOfNodes];
    bestSolution = new Node[tsp->dimensionOfNodes];

    //Get the current time before the algorithm starts
    auto start = omp_get_wtime();

    /*Third subsection: Generate Initial Solution by greedy algorithm*/
    greedyTSP(tsp, initialSolution);

    /*Fourth subsection: Apply a local search*/
    twoOpt(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);
    //linKernighan(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);

    #pragma omp parallel
    {
        //Allocating an array of disturbed solutions for each thread
        perturbedSolution = new Node[tsp->dimensionOfNodes];

        //Deallocating perturbedSolution array for each thread
        #pragma omp critical
        {
            delete[] perturbedSolution;
        }
    }
    //Get the current time after the algorithm ends
    auto finish = omp_get_wtime();

    /*Seventh subsection: Print the results*/
    cout << "Best solution cost: "<< solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes) << endl;
    cout << "Best solution rout: ";
    for (int i = 0; i < tsp->dimensionOfNodes; i++)
        cout << "[" << bestSolution[i].ID << "] ";

    //Calculate the time duration by subtracting the start time from the finish time
    auto elapsed = finish - start;

    //Print the time duration
    cout << "Elapsed time: " << elapsed << " s\n";

    double bestSolutionCost = solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes);
    printResult(bestSolution, argv[2], elapsed, bestSolutionCost, tsp->dimensionOfNodes, MAX_ITERATION);

    //Memory deallocation
    delete[] bestSolution;
    //delete[] perturbedSolution;
    delete[] initialSolution;
    //delete[] threadsSolution;
    delete[] tsp->nodes;
    delete[] tsp->nodesDistance;
    delete tsp;

    return 0;
}


