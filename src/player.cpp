#include "mimics/player.h"

Player::Player(std::shared_ptr< Camera > camera) : Mimic(camera), type(Type::PLAYER), accelerationSpeed(15)
{
    this->keystates.insert(std::make_pair(SDL_SCANCODE_A, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_D, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_W, false));
    this->keystates.insert(std::make_pair(SDL_SCANCODE_S, false));

}

void Player::handleEvents(SDL_Event& e)
{
    // Type pretedermined
    if (e.type == SDL_MOUSEMOTION)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        this->aim.x = x + this->camera->pos.x;
        this->aim.y = y + this->camera->pos.y;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        // Right-click action
        if (e.button.button == SDL_BUTTON_RIGHT)
        {
            this->setPos((int) this->pos.x - ((int) this->pos.x - ((int) this->camera->pos.x + e.button.x)),
                         (int) this->pos.y - ((int) this->pos.y - ((int) this->camera->pos.y + e.button.y)));
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

//    if (this->destination.x == 0 && !keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D])
//    {
//        this->keystates[SDL_SCANCODE_D] -= 2;
//    }
//
//    if (this->destination.y == 0 && !keystates[SDL_SCANCODE_W] && !keystates[SDL_SCANCODE_S])
//    {
//        this->keystates[SDL_SCANCODE_S] -= 2;
//    }

}

void Player::update(float dTime)
{
    // Set max speed
    if (this->velocity.x > MAX_X_VEL)
    {
        this->velocity.x = MAX_X_VEL;
    }
    if (this->velocity.x < -MAX_X_VEL)
    {
        this->velocity.x = -MAX_X_VEL;
    }

    if (this->velocity.y > MAX_Y_VEL)
    {
        this->velocity.y = MAX_Y_VEL;
    }
    if (this->velocity.y < -MAX_Y_VEL)
    {
        this->velocity.y = -MAX_Y_VEL;
    }

    if (this->keystates[SDL_SCANCODE_A])
    {
        this->velocity.x -= accelerationSpeed;
    }
    if (this->keystates[SDL_SCANCODE_D])
    {
        this->velocity.x += accelerationSpeed;
    }
    if (this->keystates[SDL_SCANCODE_W])
    {
        this->velocity.y -= accelerationSpeed;
    }
    if (this->keystates[SDL_SCANCODE_S])
    {
        this->velocity.y += accelerationSpeed;
    }

    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;

    this->pos.x += this->velocity.x * (dTime);
    this->pos.y += this->velocity.y * (dTime);

    // Deceleration
    this->velocity.x *= .98f;
    this->velocity.y *= .98f;

    // Update center, angle
    this->updateCenter();
    this->updateAngle();

    // Update camera
    this->camera->pos.x = (int) this->center.x - (phraktal::levels::SCREEN_WIDTH / 2);
    this->camera->pos.y = (int) this->center.y - (phraktal::levels::SCREEN_HEIGHT / 2);

    // Keep within level size
    if ((int) this->pos.x <= 0)
    {
        this->velocity.x = -this->velocity.x;
    }
    else if ((int) this->pos.x > phraktal::levels::LEVEL_WIDTH - this->texture->getWidth())
    {
        this->velocity.x = -this->velocity.x;
    }
    if ((int) this->pos.y <= 0)
    {
        this->velocity.y = -this->velocity.y;
    }
    else if ((int) this->pos.y > phraktal::levels::LEVEL_HEIGHT - this->texture->getHeight())
    {
        this->velocity.y = -this->velocity.y;
    }
}


void Player::render()
{
    this->texture->renderTexture((int) this->pos.x - (int) this->camera->pos.x, (int) this->pos.y - (int) this->camera->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
}

void Player::setPos(int x, int y)
{
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;
    this->pos.x = x;
    this->pos.y = y;
    this->updateCenter();
    this->updateAngle();
}

Type Player::getType()
{
    return this->type;
}

void Player::updateCenter()
{
    this->center.x = this->pos.x + (this->texture->getWidth() / 2);
    this->center.y = this->pos.y + (this->texture->getHeight() / 2);
}

void Player::updateAngle()
{
    this->angle = (std::atan2(this->center.y + this->camera->pos.y - (this->aim.y + this->camera->pos.y), this->center.x + this->camera->pos.x - (this->aim.x + this->camera->pos.x)) * (180 / PI)) - 90;
    if (this->angle < 0)
    {
        this->angle = 360 - (-angle);
    }
}
