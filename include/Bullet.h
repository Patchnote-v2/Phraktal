#ifndef PHRAKTAL_BULLET_H
#define PHRAKTAL_BULLET_H

#include <cmath>

#include "MoveableEntity.h"

class Bullet : public MoveableEntity
{
    const float MINIMUM_SPEED = 200;
public:
    Bullet(Camera &camera, int x, int y, float maxSpeed, Entity::Type type);
    ~Bullet() {};

    virtual void setDestination(int x, int y) override;

    void setVelocityFromAngle(int angle);

    void update(float) override;
};

#endif //PHRAKTAL_BULLET_H
