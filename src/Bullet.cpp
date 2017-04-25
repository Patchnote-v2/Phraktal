#include "Bullet.h"

Bullet::Bullet(Camera &camera, int x, int y, float maxSpeed, Entity::Type type, Powerup::PowerupType powerupType) :
        MoveableEntity(camera, x, y),
        powerupType(powerupType)
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

void Bullet::setDestination(int x, int y)
{
    MoveableEntity::setDestination(x, y);
    this->aim.x = x;
    this->aim.y = y;
    this->updateAngle();
}

void Bullet::setVelocityFromAngle(int angle)
{
    float radians = (phraktal::levels::PI / 180 * angle);
    radians -= phraktal::levels::PI / 2;
    this->velocity.x = (int) (cos(radians) * this->maxSpeed);
    this->velocity.y = (int) (sin(radians) * this->maxSpeed);
    this->velocity.normalize();
}

void Bullet::update(float dTime)
{
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;

    this->pos.x += this->velocity.x * dTime * this->maxSpeed;
    this->pos.y += this->velocity.y * dTime * this->maxSpeed;

    this->updateCenter();
}

int Bullet::getDamage()
{
    switch (this->powerupType)
    {
        case Powerup::PowerupType::NONE:
            return phraktal::levels::REGULAR_SHOT_DAMAGE;

        case Powerup::PowerupType::SPREAD:
            return phraktal::levels::SPREAD_SHOT_DAMAGE;

        case Powerup::PowerupType::LARGE:
            return phraktal::levels::LARGE_SHOT_DAMAGE;

        default:
            return phraktal::levels::REGULAR_SHOT_DAMAGE;
    }
}
