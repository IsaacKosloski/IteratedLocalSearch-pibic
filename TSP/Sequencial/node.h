#ifndef ITERATEDLOCALSEARCH_NODE_H
#define ITERATEDLOCALSEARCH_NODE_H
#include <bits/stdc++.h> //This library is used to get a lot of functions and methods used by all code
#include <algorithm>

using namespace std;
using ::string;

//enumerable of edge distance (or weight) types, for class node
enum EdgeDistanceTypes   : int {EXPLICIT, EUC_2D, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D, CEIL_2D, GEO, ATT, XRAY1, XRAY2, SPECIAL};
enum EdgeDistanceFormats : int {FUNCTION, FULL_MATRIX, UPPER_ROW, LOWER_ROW, UPPER_DIAG_ROW, LOWER_DIAG_ROW, UPPER_COL, LOWER_COL, UPPER_DIAG_COL, LOWER_DIAG_COL};
enum DisplayDataTypes    : int {COORD_DISPLAY, TWOD_DISPLAY, NO_DISPLAY};


class Node
{
    public:
        //type of how the distances (or weights) are given.
        // The value, comes from an enum.
        int ID; //usable to identify a node (spot)
        int numberOfComponents;
        bool eligible; //usable to identify the nodes availability
        double distanceValue;
        double *positionComponents; // graph coordinates components

        Node() noexcept;
        Node(int ID, int numberOfComponents, double* positionComponents) noexcept(false);
        Node(int ID, bool eligible, double distanceValue) noexcept;

        void copy(Node* source, Node* sourceEnd, Node* destination);

};


#endif //ITERATEDLOCALSEARCH_NODE_H
