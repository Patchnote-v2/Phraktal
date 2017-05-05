#ifndef PHRAKTAL_ENEMY_H
#define PHRAKTAL_ENEMY_H

#include <cstdlib>

#include "SDL2/SDL.h"

#include "MoveableEntity.h"
#include "Player.h"
#include "Bar.h"

class Enemy : public MoveableEntity
{
public:

    Enemy(Camera &camera, int x, int y, Powerup::PowerupType powerupType);

    void update(float dTime) override;
    void toggleActive();
    void setCurrentTarget(std::shared_ptr< Player > currentTarget);
    std::shared_ptr< Player > getCurrentTarget() const;

    void setHealth(int health);
    int getHealth() const;
    std::shared_ptr< Bar > getHealthBar() const;

    std::unique_ptr< SDL_Color > getHealthColor() const;

    Powerup::PowerupType getPowerupType() const;

    void resetShotCooldown();
    bool canFire() const;

private:
    Powerup::PowerupType powerupType;
    bool active;
    int health;
    int maxHealth;
    int shotCooldown;
    int maxShotCooldownTime;

    std::shared_ptr< Bar > healthBar;

    std::shared_ptr< Player > currentTarget;

};

#endif //PHRAKTAL_ENEMY_H
