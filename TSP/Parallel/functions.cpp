#include "functions.h"

/* Fourth section: Functions and methods implementation */
/*(i) Initial solution methods*/
void greedyTSP(Scanner* &tsp, Node* &initialSolution)
{
    //Start with the first node
    int currentNode = 0;
    initialSolution[0].ID = currentNode;
    tsp->nodes[currentNode].eligible = false;

    //For each node, consider the nearest node that is still eligible
    for (int i = 1; i < tsp->dimensionOfNodes; i++)
    {
        double minDistance = numeric_limits<double>::max();
        int nearestNode = 0;
        for (int j = 0; j < tsp->dimensionOfNodes; j++)
        {
            if (tsp->nodesDistance[(currentNode * tsp->dimensionOfNodes) + j].distanceValue < minDistance &&
                tsp->nodes[j].eligible)
            {
                minDistance = tsp->nodesDistance[(currentNode * tsp->dimensionOfNodes) + j].distanceValue;
                nearestNode = j;
            }
        }
        initialSolution[i].ID = nearestNode;
        initialSolution[i].distanceValue = minDistance;
        for (int j = 0; j < tsp->dimensionOfNodes; j++)
            tsp->nodesDistance[(j * tsp->dimensionOfNodes) + i].eligible = false;
        currentNode = nearestNode;
        tsp->nodes[currentNode].eligible = false;
    }
}

/**********************************************************************************************************************/
/*(ii) Perturbation methods*/
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes, int threadID)
{
    copyMatrixToArray(perturbedSolution, initialSolution, threadID, dimensionOfNodes);

    int start1 = rand() % (dimensionOfNodes / 4);
    int end1 = start1 + (dimensionOfNodes / 4);
    int start2 = end1 + (rand() % (dimensionOfNodes / 4));
    int end2 = start2 + (dimensionOfNodes / 4);

    for(int i = start1, j = end2; i < end1; i++, j--)
        swap(perturbedSolution[i], perturbedSolution[j]);/**/
}


void doubleBridgeMove2(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes, int threadID)
{
    // Copies the original solution to the perturbed solution.
    copyMatrixToArray(perturbedSolution, initialSolution, threadID, dimensionOfNodes);

    // Generates two random indices in the range [0, dimensionOfNodes - 1].
    int start1 = rand() % (dimensionOfNodes / 4);
    int end1 = start1 + (dimensionOfNodes / 4);
    int start2 = end1 + (rand() % (dimensionOfNodes / 4));
    //int end2 = start2 + (dimensionOfNodes / 4);


    for(int i = start1, j = start2; i < end1; i++, j++)
        swap(perturbedSolution[i], perturbedSolution[j]);
}
/**********************************************************************************************************************/
/*(iii) Local search methods*/
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes)
{
    Node* newSolution = new Node[dimensionOfNodes];
    memcpy(bestSolution, solution, dimensionOfNodes * sizeof(Node));

    double bestCost = solutionCost(bestSolution, tsp->nodesDistance, dimensionOfNodes);
    double newCost;
    bool improvement = true;

    while(improvement)
    {
        improvement = false;
        //#pragma omp parallel for collapse(2) private(bestSolution, newSolution)
        for (int i = 1; i < dimensionOfNodes - 2 ; i++)
            for (int j = i + 1; j < dimensionOfNodes; j++)
            {
                memcpy(newSolution, bestSolution, dimensionOfNodes * sizeof(Node));
                reverse(newSolution + i, newSolution + j);
                newCost = solutionCost(newSolution, tsp->nodesDistance, dimensionOfNodes);
                if (newCost < bestCost)
                {
                    memcpy(bestSolution, newSolution, dimensionOfNodes * sizeof(Node));
                    bestCost = newCost;
                    improvement = true;
                }
            }
    }
    delete[] newSolution;
}

