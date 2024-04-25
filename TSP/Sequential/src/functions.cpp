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

void doubleBridgeMoveImproved(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes)
{
    //Produce a high-quality random numbers source
    random_device rd;
    //A 32-bit Mersenne Twister-based random number generator
    mt19937 gen(rd());
    //A probability distribution that generates integers uniformly distributed within a range from 1 to dimensionOfNodes - 4
    uniform_int_distribution<> dis(1, dimensionOfNodes - 4);
    // Randomly choose four cut points
    int cutPoint1 = dis(gen);
    int cutPoint2 = cutPoint1 + dis(gen);
    int cutPoint3 = cutPoint2 + dis(gen);

    // Segment 1: city 0 up to the first cut point
    initialSolution->copy(initialSolution, initialSolution + cutPoint1, perturbedSolution);

    // Segment 2: third cut point up to the fourth cut point
    initialSolution->copy(initialSolution + cutPoint3, initialSolution + cutPoint3 + (cutPoint2 - cutPoint1), perturbedSolution + cutPoint1);

    // Segment 3: second cut point up to the third cut point
    initialSolution->copy(initialSolution + cutPoint2, initialSolution + cutPoint3, perturbedSolution + cutPoint1 + (cutPoint2 - cutPoint1));

    // Segment 4: first cut point up to the second cut point
    initialSolution->copy(initialSolution + cutPoint1, initialSolution + cutPoint2, perturbedSolution + cutPoint1 + (cutPoint3 - cutPoint2));

    // Segment 5: fourth cut point up to the end
    initialSolution->copy(initialSolution + cutPoint1 + (cutPoint3 - cutPoint2), initialSolution + dimensionOfNodes, perturbedSolution + cutPoint1 + (cutPoint3 - cutPoint2));

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
        for (int i = 1; i < dimensionOfNodes - 2; i++)
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
/*void threeOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes)
{
    Node* newSolution = new Node[dimensionOfNodes];
    memcpy(bestSolution, solution, dimensionOfNodes * sizeof(Node));

    double bestCost = solutionCost(bestSolution, tsp->nodesDistance, dimensionOfNodes);
    double newCost;
    bool improvement = true;

    while(improvement)
    {
        improvement = false;
        for (int i = 0; i < dimensionOfNodes - 2 ; i++)
            for (int j = i + 1; j < dimensionOfNodes - 1; j++)
                for (int k = j + 1; k < dimensionOfNodes; k++)
                {
                    memcpy(newSolution, bestSolution, dimensionOfNodes * sizeof(Node));

                    //Remove edges (i, i+1), (j, j+1), and (k, k+1)
                    int nextI = (i + 1) % dimensionOfNodes; // Handle wrap-around for indices
                    int nextJ = (j + 1) % dimensionOfNodes;
                    int nextK = (k + 1) % dimensionOfNodes;

                    //Try all eight possible reconnections:

                }
    }
    delete[] newSolution;
}*/

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
void better(Scanner* tsp, Node* &perturbedSolution, Node* &bestSolution, int dimensionOfNodes)
{
    /*Check if the new route is better than the previous one*/
    if (solutionCost(perturbedSolution, tsp->nodesDistance, tsp->dimensionOfNodes) <
        solutionCost(bestSolution, tsp->nodesDistance, tsp->dimensionOfNodes))
        /*If the new route is better than the previous one, update the best route*/
        memcpy(bestSolution, perturbedSolution, tsp->dimensionOfNodes * sizeof(Node));
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

void printResult(Node* bestSolution, const char* fileName, double elapsedTime, double cost, int dimensionOfNodes, int iterations)
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

void printListOfNode(Node *list, int dimensionOfNodes)
{

    for (int in = 0; in < dimensionOfNodes; in++)
        cout << "[" << list[in].ID << "] ";

    cout << endl;

}
void printListOfNodeWeight(Node *list, int dimensionOfNodes)
{

    for (int in = 0; in < dimensionOfNodes; in++)
    {
        cout << "[" << list[in].ID << "]-> " << list[in].distanceValue << " " << endl;
    }
    cout << endl;

}

void printListOfDouble(double *list, int dimensionOfNodes)
{

    for (int in = 0; in < dimensionOfNodes; in++)
        cout << "[" << list[in] << "] ";
    cout << endl;

}

