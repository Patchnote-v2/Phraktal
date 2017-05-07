#include "Player.h"

Player::Player(Camera &camera, int x, int y) :
        MoveableEntity(camera, x, y),
        accelerationSpeed(30.0f),
        decelerationSpeed(0.70f),
        powerupType(Powerup::PowerupType::NONE),
        shotCooldown(0),
        maxShotCooldownTime(phraktal::levels::REGULAR_PLAYER_SHOT_COOLDOWN),
        numCoinsCollected(0)
{
    this->type = Type::PLAYER;

    this->keystates.insert(std::make_pair(SDL_SCANCODE_A, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_D, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_W, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_S, false));
}

void Player::handleEvents(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        // Right-click action
        if (e.button.button == SDL_BUTTON_RIGHT)
        {
            this->setPos((int) this->pos.x - ((int) this->pos.x - ((int) this->camera.pos.x + e.button.x)),
                         (int) this->pos.y - ((int) this->pos.y - ((int) this->camera.pos.y + e.button.y)));
        }
    }
}

void Player::handleKeystates(const Uint8* keystates)
{
    this->keystates[SDL_SCANCODE_A] = keystates[SDL_SCANCODE_A];
    this->keystates[SDL_SCANCODE_D] = keystates[SDL_SCANCODE_D];
    this->keystates[SDL_SCANCODE_W] = keystates[SDL_SCANCODE_W];
    this->keystates[SDL_SCANCODE_S] = keystates[SDL_SCANCODE_S];
}

void Player::update(float dTime)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    this->aim.x = mouseX + this->camera.pos.x;
    this->aim.y = mouseY + this->camera.pos.y;

    // Set max speed
    // S - down movement
    if (this->velocity.y > MAX_Y_VEL)
    {
        this->velocity.y = MAX_Y_VEL;
    }
    else if (this->keystates[SDL_SCANCODE_S])
    {
        this->velocity.y += accelerationSpeed;
    }

    // W - up movement
    if (this->velocity.y < -MAX_Y_VEL)
    {
        this->velocity.y = -MAX_Y_VEL;
    }
    else if (this->keystates[SDL_SCANCODE_W])
    {
        this->velocity.y -= accelerationSpeed;
    }

    // D - right movement
    if (this->velocity.x > MAX_X_VEL)
    {
        this->velocity.x = MAX_X_VEL;
    }
    else if (this->keystates[SDL_SCANCODE_D])
    {
        this->velocity.x += accelerationSpeed;
    }

    // A - left movement
    if (this->velocity.x < -MAX_X_VEL)
    {
        this->velocity.x = -MAX_X_VEL;
    }
    else if (this->keystates[SDL_SCANCODE_A])
    {
        this->velocity.x -= accelerationSpeed;
    }

    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;

    this->pos.x += this->velocity.x * (dTime);
    this->pos.y += this->velocity.y * (dTime);

    // Deceleration
    if (!this->keystates[SDL_SCANCODE_W] && !this->keystates[SDL_SCANCODE_S])
    {
        this->velocity.y *= this->decelerationSpeed;
    }
    if (!this->keystates[SDL_SCANCODE_A] && !this->keystates[SDL_SCANCODE_D])
    {
        this->velocity.x *= this->decelerationSpeed;
    }

    // Update center, angle
    this->updateCenter();
    this->updateAngle();

    // Update camera
    this->camera.pos.x = (int) this->center.x - (phraktal::levels::SCREEN_WIDTH / 2);
    this->camera.pos.y = (int) this->center.y - (phraktal::levels::SCREEN_HEIGHT / 2);
    this->camera.update();

    // Keep within level size
    if ((int) this->pos.x < 0)
    {
        this->pos.x = 0;
        this->velocity.x = -this->velocity.x;
    }
    else if ((int) this->pos.x > phraktal::levels::LEVEL_WIDTH - this->texture->getWidth())
    {
        this->pos.x = phraktal::levels::LEVEL_WIDTH - this->texture->getWidth();
        this->velocity.x = -this->velocity.x;
    }
    if ((int) this->pos.y < 0)
    {
        this->pos.y = 0;
        this->velocity.y = -this->velocity.y;
    }
    else if ((int) this->pos.y > phraktal::levels::LEVEL_HEIGHT - this->texture->getHeight())
    {
        this->pos.y = phraktal::levels::LEVEL_HEIGHT - this->texture->getHeight();
        this->velocity.y = -this->velocity.y;
    }

    // Shot cooldown
    if (this->shotCooldown < this->maxShotCooldownTime && this->shotCooldown >= 0)
    {
        this->shotCooldown += dTime * 1000;
    }
    else if (this->shotCooldown < 0 )
    {
        this->shotCooldown = 0;
    }
    if (this->shotCooldown > this->maxShotCooldownTime)
    {
        this->shotCooldown = this->maxShotCooldownTime;
    }
}

void Player::setPowerupType(Powerup::PowerupType powerupType)
{
    this->powerupType = powerupType;
    switch (this->powerupType)
    {
        case Powerup::PowerupType::NONE:
        {
            this->maxShotCooldownTime = phraktal::levels::REGULAR_PLAYER_SHOT_COOLDOWN;
            break;
        }

        case Powerup::PowerupType::SPREAD:
        {
            this->maxShotCooldownTime = phraktal::levels::SPREAD_PLAYER_SHOT_COOLDOWN;
            break;
        }

        case Powerup::PowerupType::LARGE:
        {
            this->maxShotCooldownTime = phraktal::levels::LARGE_PLAYER_SHOT_COOLDOWN;
            break;
        }
    }
}

Powerup::PowerupType Player::getPowerupType() const
{
    return this->powerupType;
}

void Player::removePowerup()
{
    this->powerupType = Powerup::PowerupType::NONE;
    this->maxShotCooldownTime = phraktal::levels::REGULAR_PLAYER_SHOT_COOLDOWN;
}

void Player::resetShotCooldown()
{
    this->shotCooldown = 0;
}

int Player::getShotCooldown() const
{
    return this->shotCooldown;
}

int Player::getMaxShotCooldownTime() const
{
    return this->maxShotCooldownTime;
}

bool Player::canFire() const
{
    return this->shotCooldown >= this->maxShotCooldownTime;
}

std::unique_ptr< SDL_Color > Player::getChargeBarColor() const
{
    std::unique_ptr< SDL_Color > color(new SDL_Color{0x00, 0x00, 0x00, 0xFF});
    int percentage = (int) (((float) this->shotCooldown / (float) this->maxShotCooldownTime) * 100.f);

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

void Player::addCoins(int numCoins)
{
    this->numCoinsCollected += numCoins;
}

void Player::resetCoins()
{
    this->numCoinsCollected = 0;
}

int Player::getCoinCount() const
{
    return this->numCoinsCollected;
}

void Player::penalizePlayerCooldown(int penaltyAmount)
{
    this->shotCooldown -= penaltyAmount;
}
