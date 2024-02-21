#include "node.h"

Node::Node() noexcept
{

}

Node::Node(int ID, int numberOfComponents, double *positionComponents) noexcept(false)
{
    this->ID = ID;
    this->positionComponents = new double[numberOfComponents];
    for (int i = 0; i < numberOfComponents; i++)
        this->positionComponents[i] = positionComponents[i];
    this->eligible = true;
}

Node::Node(int ID, bool eligible, double distanceValue) noexcept
{
    this->ID = ID;
    this->eligible = eligible;
    this->distanceValue = distanceValue;
}
