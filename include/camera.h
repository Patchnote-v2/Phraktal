#ifndef TOPDOWN_CAMERA_H
#define TOPDOWN_CAMERA_H

#include <memory>
#include <base/utils.h>
#include "base/vector2.h"

class Camera
{
public:
    Vector2 pos;
    Vector2 size;
    Vector2 center;

    Camera(int x, int y, int w, int h);
    std::shared_ptr< SDL_Rect > getRect();
    void update();
};

#endif //TOPDOWN_CAMERA_H