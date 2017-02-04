#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "MoveableEntity.h"

namespace phraktal
{
    class Enemy : public MoveableEntity
    {
    public:
        float shotCooldown;

        Enemy(Camera& camera, int x, int y);
        void update(float dTime) override;
        void toggleActive();
        void setCurrentTarget(std::shared_ptr< Player > currentTarget);
        std::shared_ptr< Player > getCurrentTarget() const;

    private:
        bool active;
        std::shared_ptr< Player > currentTarget;

    };
}
#endif