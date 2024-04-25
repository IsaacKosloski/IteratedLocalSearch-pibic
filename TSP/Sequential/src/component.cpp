#include "component.h"

Component::Component() noexcept
{

}

Component::Component(int numberOfComponents, double *positionComponents) noexcept(false)
{
    this->positionComponents = new double[numberOfComponents];
    for (int i = 0; i < numberOfComponents; i++)
        this->positionComponents[i] = positionComponents[i];
}
