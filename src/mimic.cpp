#include "mimics/mimic.h"

Mimic::Mimic(std::shared_ptr< Camera > camera) : camera(camera), texture(new TextureW()), oldPos(0.f, 0.f), pos(0.f, 0.f), center(0.f, 0.f), aim(0.f, 0.f), velocity(0.f, 0.f), destination(0.f, 0.f)
{
    this->center = {0, 0};
    this->angle = 0.f;
}

void Mimic::setTexture(std::string name)
{
    this->texture->loadTexture(name);
}

void Mimic::setRenderer(std::shared_ptr< SDL_Renderer > renderer)
{
    this->texture->setRenderer(renderer);
}

std::unique_ptr< SDL_Rect > Mimic::getRect()
{
    std::unique_ptr< SDL_Rect > rect(new SDL_Rect());

    rect->x = (int) this->pos.x;
    rect->y = (int) this->pos.y;
    rect->w = this->texture->getWidth();
    rect->h = this->texture->getHeight();

    return std::move(rect);
}

std::shared_ptr< Vector2 > Mimic::getOldPos()
{
    return std::make_shared< Vector2 >(this->oldPos);
}

std::shared_ptr< Vector2 > Mimic::getPos()
{
    return std::make_shared< Vector2 >(this->pos);
}
std::shared_ptr< Vector2 > Mimic::getCenter()
{
    return std::make_shared< Vector2 >(this->center);
}

void Mimic::setType(Type type)
{
    this->type = type;
}

Type Mimic::getType() const
{
    return this->type;
}

bool Mimic::hasMoved()
{
    return ((int) this->pos.x != (int) this->oldPos.x) || ((int) this->pos.y != (int) this->oldPos.y);
}

bool Mimic::checkCollision(std::shared_ptr< Mimic > m2)
{
    std::unique_ptr< SDL_Rect > rect1 = this->getRect();
    std::unique_ptr< SDL_Rect > rect2 = m2->getRect();
    return SDL_HasIntersection(rect1.get(), rect2.get());
}

void Mimic::update(float)
{
    //
}

void Mimic::render()
{
    this->texture->renderTexture((int) this->pos.x - (int) this->camera->pos.x, (int) this->pos.y - (int) this->camera->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
}
