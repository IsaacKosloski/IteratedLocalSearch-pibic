//Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//g++ node.cpp scanner.cpp functions.cpp main.cpp -o TspSeq2 -Wall -pedantic
//./TspSeq2 ../Benchmarks/a280.tsp/a280.tsp ../Solutions/Sequential/a280.sol
//./TspSeq2 ../Benchmarks/d198.tsp/d198.tsp ../Solutions/Sequential/d198two.sol

#include "scanner.h"
#include "functions.h"
#define MAX_ITERATION 1000

/* First section: Global variables */

/* Second  section: main function */
int main(int argc,char **argv)
{
    for(int exec = 0; exec < 30; exec++)
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

        /*Third subsection: Generate Initial Solution by greedy algorithm*/
        greedyTSP(tsp, initialSolution);
        /*Fourth subsection: Apply a local search*/
        //linKernighan(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);
        twoOpt(tsp, initialSolution, bestSolution ,tsp->dimensionOfNodes);

        while (iteration < MAX_ITERATION)
        {
            /*Fifth subsection: Change de route, by apply a perturbation*/
            doubleBridgeMove(bestSolution, perturbedSolution , tsp->dimensionOfNodes);
            //linKernighan(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes);
            twoOpt(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes);
            /*Sixth subsection: Check if the new route is better than the previous one*/
            better(tsp, perturbedSolution, bestSolution, tsp->dimensionOfNodes);
            iteration++;
        }

        //Get the current time after the algorithm ends
        auto finish = chrono::high_resolution_clock::now();

        /*Seventh subsection: Print the results*/
        /*cout << "Best solution cost: " << solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes) << endl;
        cout << "Best solution rout: ";
        for (int i = 0; i < tsp->dimensionOfNodes; i++)
            cout << "[" << bestSolution[i].ID << "] ";*/

        //Calculate the time duration by subtracting the start time from the finish time
        chrono::duration<double> elapsed = finish - start;

        //Print the time duration
        cout << "Exec num: " << exec << "\n";
        cout << "Elapsed time: " << elapsed.count() << " s\n";

        string dimension(argv[2]);
        string outputFile1 = "../../Solutions/Tempestade/Sequential/" + dimension + "/" + dimension + "i3GDBO2B" + to_string(exec) + ".sol" ;
        char const *outputFileC = outputFile1.c_str();

        double bestSolutionCost = solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes);
        printResult(bestSolution, outputFileC, elapsed.count(), bestSolutionCost, tsp->dimensionOfNodes, MAX_ITERATION);

        //Memory deallocation
        delete[] bestSolution;
        delete[] perturbedSolution;
        delete[] initialSolution;
        delete[] tsp->nodes;
        delete[] tsp->nodesDistance;
        delete tsp;
    }
    return 0;
}


