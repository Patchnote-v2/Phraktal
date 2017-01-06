#include "mimics/enemy.h"

void Enemy::setPos(int x, int y)
{
    this->pos.x = x;
    this->pos.y = y;
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;
    this->center.x = this->pos.x + (this->texture->getWidth() / 2);
    this->center.y = this->pos.y + (this->texture->getHeight() / 2);
}

void Enemy::update(float dTime)
{
    this->aim.x = this->currentTarget->getPos()->x;
    this->aim.y = this->currentTarget->getPos()->y;
    this->angle = (std::atan2(this->center.y + this->camera->pos.y - (this->aim.y + this->camera->pos.y), this->center.x + this->camera->pos.x - (this->aim.x + this->camera->pos.x)) * (180 / PI)) - 90;
    if (this->angle < 0)
    {
        this->angle = 360 - (-angle);
    }

    this->shotCooldown += dTime;
}

void Enemy::toggleActive()
{
    this->active = !this->active;
}

const std::shared_ptr< Player > Enemy::getCurrentTarget() const
{
    return this->currentTarget;
}

void Enemy::setCurrentTarget(const std::shared_ptr< Player > currentTarget)
{
    this->currentTarget = currentTarget;
}

std::unique_ptr< Bullet > Enemy::fireBullet()
{
    return std::unique_ptr< Bullet >();
}
