#include "vector2.h"

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

    this->x = this->x / distance;
    this->y = this->y / distance;
}

float Vector2::distance()
{
    return std::sqrt(
        std::pow(this->x, 2) +
        std::pow(this->y, 2));
        // std::pow(this->pos.x - this->destination.x, 2) +
        // std::pow(this->pos.y - this->destination.y, 2));
}