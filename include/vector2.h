#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>
#include "SDL2/SDL.h"

class Vector2
{
public:
    float x;
    float y;

    Vector2(float x, float y);

    void normalize();

    inline Vector2 operator=(Vector2 right)
    {
        this->x = right.x;
        this->y = right.y;
        return *this;
    }
private:
    float distance();
};

inline Vector2 operator+=(Vector2 left, Vector2 right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

inline Vector2 operator-=(Vector2 left, Vector2 right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

inline Vector2 operator*=(Vector2 left, Vector2 right)
{
    left.x *= right.x;
    left.y *= right.y;
    return left;
}

inline Vector2 operator/=(Vector2 left, Vector2 right)
{
    left.x /= right.x;
    left.y /= right.y;
    return left;
}

inline Vector2 operator+(Vector2 left, Vector2 right)
{
    return left += right;
}

inline Vector2 operator-(Vector2 left, Vector2 right)
{
    return left -= right;
}

inline Vector2 operator*(Vector2 left, Vector2 right)
{
    return left *= right;
}

inline Vector2 operator/(Vector2 left, Vector2 right)
{
    return left /= right;
}

inline bool operator==(Vector2 left, Vector2 right)
{
    return left.x == right.y && left.y == right.y;
}

inline bool operator !=(Vector2 left, Vector2 right)
{
    return !(left == right);
}

// Modified SDL_PointInRect()
inline bool Vector2InRect(const Vector2* p, const SDL_Rect* r)
{
    return ( ((int) p->x >= r->x) && ((int) p->x < (r->x + r->w)) &&
             ((int) p->y >= r->y) && ((int) p->y < (r->y + r->h)) ) ? true : false;
}
#endif