void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes, int threadID)
{
    Node* newSolution = new Node[dimensionOfNodes];
    copyMatrixToArray(solution, bestSolution, threadID, dimensionOfNodes);

    double bestCost = solutionCost(bestSolution, tsp->nodesDistance, dimensionOfNodes, threadID);
    bool improvement = true;

    while(improvement)
    {

        improvement = false;
        //#pragma omp parallel for collapse(2) private(bestSolution, newSolution)
        /*for (int i = 1; i < dimensionOfNodes - 2 ; i++)
            for (int j = i + 1; j < dimensionOfNodes; j++)
            {
                memcpy(newSolution, bestSolution, dimensionOfNodes * sizeof(Node));
                reverse(newSolution + i, newSolution + j);
                double newCost = solutionCost(newSolution, tsp->nodesDistance, dimensionOfNodes);
                if (newCost < bestCost)
                {
                    memcpy(bestSolution, newSolution, dimensionOfNodes * sizeof(Node));
                    bestCost = newCost;
                    improvement = true;
                }
            }]*/
    }
    delete[] newSolution;
}


void linKernighan(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes)
{
    Node* newSolution = new Node[dimensionOfNodes];
    memcpy(bestSolution, solution, dimensionOfNodes * sizeof(Node));

    double bestCost = solutionCost(bestSolution, tsp->nodesDistance, dimensionOfNodes);
    double newCost;
    bool improvement = true;
    //Continue until no further improvement is possible
    while (improvement)
    {
        improvement = false;

        // Iterate through all pairs of cities
        #pragma omp parallel for collapse(4)
        for (int i = 0; i < dimensionOfNodes - 1; i++)
            for (int j = i + 1; j < dimensionOfNodes; j++)
                //Generate 2-opt exchanges for each pair
                for (int k = 0; k < dimensionOfNodes - 1; k++)
                    for (int l = k + 1; l < dimensionOfNodes; l++)
                    {
                        // Check if i, j, k, l are distinct and not adjacent
                        if ((i == j || j == i + 1) || (k == l || l == k + 1) || (i == k || j == l) || (l == i + 1 || j == k + 1))
                            continue;
                        // Perform 2-opt exchange to generate new tour
                        memcpy(newSolution, bestSolution, dimensionOfNodes * sizeof(Node));

                        // Add edges (i, k) and (j, l)
                        swap(newSolution[j], newSolution[k]);

                        // Calculate the new cost in tour length
                        newCost = solutionCost(newSolution, tsp->nodesDistance, dimensionOfNodes);

                        // Check if the new tour is better
                        if (newCost < bestCost)
                        {
                            bestCost = newCost;
                            improvement = true;
                            memcpy(bestSolution, newSolution, dimensionOfNodes * sizeof(Node));
                        }
                    }
    }
    delete[] newSolution;
}

/**********************************************************************************************************************/
/*(iv) Acceptance criterion methods*/
void better(Scanner* tsp, Node* &perturbedSolution, Node* &threadsSolution, int threadID)
{
    /*Check if the new route is better than the previous one*/
    if (solutionCost(perturbedSolution, tsp->nodesDistance, tsp->dimensionOfNodes) <
        solutionCost(threadsSolution, tsp->nodesDistance, tsp->dimensionOfNodes, threadID))
        /*If the new route is better than the previous one, update the best route*/
        copyArrayToMatrix(threadsSolution, perturbedSolution, threadID, tsp->dimensionOfNodes);
}

/**********************************************************************************************************************/
/*(v) Auxiliary methods*/
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes)
{
    double cost = 0;
    for (int i = 0; i < dimensionOfNodes - 1; i++)
        cost += nodesDistance[(solution[i].ID * dimensionOfNodes) + solution[i + 1].ID].distanceValue;
    cost += nodesDistance[(solution[dimensionOfNodes - 1].ID * dimensionOfNodes) + solution[0].ID].distanceValue;
    return cost;
}

double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes, int threadID)
{
    double cost = 0;
    for (int i = 0; i < dimensionOfNodes - 1; i++)
        cost += nodesDistance[(solution[(dimensionOfNodes * threadID) + i].ID * dimensionOfNodes) + solution[(dimensionOfNodes * threadID) + i + 1].ID].distanceValue;
    cost += nodesDistance[(solution[(dimensionOfNodes * threadID) + dimensionOfNodes - 1].ID * dimensionOfNodes) + solution[(dimensionOfNodes * threadID) + 0].ID].distanceValue;
    return cost;
}

