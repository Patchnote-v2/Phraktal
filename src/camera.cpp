#include "camera.h"

Camera::Camera(int x, int y, int w, int h) : pos(x, y), size(w, h), center(.0f, .0f)
{}

std::shared_ptr< SDL_Rect > Camera::getRect()
{
    auto rect = std::make_shared< SDL_Rect >();

    rect->x = (int) this->pos.x;
    rect->y = (int) this->pos.y;
    rect->w = (int) this->size.x;
    rect->h = (int) this->size.y;

    return rect;
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
