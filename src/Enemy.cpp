#include "Enemy.h"

Enemy::Enemy(Camera &camera, int x, int y, Powerup::PowerupType powerupType) :
        MoveableEntity(camera, x, y),
        powerupType(powerupType),
        health(phraktal::levels::ENEMY_REGULAR_HEALTH),
        shotCooldown(0)
{
    switch (this->powerupType)
    {
        case Powerup::PowerupType::NONE:
            this->maxShotCooldownTime = phraktal::levels::REGULAR_ENEMY_SHOT_COOLDOWN;
            this->health = phraktal::levels::ENEMY_REGULAR_HEALTH;
            this->maxHealth = phraktal::levels::ENEMY_REGULAR_HEALTH;
            break;

        case Powerup::PowerupType::SPREAD:
            this->maxShotCooldownTime = phraktal::levels::SPREAD_ENEMY_SHOT_COOLDOWN;
            this->health = phraktal::levels::ENEMY_SPREAD_HEALTH;
            this->maxHealth = phraktal::levels::ENEMY_SPREAD_HEALTH;
            break;

        case Powerup::PowerupType::LARGE:
            this->maxShotCooldownTime = phraktal::levels::LARGE_ENEMY_SHOT_COOLDOWN;
            this->health = phraktal::levels::ENEMY_LARGE_HEALTH;
            this->maxHealth = phraktal::levels::ENEMY_LARGE_HEALTH;
            break;
    }
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
    this->healthBar->setPercentage((int) (((float) this->health / (float) this->maxHealth) * 100.f));

    // Shot cooldown
    if (this->shotCooldown < this->maxShotCooldownTime)
    {
        this->shotCooldown += dTime * 1000;
    }
    if (this->shotCooldown > this->maxShotCooldownTime)
    {
        this->shotCooldown = this->maxShotCooldownTime;
    }
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
     */std::unique_ptr< SDL_Color > color(new SDL_Color{0x00, 0x00, 0x00, 0xFF});
    int percentage = (int) (((float) this->health / (float) this->maxHealth) * 100.f);

    if (percentage == 100)
    {
        color->r = 0x00;
        color->g = 0xFF;
    }
    else if (percentage >= 50)
    {
        color->r = (Uint8) ((100 - percentage) / phraktal::levels::HALF_PERCENT_TO_COLOR_CONVERSION);
        color->g = 0xFF;
    }
    else
    {
        color->r = 0xFF;
        color->g = (Uint8) (percentage / phraktal::levels::HALF_PERCENT_TO_COLOR_CONVERSION);
    }

    return std::move(color);
}

Powerup::PowerupType Enemy::getPowerupType() const
{
    return this->powerupType;
}

void Enemy::resetShotCooldown()
{
    this->shotCooldown = 0;
}

bool Enemy::canFire() const
{
    return this->shotCooldown >= this->maxShotCooldownTime;
}