void copyMatrixToArray(Node* &destination, Node* &source, int origin, int size)
{
    for (int i = 0; i < size; i++)
        destination[i] = source[(size * origin) + i];
}

void copyArrayToMatrix(Node* &destination, Node* &source, int origin, int size)
{
    for (int i = 0; i < size; i++)
        destination[(size * origin) + i] = source[i];
}

void findBestSolution(Node* &solutions, Node* &nodesDistance, Node* &bestSolution, int dimensionOfNodes, int numberOfSolutions)
{
    int threadID = 0;
    double bestSolutionValue = solutionCost(solutions, nodesDistance, dimensionOfNodes, threadID);
    for (int i = 0; i < numberOfSolutions -1; i++)
        if (solutionCost(solutions, nodesDistance, dimensionOfNodes, i + 1) < bestSolutionValue)
        {
            bestSolutionValue = solutionCost(solutions, nodesDistance, dimensionOfNodes, i + 1);
            threadID = i + 1;
        }
    copyMatrixToArray(bestSolution, solutions, threadID, dimensionOfNodes);
}

void printResult(Node* bestSolution, char* fileName, double elapsedTime, double cost, int dimensionOfNodes, int iterations)
{
    //Writing the last result in the file
    ofstream outputFile(fileName);
    if (outputFile.is_open())
    {
        outputFile << "Iterations: " << iterations;
        outputFile << " Time: " << fixed << setprecision(4) << elapsedTime << " sec - " << elapsedTime/60 << " min - " << elapsedTime/3600 << " horas" << endl;
        outputFile << floor(elapsedTime/3600) << " h " << ((elapsedTime/3600) - floor(elapsedTime/3600)) * 60 << " min" << endl ;
        outputFile << "Problem dimension: " << dimensionOfNodes << endl;
        outputFile << "\nTotal distance: " << cost << endl;

        for (int i = 0; i < dimensionOfNodes; i++)
            outputFile << "[" << bestSolution[i].ID << "] ";
    }
    outputFile.close();
}

/**********************************************************************************************************************/
/*(vi) Debug methods*/
void printTableOfDouble(double *matrix, int dimensionOfNodes) {
    for (int in = 0; in < dimensionOfNodes; in++)
    {
        for (int jn = 0; jn < dimensionOfNodes; jn++)
        {
            if (matrix[(in * dimensionOfNodes) + jn] < 10)
                cout << "|0" << fixed << setprecision(4) << matrix[(in * dimensionOfNodes) + jn] << "|  ";
            else
                cout << "|" << fixed << setprecision(4) << matrix[(in * dimensionOfNodes) + jn] << "|  ";
        }
        cout << endl;
    }
    cout << endl;
}

void printTableOfNode(Node *matrix, int dimensionOfNodes) {
    for (int in = 0; in < dimensionOfNodes; in++)
    {
        for (int jn = 0; jn < dimensionOfNodes; jn++)
        {
            if (matrix[(in * dimensionOfNodes) + jn].distanceValue < 10)
                cout << "|0" << fixed << setprecision(4) << matrix[(in * dimensionOfNodes) + jn].distanceValue << "|  ";
            else
                cout << "|" << fixed << setprecision(4) << matrix[(in * dimensionOfNodes) + jn].distanceValue << "|  ";
        }
        cout << endl;
    }
    cout << endl;
}

void printTableOfNode(Node *matrix, int dimensionOfNodes, int threadsNum) {
    for (int in = 0; in < threadsNum; in++)
    {
        for (int jn = 0; jn < dimensionOfNodes; jn++)
        {
            cout << "[" << matrix[(dimensionOfNodes * in) + jn].ID << "] " ;
        }
        cout << endl;
    }
    cout << endl;
}


void printListOfNode(Node *list, int dimensionOfNodes) {

    for (int in = 0; in < dimensionOfNodes; in++)
        cout << "[" << list[in].ID << "] ";
    cout << endl;

}

void printListOfDouble(double *list, int dimensionOfNodes) {

    for (int in = 0; in < dimensionOfNodes; in++)
        cout << "[" << list[in] << "] ";
    cout << endl;

}

