#ifndef ITERATEDLOCALSEARCH_FUNCTIONS_H
#define ITERATEDLOCALSEARCH_FUNCTIONS_H
#include "scanner.h"
#include "node.h"
#include <omp.h>

void printTableOfNode(Node *matrix, int dimensionOfNodes);
void printListOfNode(Node *list, int dimensionOfNodes);
void printListOfDouble(double *list, int dimensionOfNodes);
void printTableOfDouble(double* matrix, int dimensionOfNodes);

void greedyTSP(Scanner* &tsp, Node* &initialSolution);
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes, int threadID);
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes);
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes, int threadID);
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes);
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes, int threadID);
void printResult(Node* bestSolution, char* fileName, double elapsedTime, double cost, int dimensionOfNodes, int iterations);
void copyMatrixToArray(Node* &destination, Node* &source, int origin, int size);
void copyArrayToMatrix(Node* &destination, Node* &source, int origin, int size);
void findBestSolution(Node* &solutions, Node* &nodesDistance, Node* &bestSolution, int dimensionOfNodes, int numberOfSolutions);

#endif //ITERATEDLOCALSEARCH_FUNCTIONS_H
