#ifndef ITERATEDLOCALSEARCH_FUNCTIONS_H
#define ITERATEDLOCALSEARCH_FUNCTIONS_H
#include "scanner.h"
#include "node.h"
#include <random>
#include <algorithm>

/*(i) Initial solution methods*/
void greedyTSP(Scanner* &tsp, Node* &initialSolution);
/**********************************************************************************************************************/
/*(ii) Perturbation methods*/
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes);
void doubleBridgeMove2(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes);
void doubleBridgeMoveImproved(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes);
/**********************************************************************************************************************/
/*(iii) Local search methods*/
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes);
void linKernighan(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes);
/**********************************************************************************************************************/
/*(iv) Acceptance criterion methods*/
void better(Scanner* tsp, Node* &perturbedSolution, Node* &bestSolution, int dimensionOfNodes);
/**********************************************************************************************************************/
/*(v) Auxiliary methods*/
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes);
void printResult(Node* bestSolution, char* fileName, double elapsedTime, double cost, int dimensionOfNodes, int iterations);
/**********************************************************************************************************************/
/*(vi) Debug methods*/
void printTableOfNode(Node *matrix, int dimensionOfNodes);
void printListOfNode(Node *list, int dimensionOfNodes);
void printListOfNodeWeight(Node *list, int dimensionOfNodes);
void printListOfDouble(double *list, int dimensionOfNodes);
void printTableOfDouble(double* matrix, int dimensionOfNodes);

#endif //ITERATEDLOCALSEARCH_FUNCTIONS_H









