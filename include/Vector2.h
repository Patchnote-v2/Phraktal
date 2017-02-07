#ifndef PHRAKTAL_VECTOR2_H
#define PHRAKTAL_VECTOR2_H

#include <cmath>

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

inline Vector2 operator+=(Vector2 left, const Vector2 &right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

inline Vector2 operator-=(Vector2 left, const Vector2 &right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

inline Vector2 operator*=(Vector2 left, const Vector2 &right)
{
    left.x *= right.x;
    left.y *= right.y;
    return left;
}

inline Vector2 operator/=(Vector2 left, const Vector2 &right)
{
    left.x /= right.x;
    left.y /= right.y;
    return left;
}

inline Vector2 operator+(Vector2 left, const Vector2 &right)
{
    return left += right;
}

inline Vector2 operator-(Vector2 left, const Vector2 &right)
{
    return left -= right;
}

inline Vector2 operator*(Vector2 left, const Vector2 &right)
{
    return left *= right;
}

inline Vector2 operator/(Vector2 left, const Vector2 &right)
{
    return left /= right;
}

inline bool operator==(Vector2 left, const Vector2 &right)
{
    return left.x == right.y && left.y == right.y;
}

inline bool operator!=(Vector2 left, const Vector2 &right)
{
    return !(left == right);
}

#endif //PHRAKTAL_VECTOR2_H
