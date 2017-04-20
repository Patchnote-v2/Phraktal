#include "Powerup.h"

Powerup::Powerup(Camera &camera, int x, int y, PowerupType powerupType) :
        Entity(camera, x, y),
        powerupType(powerupType)
{
    this->type = Entity::Type::POWERUP;
}

void Powerup::setPowerupType(Powerup::PowerupType powerupType)
{
    this->powerupType = powerupType;
}

Powerup::PowerupType Powerup::getPowerupType() const
{
    return this->powerupType;
}
