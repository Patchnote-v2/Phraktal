#ifndef TOPDOWN_BACKGROUNDENTITY_H
#define TOPDOWN_BACKGROUNDENTITY_H

#include "Entity.h"

class BackgroundEntity : public Entity
{
public:
    BackgroundEntity(Camera &camera, int x, int y);
    ~BackgroundEntity()
    {};
};

#endif
