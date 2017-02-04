#ifndef TOPDOWN_ENEMY_H
#define TOPDOWN_ENEMY_H

#include "MoveableEntity.h"
#include "Player.h"

class Enemy : public MoveableEntity
{
public:
    float shotCooldown;

    Enemy(Camera &camera, int x, int y);

    void update(float dTime) override;
    void toggleActive();
    void setCurrentTarget(std::shared_ptr< Player > currentTarget);
    std::shared_ptr< Player > getCurrentTarget() const;

private:
    bool active;
    std::shared_ptr< Player > currentTarget;

};

#endif //TOPDOWN_ENEMY_H
