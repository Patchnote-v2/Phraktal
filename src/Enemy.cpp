#include "Enemy.h"

Enemy::Enemy(Camera &camera, int x, int y) :
        MoveableEntity(camera, x, y),
        shotCooldown(0)
{
    this->type = Entity::Type::ENEMY;
}

void Enemy::update(float dTime)
{
    if (this->currentTarget)
    {
        this->aim.x = this->currentTarget->getCenter().x;
        this->aim.y = this->currentTarget->getCenter().y;
    }

    MoveableEntity::update(dTime);

    this->shotCooldown += dTime;
}

void Enemy::toggleActive()
{
    this->active = !this->active;
}

void Enemy::setCurrentTarget(std::shared_ptr< Player > currentTarget)
{
    this->currentTarget = currentTarget;
}

std::shared_ptr< Player > Enemy::getCurrentTarget() const
{
    return this->currentTarget;
}