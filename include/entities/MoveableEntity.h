#ifndef MOVEABLE_ENTITY_H
#define MOVEABLE_ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <memory>

#include "Entity.h"

class Level;

namespace phraktal
{
    class MoveableEntity : public Entity
    {
    public:
        MoveableEntity(Camera& camera, int x, int y);
        ~MoveableEntity(){};

        void setDestination(Vector2 destination);
        void setDestination(int x, int y);

        void setPos(int x, int y) override;
        Vector2 getOldPos() const;

        void setMaxSpeed(float maxSpeed);

        bool hasMoved() const;
        virtual void update(float dTime) override;

    protected:
        Vector2 oldPos;

        float maxSpeed;

        Vector2 velocity;
        Vector2 destination;
    };
}
#endif