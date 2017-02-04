#include "Vector2.h"

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vector2::normalize()
{
    float distance = this->distance();
    if (distance == 0)
    {
        return;
    }

    this->x /= distance;
    this->y /= distance;
}

float Vector2::distance()
{
    return std::sqrt(
            std::pow(this->x, 2) +
            std::pow(this->y, 2));
}