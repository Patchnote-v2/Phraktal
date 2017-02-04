#include <memory>
#include "Camera.h"

Camera::Camera(int x, int y, int w, int h) : pos(x, y), size(w, h), center(0.0f, 0.0f)
{}

std::unique_ptr< SDL_Rect > Camera::getRect()
{
    std::unique_ptr< SDL_Rect > rect(new SDL_Rect());

    rect->x = (int) this->pos.x;
    rect->y = (int) this->pos.y;
    rect->w = (int) this->size.x;
    rect->h = (int) this->size.y;

    return std::move(rect);
}

void Camera::update()
{
    if (this->pos.x < 0)
    {
        this->pos.x = 0;
    }
    if (this->pos.y < 0)
    {
        this->pos.y = 0;
    }
    if (this->pos.x + this->size.x > phraktal::levels::LEVEL_WIDTH)
    {
        this->pos.x = phraktal::levels::LEVEL_WIDTH - this->size.x;
    }
    if (this->pos.y + this->size.y > phraktal::levels::LEVEL_HEIGHT)
    {
        this->pos.y = phraktal::levels::LEVEL_HEIGHT - this->size.y;
    }
    this->center.x = this->pos.x + (this->size.x / 2);
    this->center.y = this->pos.y + (this->size.y / 2);
}