/*Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//Using OpenMP for parallel computation
g++ node.cpp scanner.cpp functions.cpp main.cpp -o TspPar -fopenmp -Wall -pedantic
 --d198
 ./TspPar ../Benchmarks/d198.tsp/d198.tsp ../Solutions/Parallel/d198/d198i3.sol
 ./TspPar ../Benchmarks/d198.tsp/d198.tsp ../Solutions/Parallel/d198/d198i4.sol
 ./TspPar ../Benchmarks/d198.tsp/d198.tsp ../Solutions/Parallel/d198/d198i5.sol
--
./TspPar ../Benchmarks/a280.tsp/a280.tsp ../Solutions/Parallel/a280i5.sol
./TspPar ../Benchmarks/lin318.tsp/lin318.tsp ../Solutions/Parallel/lin318i5.sol
./TspPar ../Benchmarks/pcb442.tsp/pcb442.tsp ../Solutions/Parallel/pcb442i5.sol
./TspPar ../Benchmarks/rat783.tsp/rat783.tsp ../Solutions/Parallel/rat783i5.sol
./TspPar ../Benchmarks/u1060.tsp/u1060.tsp ../Solutions/Parallel/u1060i5.sol
 ./TspPar ../Benchmarks/pcb1173.tsp/pcb1173.tsp ../Solutions/Parallel/pcb1173i5.sol
 ./TspPar ../Benchmarks/fl1577.tsp/pcb442.tsp ../Solutions/Parallel/fl1577i5.sol

 ./TspPar ../Benchmarks/pcb442.tsp/pcb442.tsp ../Solutions/Parallel/pcb442i5.sol
 ./TspPar ../Benchmarks/pcb442.tsp/pcb442.tsp ../Solutions/Parallel/pcb442i5.sol


// ./TspPar a280.tsp a280.res
// ./TspPar bench-res-test/d198.tsp bench-res-test/d198.res
// ./TspPar bench-res-test/a280.tsp bench-res-test/a280.res
// ./TspPar bench-res-test/lin318.tsp bench-res-test/lin318.res
// ./TspPar bench-res-test/pcb442.tsp bench-res-test/pcb442.res
// ./TspPar bench-res-test/rat783.tsp bench-res-test/rat783.res
// ./TspPar bench-res-test/u1060.tsp bench-res-test/u1060.res
// ./TspPar bench-res-test/pcb1173.tsp bench-res-test/pcb1173.res
// ./TspPar bench-res-test/fl1577.tsp bench-res-test/fl1577.res

*/

#include "../Parallel/scanner.h"
#include "../Parallel/functions.h"
#include <omp.h>

#define MAX_ITERATION 1000

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
    //perturbedSolution = new Node[tsp->dimensionOfNodes];
    bestSolution = new Node[tsp->dimensionOfNodes];

    //Get the current time before the algorithm starts
    auto start = omp_get_wtime();

    /*Third subsection: Generate Initial Solution by greedy algorithm*/
    greedyTSP(tsp, initialSolution);

    /*Fourth subsection: Apply a local search*/
    twoOpt(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);

    #pragma omp parallel
    {
        //Allocating a two-dimension array to getting each thread the best final solution
        #pragma omp single
        {
            //Allocating an array of disturbed solutions for each thread
            perturbedSolution = new Node[tsp->dimensionOfNodes * omp_get_num_threads()];
            threadsSolution = new Node[tsp->dimensionOfNodes * omp_get_num_threads()];
        }

        //Copying the bestSolution array for each thread matrix row **
        #pragma omp critical
        {
            for (i = 0; i < tsp->dimensionOfNodes; i++)
                threadsSolution[(tsp->dimensionOfNodes * omp_get_thread_num()) + i] = bestSolution[i];
        }


        #pragma omp for
        for (iteration = 0; iteration < MAX_ITERATION; iteration++)
        {
            /*Fifth subsection: Change de route, by apply a perturbation*/
            doubleBridgeMove(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());
            /*doubleBridgeMove(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());
            doubleBridgeMove2(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());
            doubleBridgeMove2(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());
            doubleBridgeMove2(threadsSolution, perturbedSolution , tsp->dimensionOfNodes, omp_get_thread_num());*/

            twoOpt(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes, omp_get_thread_num());

            /*Sixth subsection: Check if the new route is better than the previous one*/
            better(tsp, perturbedSolution, threadsSolution, omp_get_thread_num());
        }
        #pragma omp single
        {
            findBestSolution(threadsSolution, tsp->nodesDistance, bestSolution, tsp->dimensionOfNodes, omp_get_num_threads());
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
    delete[] initialSolution;
    delete[] perturbedSolution;
    delete[] threadsSolution;
    delete[] tsp->nodes;
    delete[] tsp->nodesDistance;
    delete tsp;

    return 0;
}


