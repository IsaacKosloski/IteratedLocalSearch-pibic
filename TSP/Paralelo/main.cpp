//Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//Using OpenMP for parallel computation
//g++ node.cpp scanner.cpp functions.cpp main.cpp -o TSP -fopenmp -Wall -pedantic


#include "scanner.h"
#include "functions.h"
#include <omp.h>

#define MAX_ITERATION 10

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
//    perturbedSolution = new Node[tsp->dimensionOfNodes];
    bestSolution = new Node[tsp->dimensionOfNodes];

    //Get the current time before the algorithm starts
    auto start = omp_get_wtime();

    /*Third subsection: Generate Initial Solution by greedy algorithm*/
    greedyTSP(tsp, initialSolution);

    /*Fourth subsection: Apply a local search*/
    twoOpt(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);

    #pragma omp parallel
    {
        //Allocating an array of disturbed solutions for each thread
        perturbedSolution = new Node[tsp->dimensionOfNodes];

        //Allocating a two-dimension array to getting each thread the best final solution
        #pragma omp single
            threadsSolution = new Node[tsp->dimensionOfNodes * omp_get_num_threads()];

        //Copying the bestSolution array for each thread matrix row
        #pragma omp critical
            for (i = 0; i < tsp->dimensionOfNodes; i++)
                threadsSolution[(tsp->dimensionOfNodes * omp_get_thread_num()) + i] = bestSolution[i];

        #pragma omp for
        for (iteration = 0; iteration < MAX_ITERATION; iteration++)
        {
            /*Fifth subsection: Change de route, by apply a perturbation*/
            doubleBridgeMove(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());
            twoOpt(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes, omp_get_thread_num());
            /*Sixth subsection: Check if the new route is better than the previous one*/
            if (solutionCost(perturbedSolution, tsp->nodesDistance, tsp->dimensionOfNodes) <
                solutionCost(threadsSolution, tsp->nodesDistance, tsp->dimensionOfNodes, omp_get_thread_num()))
                /*If the new route is better than the previous one, update the best route*/
                copyArrayToMatrix(threadsSolution, perturbedSolution, omp_get_thread_num(), tsp->dimensionOfNodes);
        }
        #pragma omp single
            findBestSolution(threadsSolution, tsp->nodesDistance, bestSolution, tsp->dimensionOfNodes, omp_get_num_threads());
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
    delete[] perturbedSolution;
    delete[] initialSolution;
    delete[] threadsSolution;
    delete tsp;

    return 0;
}


