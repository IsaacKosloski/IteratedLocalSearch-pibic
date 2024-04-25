#ifndef ITERATEDLOCALSEARCH_NODE_H
#define ITERATEDLOCALSEARCH_NODE_H
#include <bits/stdc++.h> //This library is used to get a lot of functions and methods used by all code
#include <algorithm>

using namespace std;
using ::string;

class Node
{
    public:
        //type of how the distances (or weights) are given.
        // The value, comes from an enum.
        int ID; //usable to identify a node (spot)
        bool eligible; //usable to identify the nodes availability
        double distanceValue;

        Node() noexcept;
        Node(int ID, bool eligible, double distanceValue) noexcept;

        void copy(Node* source, Node* sourceEnd, Node* destination);

};


#endif //ITERATEDLOCALSEARCH_NODE_H
