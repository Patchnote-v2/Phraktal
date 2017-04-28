#ifndef PHRAKTAL_ENEMY_H
#define PHRAKTAL_ENEMY_H

#include <cstdlib>

#include "MoveableEntity.h"
#include "Player.h"
#include "Bar.h"

class Enemy : public MoveableEntity
{
public:
    float shotCooldown;
//    SDL_Rect* healthBarOutline;
//    SDL_Rect* healthBarFilled;

    Enemy(Camera &camera, int x, int y);

    void update(float dTime) override;
    void toggleActive();
    void setCurrentTarget(std::shared_ptr< Player > currentTarget);
    std::shared_ptr< Player > getCurrentTarget() const;

    void setHealth(int health);
    int getHealth() const;
    std::shared_ptr< Bar > getHealthBar() const;

    std::unique_ptr< SDL_Color > getHealthColor() const;

private:
    bool active;
    int health;

    std::shared_ptr< Bar > healthBar;

    std::shared_ptr< Player > currentTarget;

};

#endif //PHRAKTAL_ENEMY_H
