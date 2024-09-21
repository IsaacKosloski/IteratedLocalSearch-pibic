/*Iterated Local Search
//Initial Solution: generate by a greedy algorithm;
//Using OpenMP for parallel computation*/
#include "functions.h"

#define MAX_ITERATION 1000
#define MAX_EXEC 30

/* First section: Global variables */

/* Second  section: main function */
int main(int argc,char **argv)
{
    //Running code for MAX_EXEC

        /*First subsection: variables and data structs declaration (and some default definition)*/
        int iteration = 0, exec = 16;
        double bestSolutionWeight = numeric_limits<double>::max();
    for(exec = 16; exec < MAX_EXEC; exec++)
    {
        Node *initialSolution, *bestSolution;

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

        //omp_set_nested(1);
        #pragma omp parallel //num_threads(omp_get_num_threads()/2)
        {

            double threadsWeight = 0.0;
            Node* threadsSolution = new Node[tsp->dimensionOfNodes];
            Node* perturbedSolution = new Node[tsp->dimensionOfNodes];
            memcpy(threadsSolution, bestSolution, tsp->dimensionOfNodes * sizeof(Node));

            #pragma omp for schedule(auto)  //reduction(min:threadsSolution) /*customize a function for reduction on solution*/
            for (iteration = 0; iteration < MAX_ITERATION; iteration++)
            {
                /*Fifth subsection: Change de route, by apply a perturbation*/
                #pragma omp parallel
                {
                    doubleBridgeMove(threadsSolution, perturbedSolution , tsp->dimensionOfNodes);
                }
                /*Then, applying a local search*/
                twoOpt(tsp, perturbedSolution, perturbedSolution, tsp->dimensionOfNodes);

                /*Sixth subsection: Check if the new route is better than the previous one*/
                better(tsp, perturbedSolution, threadsSolution, threadsWeight);
            }
            #pragma omp critical
            {
                if(threadsWeight < bestSolutionWeight)
                {
                    bestSolutionWeight = threadsWeight;
                    memcpy(bestSolution, threadsSolution, tsp->dimensionOfNodes * sizeof(Node));
                }
            }
            memcpy(bestSolution, threadsSolution, tsp->dimensionOfNodes * sizeof(Node));

            delete[] perturbedSolution;
            delete[] threadsSolution;
        }
        //Get the current time after the algorithm ends
        auto finish = omp_get_wtime();

        /*Seventh subsection: Print the results*/
        /*cout << "Best solution cost: "<< solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes) << endl;
        cout << "Best solution rout: ";
        for (int i = 0; i < tsp->dimensionOfNodes; i++)
            cout << "[" << bestSolution[i].ID << "] ";*/

        //Calculate the time duration by subtracting the start time from the finish time
        auto elapsed = finish - start;

        //Print the time duration
        cout << "Exec num: " << exec << "\n";
        cout << "Elapsed time: " << elapsed << " s\n";

        string dimension(argv[2]);
        string outputFile1 = "../../Solutions/Yoga/Parallel/" + dimension + "/" + dimension + "i3GDBO2B" + to_string(exec) + ".sol" ;
        char const *outputFileC = outputFile1.c_str();

        double bestSolutionCost = solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes);
        printResult(bestSolution, outputFileC, elapsed, bestSolutionCost, tsp->dimensionOfNodes, MAX_ITERATION);

        //Memory deallocation
        delete[] initialSolution;
        delete[] bestSolution;
        delete[] tsp->nodes;
        delete[] tsp->nodesDistance;
        delete tsp;
    }
    return 0;
}


