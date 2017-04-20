#ifndef PHRAKTAL_POWERUP_H_H
#define PHRAKTAL_POWERUP_H_H

#include "Entity.h"

class Powerup : public Entity
{
public:
    enum class PowerupType
    {
        NONE,
        SPREAD
    };
    Powerup(Camera &camera, int x, int y, PowerupType powerupType);
    ~Powerup() {};

    void setPowerupType(PowerupType powerupType);
    PowerupType getPowerupType() const;

private:
    PowerupType powerupType;
};

#endif //PHRAKTAL_POWERUP_H_H
