#include "BackgroundEntity.h"

BackgroundEntity::BackgroundEntity(Camera &camera, int x, int y) :
        Entity(camera, x, y)
{
    this->type = Entity::Type::BACKGROUND_ENTITY;
}