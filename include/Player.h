#ifndef PHRAKTAL_PLAYER_H
#define PHRAKTAL_PLAYER_H

#include <map>

#include "SDL2/SDL.h"

#include "MoveableEntity.h"
#include "Powerup.h"

const int MAX_X_VEL = 512;
const int MAX_Y_VEL = 512;

class Player : public MoveableEntity
{
public:

    Player(Camera &camera, int x, int y);
    ~Player()
    {};

    void handleEvents(SDL_Event& e);
    void handleKeystates(const Uint8* keystates);
    void update(float dTime) override;

    void setPowerupType(Powerup::PowerupType powerupType);
    Powerup::PowerupType getPowerupType() const;
    void removePowerup();

    void resetShotCooldown();
    int getShotCooldown() const;
    int getMaxShotCooldownTime() const;
    bool canFire() const;

private:
    float accelerationSpeed;
    float decelerationSpeed;

    Powerup::PowerupType powerupType;
    int shotCooldown;
    int maxShotCooldownTime;

    std::map< SDL_Scancode, bool > keystates;
};

#endif //PHRAKTAL_PLAYER_H
