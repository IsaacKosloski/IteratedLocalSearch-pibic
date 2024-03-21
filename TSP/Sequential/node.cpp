#include "node.h"

Node::Node() noexcept
{

}


Node::Node(int ID, bool eligible, double distanceValue) noexcept
{
    this->ID = ID;
    this->eligible = eligible;
    this->distanceValue = distanceValue;
}

void
Node::copy(Node *source, Node *sourceEnd, Node *destination)
{
    while (source != sourceEnd)
    {
        *destination = *source;
        source++;
        destination++;
    }
}