#include "mimic.h"

Mimic::Mimic() : oldPos(0.f, 0.f), pos(0.f, 0.f), center(0.f, 0.f), aim(0.f, 0.f), velocity(0.f, 0.f), destination(0.f, 0.f)
{
    this->texture = new TextureW();
    this->center = {0, 0};
    this->angle = 0.f;
}

void Mimic::setTexture(std::string name)
{
    this->texture->loadTexture(name);
}

void Mimic::setRenderer(SDL_Renderer* renderer)
{
    this->texture->setRenderer(renderer);
}

SDL_Rect* Mimic::getRect()
{
    SDL_Rect* rect = new SDL_Rect();

    rect->x = (int) this->pos.x;
    rect->y = (int) this->pos.y;
    rect->w = this->texture->getWidth();
    rect->h = this->texture->getHeight();

    return rect;
}

Vector2* Mimic::getOldPos()
{
    return &this->oldPos;
}

Vector2* Mimic::getPos()
{
    return &this->pos;
}

bool Mimic::hasMoved()
{
    return !almostEqual(this->pos.x, this->oldPos.x, FLT_EPSILON * 50000000) || !almostEqual(this->pos.y, this->oldPos.y, FLT_EPSILON * 50000000);
//    return (this->pos.x != this->oldPos.x || this->pos.y != this->oldPos.y);
}

bool Mimic::checkCollision(Mimic* m2)
{
    SDL_Rect* rect1 = this->getRect();
    SDL_Rect* rect2 = m2->getRect();
    return SDL_HasIntersection(rect1, rect2);
}

void Mimic::update(float)
{
    // this->pos.x += this->velocity.x;
    // this->pos.y += this->velocity.y;
}

void Mimic::render()
{
    this->texture->renderTexture((int) this->pos.x, (int) this->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
}
