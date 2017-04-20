#include "Player.h"

Player::Player(Camera &camera, int x, int y) :
        MoveableEntity(camera, x, y),
        accelerationSpeed(30.0f),
        decelerationSpeed(0.70f)
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
        /* Old code for waypoints
            int x;
            int y;
            SDL_GetMouseState(&x, &y);
            this->destination.x = x;
            this->destination.y = y;

            this->angle = (atan2(this->pos.y - y, this->pos.x - x) * (180 / PI)) - 90;
            if (this->angle < 0)
            {
                this->angle = 360 - (-angle);
            }

            this->velocity.x = x - this->pos.x;
            this->velocity.y = y - this->pos.y;
            this->velocity.normalize();
        */
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
}