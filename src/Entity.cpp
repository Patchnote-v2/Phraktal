#include "Entity.h"

Entity::Entity(Camera &camera, int x, int y) :
        camera(camera),
        pos((float) x, (float) y),
        center(0.0f, 0.0f),
        aim(0.0f, 0.0f),
        angle(0.0f),
        type(Type::SIMPLE_ENTITY)
{
    if (this->texture)
    {
        this->updateCenter();
    }
}

void Entity::loadTexture(std::string name)
{
    this->texture->loadTexture(name);
    this->updateCenter();
}

std::unique_ptr< SDL_Rect > Entity::getRect()
{
    std::unique_ptr< SDL_Rect > rect(new SDL_Rect());

    rect->x = (int) this->pos.x;
    rect->y = (int) this->pos.y;
    rect->w = this->texture->getWidth();
    rect->h = this->texture->getHeight();

    return std::move(rect);
}

void Entity::setPos(int x, int y)
{
    this->pos.x = x;
    this->pos.y = y;
    this->updateCenter();
    this->updateAngle();
}

Vector2 Entity::getPos() const
{
    return this->pos;
}

Vector2 Entity::getCenter() const
{
    return this->center;
}

Entity::Type Entity::getType() const
{
    return this->type;
}

int Entity::getAngle() const
{
    int angle = (int) (std::atan2(this->center.y + this->camera.pos.y - (this->aim.y + this->camera.pos.y),
                                     this->center.x + this->camera.pos.x - (this->aim.x + this->camera.pos.x)) * (180 / phraktal::levels::PI)) - 90;
    if (angle < 0)
    {
        angle = 360 - (-angle);
    }

    return angle;
}

bool Entity::isInLevel()
{
    return this->pos.x < phraktal::levels::LEVEL_WIDTH + this->texture->getWidth() &&
             this->pos.x > 0 - this->texture->getWidth() &&
             this->pos.y < phraktal::levels::LEVEL_HEIGHT + this->texture->getHeight() &&
             this->pos.y > 0 - this->texture->getHeight();
}

bool Entity::isColliding(std::shared_ptr< Entity > m2)
{
    // Detects collision based on SDL_Rect of each Entity
    auto rect1 = this->getRect();
    auto rect2 = m2->getRect();
    return SDL_HasIntersection(rect1.get(), rect2.get());
}

void Entity::update(float)
{
    this->updateCenter();
    this->updateAngle();
}

/*
 * Protected methods
 */

void Entity::updateCenter()
{
    // Updates center based on texture size
    if (this->texture)
    {
        this->center.x = this->pos.x + (this->texture->getWidth() / 2);
        this->center.y = this->pos.y + (this->texture->getHeight() / 2);
    }
}

void Entity::updateAngle()
{
    this->angle = (std::atan2(this->center.y + this->camera.pos.y - (this->aim.y + this->camera.pos.y),
                              this->center.x + this->camera.pos.x - (this->aim.x + this->camera.pos.x)) * (180 / phraktal::levels::PI)) - 90;
    if (this->angle < 0)
    {
        this->angle = 360 - (-angle);
    }
}

void Entity::render()
{
    this->texture->renderTexture((int) this->pos.x - (int) this->camera.pos.x,
                                 (int) this->pos.y - (int) this->camera.pos.y, NULL, this->angle, NULL,
                                 SDL_FLIP_NONE);

}
