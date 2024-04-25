#ifndef ITERATEDLOCALSEARCH_PIBIC_COMPONENT_H
#define ITERATEDLOCALSEARCH_PIBIC_COMPONENT_H
#include <bits/stdc++.h> //This library is used to get a lot of functions and methods used by all code

using namespace std;
using ::string;

//enumerable of edge distance (or weight) types, for class node
enum EdgeDistanceTypes   : int {EXPLICIT, EUC_2D, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D, CEIL_2D, GEO, ATT, XRAY1, XRAY2, SPECIAL};
enum EdgeDistanceFormats : int {FUNCTION, FULL_MATRIX, UPPER_ROW, LOWER_ROW, UPPER_DIAG_ROW, LOWER_DIAG_ROW, UPPER_COL, LOWER_COL, UPPER_DIAG_COL, LOWER_DIAG_COL};
enum DisplayDataTypes    : int {COORD_DISPLAY, TWOD_DISPLAY, NO_DISPLAY};

class Component
{
    public:
        //type of how the distances (or weights) are given.
        // The value, comes from an enum.
        int numberOfComponents;
        double *positionComponents; // graph coordinates components

        Component() noexcept;
        Component(int numberOfComponents, double* positionComponents) noexcept(false);

};


#endif //ITERATEDLOCALSEARCH_PIBIC_COMPONENT_H
