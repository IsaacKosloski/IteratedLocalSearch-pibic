#ifndef ITERATEDLOCALSEARCH_SCANNER_H
#define ITERATEDLOCALSEARCH_SCANNER_H
#include "node.h"
#include "component.h"

class Scanner
{
    public:
        int dimensionOfNodes;
        int distanceType;
        int explicitDistanceFormat;
        int displayDataType;
        int edgeWeightFormat;
        int edgeWeightType;
        int col;
        int row;
        string fileName;
        Component* components;
        Node* nodes;
        Node* nodesDistance;

        Scanner() noexcept;
        Scanner(string fileName) noexcept(false);

    void readFile(const string &fileName, Component* &components, Node* &nodes, Node* &nodesDistance);
    bool specificationPart(string line);
};


#endif //ITERATEDLOCALSEARCH_SCANNER_H
