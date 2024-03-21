#ifndef ITERATEDLOCALSEARCH_FUNCTIONS_H
#define ITERATEDLOCALSEARCH_FUNCTIONS_H
#include "scanner.h"
#include <omp.h>
#include <random>
#include <algorithm>
#include <cfloat>

/*(i) Initial solution methods*/
void greedyTSP(Scanner* &tsp, Node* &initialSolution);
/**********************************************************************************************************************/
/*(ii) Perturbation methods*/
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes);
void doubleBridgeMove(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes, int threadID);
void doubleBridgeMove2(Node* &initialSolution, Node* &perturbedSolution, int dimensionOfNodes, int threadID);
/**********************************************************************************************************************/
/*(iii) Local search methods*/
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes);
void twoOpt(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes, int threadID);
void linKernighan(Scanner* tsp, Node* &solution, Node* &bestSolution, int dimensionOfNodes);
/**********************************************************************************************************************/
/*(iv) Acceptance criterion methods*/
void better(Scanner* tsp, Node* &perturbedSolution, Node* &bestSolution, double &bestWeight);
void better(Scanner* tsp, Node* &perturbedSolution, Node* &threadsSolution, int threadID);
/**********************************************************************************************************************/
/*(v) Auxiliary methods*/
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes);
double solutionCost(Node* &solution, Node* &nodesDistance, int dimensionOfNodes, int threadID);
void copyMatrixToArray(Node* &destination, Node* &source, int origin, int size);
void copyArrayToMatrix(Node* &destination, Node* &source, int origin, int size);
void copyMatrixLine(Node* &destination, Node* &source, int origin, int size);
void findBestSolution(Node* &solutions, Node* &nodesDistance, Node* &bestSolution, int dimensionOfNodes, int numberOfSolutions);
void printResult(Node* bestSolution, char* fileName, double elapsedTime, double cost, int dimensionOfNodes, int iterations);
/**********************************************************************************************************************/
/*(vi) Debug methods*/
void printTableOfNode(Node *matrix, int dimensionOfNodes);
void printListOfNode(Node *list, int dimensionOfNodes);
void printTableOfNode(Node *matrix, int dimensionOfNodes, int threadsNum);
void printListOfDouble(double *list, int dimensionOfNodes);
void printTableOfDouble(double* matrix, int dimensionOfNodes);






#endif //ITERATEDLOCALSEARCH_FUNCTIONS_H
