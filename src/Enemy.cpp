#include "Enemy.h"

Enemy::Enemy(Camera &camera, int x, int y) :
        MoveableEntity(camera, x, y),
        shotCooldown(0),
        health(phraktal::levels::ENEMY_HEALTH)
{
    this->type = Entity::Type::ENEMY;

    this->healthBarOutline = new SDL_Rect{x, y - 12, 52, 12};
    this->healthBarFilled = new SDL_Rect{x + 1, y - 11, 50, 10};
}

void Enemy::update(float dTime)
{
    if (this->currentTarget)
    {
        this->aim.x = this->currentTarget->getCenter().x;
        this->aim.y = this->currentTarget->getCenter().y;
    }

    int x, y;
    x = (int) this->currentTarget->getCenter().x;
    y = (int) this->currentTarget->getCenter().y;
    this->destination.x = x;
    this->destination.y = y;
    this->velocity.x = x - this->pos.x;
    this->velocity.y = y - this->pos.y;
    this->velocity.normalize();

    MoveableEntity::update(dTime);

    this->shotCooldown += dTime;

    this->healthBarFilled->w = (int) (((float) this->health / (float) phraktal::levels::ENEMY_HEALTH) * 100 / 2);

    this->healthBarOutline->x = (int) this->pos.x - (int) this->camera.pos.x;
    this->healthBarOutline->y = (int) (this->pos.y - 12) - (int) this->camera.pos.y;
    this->healthBarFilled->x = (int) (this->pos.x + 1) - (int) this->camera.pos.x;
    this->healthBarFilled->y = (int) (this->pos.y - 11) - (int) this->camera.pos.y;

}

void Enemy::toggleActive()
{
    this->active = !this->active;
}

void Enemy::setCurrentTarget(std::shared_ptr< Player > currentTarget)
{
    this->currentTarget = currentTarget;
}

std::shared_ptr< Player > Enemy::getCurrentTarget() const
{
    return this->currentTarget;
}

void Enemy::setHealth(int health)
{
    this->health = health;
}

int Enemy::getHealth() const
{
    return health;
}

std::unique_ptr< SDL_Color > Enemy::getHealthColor() const
{
    /*
     * Ratios:
     * .392f (from 0-100 to  0-255)
     * .196f (from 0-50 to 0-255)
     */
    std::unique_ptr< SDL_Color > color(new SDL_Color{0, 0, 0, 255});
    int percentage = (int) (((float) this->health / (float) phraktal::levels::ENEMY_HEALTH) * 100.f);

    if (percentage == 100)
    {
        color->r = 0;
        color->g = 255;
    }
    else if (percentage > 50)
    {
        color->r = (Uint8) ((100 - (((float) this->health / (float) phraktal::levels::ENEMY_HEALTH) * 100.f)) / phraktal::levels::HALF_PERCENT_TO_COLOR_CONVERSION);
        color->g = 255;
    }
    else
    {
        color->r = 255;
        color->g = (Uint8) (((float) this->health / (float) phraktal::levels::ENEMY_HEALTH) * 100.f / phraktal::levels::HALF_PERCENT_TO_COLOR_CONVERSION);
    }

    return std::move(color);
}
