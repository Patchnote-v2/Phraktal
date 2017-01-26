#include "mimics/bullet.h"

void Bullet::setDestination(Vector2 destination)
{
    this->destination = destination;
    this->velocity.x = this->destination.x - this->pos.x;
    this->velocity.y = this->destination.y - this->pos.y;
    this->velocity.normalize();
    this->angle = (std::atan2(this->pos.y + this->camera->pos.y - (this->destination.y + this->camera->pos.y), this->pos.x + this->camera->pos.x - (this->destination.x + this->camera->pos.x)) * (180 / PI)) - 90;
    if (this->angle < 0)
    {
        this->angle = 360 - (-angle);
    }
}

void Bullet::setDestination(int x, int y)
{
    Vector2 destination((float) x, (float) y);
    this->setDestination(destination);
}

void Bullet::setShotPower(int shotPower)
{
    this->maxSpeed += shotPower;
}

bool Bullet::isInLevel()
{
    return !(this->pos.x < phraktal::levels::LEVEL_WIDTH + this->texture->getWidth() &&
            this->pos.x > 0 - this->texture->getWidth() &&
            this->pos.y < phraktal::levels::LEVEL_HEIGHT + this->texture->getHeight() &&
            this->pos.y > 0 - this->texture->getHeight());
}

void Bullet::update(float dTime)
{
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;

    this->pos.x += this->velocity.x * dTime * this->maxSpeed;
    this->pos.y += this->velocity.y * dTime * this->maxSpeed;
}

void Bullet::render()
{
    if (SDL_HasIntersection(this->getRect().get(), this->camera->getRect().get()))
    {
        this->texture->renderTexture((int) this->pos.x - (int) this->camera->pos.x, (int) this->pos.y - (int) this->camera->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
    }
}
