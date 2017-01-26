#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "SDL2/SDL.h"

#include "mimic.h"
#include "player.h"

class Enemy : public MoveableEntity
{
public:
    float shotCooldown;

    Enemy(std::shared_ptr< Camera > camera) : MoveableEntity(camera) {setType(Type::ENEMY);};
    void update(float dTime) override;
    void setPos(int x, int y);
    void toggleActive();
    void setCurrentTarget(const std::shared_ptr< Player > currentTarget);
    const std::shared_ptr< Player > getCurrentTarget() const;
    std::unique_ptr< Bullet > fireBullet();
private:
    bool active;
    Type type;
    std::shared_ptr< Player > currentTarget;

};
#endif