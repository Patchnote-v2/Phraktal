#ifndef TOPDOWN_MOVEABLEENTITY_H
#define TOPDOWN_MOVEABLEENTITY_H

#include "Entity.h"

class MoveableEntity : public Entity
{
public:
    MoveableEntity(Camera &camera, int x, int y);

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

    Vector2 velocity;
    Vector2 destination;

    float maxSpeed;
};
#endif //TOPDOWN_MOVEABLEENTITY_H
