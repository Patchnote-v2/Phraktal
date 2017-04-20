#include "Bullet.h"

Bullet::Bullet(Camera &camera, int x, int y, float maxSpeed, Entity::Type type) :
        MoveableEntity(camera, x, y)
{
    if (maxSpeed < MINIMUM_SPEED)
    {
        this->maxSpeed = MINIMUM_SPEED;
    }
    else
    {
        this->maxSpeed = maxSpeed;
    }
    this->type = type;
}

void Bullet::setVelocityFromAngle(int angle)
{
    float radians = (phraktal::levels::PI / 180 * angle);
    radians -= phraktal::levels::PI / 2;
    this->velocity.x = (int) (cos(radians) * this->maxSpeed);
    this->velocity.y = (int) (sin(radians) * this->maxSpeed);
    this->velocity.normalize();
}
