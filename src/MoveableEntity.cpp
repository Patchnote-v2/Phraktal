#include "MoveableEntity.h"

MoveableEntity::MoveableEntity(Camera &camera, int x, int y) :
        Entity(camera, x, y),
        oldPos(0.0f, 0.0f),
        velocity(0.0f, 0.0f),
        destination(0.0f, 0.0f),
        maxSpeed(1)
{
    this->type = Type::MOVEABLE_ENTITY;
}

void MoveableEntity::setDestination(Vector2 destination)
{
    this->destination = destination;
    this->velocity.x = this->destination.x - this->pos.x;
    this->velocity.y = this->destination.y - this->pos.y;
    this->velocity.normalize();
    this->updateAngle();
}

void MoveableEntity::setDestination(int x, int y)
{
    Vector2 destination((float) x, (float) y);
    this->setDestination(destination);
}

void MoveableEntity::setPos(int x, int y)
{
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;
    Entity::setPos(x, y);
}

Vector2 MoveableEntity::getOldPos() const
{
    return this->oldPos;
}

void MoveableEntity::setMaxSpeed(float maxSpeed)
{
    this->maxSpeed = maxSpeed;
}

bool MoveableEntity::hasMoved() const
{
//    return ((int) this->pos.x != (int) this->oldPos.x) || ((int) this->pos.y != (int) this->oldPos.y);
    if ((std::round(this->pos.x) != std::round(this->oldPos.x)) || (std::round(this->pos.y) != std::round(this->oldPos.y)))
    {
        std::cout << "--X: " << this->pos.x << " - " << std::round(this->pos.x) << std::endl;
        std::cout << "--oX: " << this->oldPos.x << " - " << std::round(this->oldPos.x) << std::endl;
        std::cout << "--Y: " << this->pos.y << " - " << std::round(this->pos.y) << std::endl;
        std::cout << "--oY: " << this->oldPos.y << " - " << std::round(this->oldPos.y) << std::endl;
    }

    return (std::round(this->pos.x) != std::round(this->oldPos.x)) || (std::round(this->pos.y) != std::round(this->oldPos.y));
}

void MoveableEntity::update(float dTime)
{
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;

    this->pos.x = std::round(this->pos.x + this->velocity.x * dTime * this->maxSpeed);
    this->pos.y += this->velocity.y * dTime * this->maxSpeed;

    Entity::update(dTime);
}