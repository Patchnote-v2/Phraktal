#include "Enemy.h"

Enemy::Enemy(Camera &camera, int x, int y) :
        MoveableEntity(camera, x, y),
        shotCooldown(0),
        health(phraktal::levels::ENEMY_HEALTH)
{
    this->type = Entity::Type::ENEMY;
    this->healthBar = std::make_shared< Bar >(x, y - 20, 50, 10);
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

    this->healthBar->setPos((int) (this->pos.x - this->camera.pos.x), (int) (this->pos.y - 10 - this->camera.pos.y));
    this->healthBar->setPercentage((int) (((float) this->health / (float) phraktal::levels::ENEMY_HEALTH) * 100.f));
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

std::shared_ptr< Bar > Enemy::getHealthBar() const
{
    return this->healthBar;
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
