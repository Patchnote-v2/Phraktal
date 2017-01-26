#include "entities/mimic.h"

MoveableEntity::MoveableEntity(std::shared_ptr< Camera > camera) : camera(camera), texture(new TextureW()), oldPos(0.f, 0.f), pos(0.f, 0.f), center(0.f, 0.f), aim(0.f, 0.f), velocity(0.f, 0.f), destination(0.f, 0.f)
{
    this->center = {0, 0};
    this->angle = 0.f;
}


Vector2 phraktal::MoveableEntity::getOldPos()
{
    return Vector2(this->oldPos);
}

bool phraktal::MoveableEntity::hasMoved()
{
    return ((int) this->pos.x != (int) this->oldPos.x) || ((int) this->pos.y != (int) this->oldPos.y);
}

bool phraktal::MoveableEntity::checkCollision(std::shared_ptr< MoveableEntity > m2)
{
    std::unique_ptr< SDL_Rect > rect1 = this->getRect();
    std::unique_ptr< SDL_Rect > rect2 = m2->getRect();
    return SDL_HasIntersection(rect1.get(), rect2.get());
}

void phraktal::MoveableEntity::update(float)
{
    //
}

void phraktal::MoveableEntity::render()
{
    this->texture->renderTexture((int) this->pos.x - (int) this->camera->pos.x, (int) this->pos.y - (int) this->camera->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
}
