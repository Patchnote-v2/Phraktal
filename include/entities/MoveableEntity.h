#ifndef MIMIC_H
#define MIMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <memory>

#include "SDL2/SDL.h"
#include "base/texturew.h"
#include "base/vector2.h"
#include "base/utils.h"
#include "level.h"
#include "camera.h"

class Level;

namespace phraktal
{
    class MoveableEntity
    {
    public:
        MoveableEntity(std::shared_ptr< Camera > camera);

        ~MoveableEntity()
        {};

        Vector2 getOldPos();
        bool hasMoved();

        virtual bool checkCollision(std::shared_ptr< MoveableEntity > m2);

        virtual void handleEvents(SDL_Event&)
        {};

        virtual void update(float);

        virtual void render();

    protected:
        Camera& camera;
        std::unique_ptr< TextureW > texture;
        Vector2 oldPos;
        Vector2 pos;
        Vector2 center;
        double angle;
        Vector2 aim;

        Vector2 velocity;
        Vector2 destination;

    };
}
#endif