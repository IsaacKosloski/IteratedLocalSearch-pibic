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

void greedyRandomTSP(Scanner* &tsp, Node* &initialSolution)
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
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes)
{
    // Copies the original solution to the perturbed solution.
    memcpy(perturbedSolution, initialSolution, dimensionOfNodes * sizeof(Node));

    // Generates two random indices in the range [0, dimensionOfNodes - 1].
    int start1 = rand() % (dimensionOfNodes / 4);
    int end1 = start1 + (dimensionOfNodes / 4);
    int start2 = end1 + (rand() % (dimensionOfNodes / 4));
    int end2 = start2 + (dimensionOfNodes / 4);

    for(int i = start1, j = end2; i < end1; i++, j--)
        swap(perturbedSolution[i], perturbedSolution[j]);
}

void doubleBridgeMove2(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes)
{
    // Copies the original solution to the perturbed solution.
    memcpy(perturbedSolution, initialSolution, dimensionOfNodes * sizeof(Node));

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

/**********************************************************************************************************************/
/*(iv) Acceptance criterion methods*/

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

