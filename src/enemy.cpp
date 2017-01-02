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