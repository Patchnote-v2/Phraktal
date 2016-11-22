#include "player.h"

void Player::handleEvents(SDL_Event& e)
{
    // Type pretedermined
    
    if (e.type == SDL_MOUSEMOTION)
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        this->aim.x = x;
        this->aim.y = y;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_RIGHT)
        {
            this->oldPos.x = this->pos.x;
            this->oldPos.y = this->pos.y;
            this->pos.x = e.button.x;
            this->pos.y = e.button.y;
            this->center.x = this->pos.x + (this->texture->getWidth() / 2);
            this->center.y = this->pos.y + (this->texture->getHeight() / 2);
            this->angle = (std::atan2(this->center.y - this->pos.y, this->center.x - this->pos.x) * (180 / PI)) - 90;
            if (this->angle < 0)
            {
                this->angle = 360 - (-angle);
            }
        }
        else if (e.button.button == SDL_BUTTON_LEFT)
        {
            this->fireBullet();

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
}

void Player::handleKeystates(const Uint8* keystates)
{
    if (keystates[SDL_SCANCODE_A])
    {
        // std::cout << "a";
        this->velocity.x -= 1;
    }
    if (keystates[SDL_SCANCODE_D])
    {
        // std::cout << "d";
        this->velocity.x += 1;
    }
    if (this->destination.x == 0 && !keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D])
    {
        this->velocity.x = 0;
    }

    if (keystates[SDL_SCANCODE_W])
    {
        // std::cout << "w";
        this->velocity.y -= 1;
    }
    if (keystates[SDL_SCANCODE_S])
    {
        // std::cout << "s";
        this->velocity.y += 1;
    }
    if (this->destination.y == 0 && !keystates[SDL_SCANCODE_W] && !keystates[SDL_SCANCODE_S])
    {
        this->velocity.y = 0;
    }
    // std::cout << std::endl;
}

void Player::update(float dTime)
{
    // Update bullets
    for (unsigned int i = 0; i < this->bullets.size(); i++)
    {
        Bullet* bullet = this->bullets[i];
        bullet->update(dTime);
        if (bullet->inFrame())
        {
            this->bullets.erase(this->bullets.begin() + i);
        }
    }

    // Set max speed
    if (this->velocity.x > 1)
    {
        this->velocity.x = 1;
    }
    if (this->velocity.x < -1)
    {
        this->velocity.x = -1;
    }

    if (this->velocity.y > 1)
    {
        this->velocity.y = 1;
    }
    if (this->velocity.y < -1)
    {
        this->velocity.y = -1;
    }

    // Update old position, position
    if (!almostEqual(this->pos.x, this->oldPos.x, FLT_EPSILON * 50000000))
    {
        this->oldPos.x = this->pos.x;
    }
    if (!almostEqual(this->pos.y, this->oldPos.y, FLT_EPSILON * 50000000))
    {
        this->oldPos.y = this->pos.y;
    }
    this->pos.x += this->velocity.x * dTime * MAX_SPEED;
    this->pos.y += this->velocity.y * dTime * MAX_SPEED;

    // Update center, angle
    this->center.x = this->pos.x + (this->texture->getWidth() / 2);
    this->center.y = this->pos.y + (this->texture->getHeight() / 2);
    this->updateAngle();

    // todo
    // Old code for destination navigation
    /*
    if (abs(this->pos.x - this->destination.x) <= abs(this->velocity.x * dTime * MAX_SPEED) + EPSILON)
    {
        this->velocity.x = 0;
        this->pos.x = this->destination.x;
        this->destination.x = 0;
    }
    else
    {
        this->pos.x += this->velocity.x * dTime * MAX_SPEED;
    }

    if (abs(this->pos.y - this->destination.y) <= abs(this->velocity.y * dTime * MAX_SPEED) + EPSILON)
    {
        this->velocity.y = 0;
        this->pos.y = this->destination.y;
        this->destination.y = 0;
    }
    else
    {
        this->pos.y += this->velocity.y * dTime * MAX_SPEED;
    }
     */



    // Boundary box
    if (this->pos.x <= 0)
    {
        this->velocity.x = 10;
//        this->pos.x = 1;
    }
    else if (this->pos.x > 1080 - this->texture->getWidth())
    {
        this->velocity.x = -10;
//        this->pos.x = 1080 - this->texture->getWidth();
    }

    if (this->pos.y <= 0)
    {
        this->velocity.y = 10;
//        this->pos.y = 1;
    }
    else if (this->pos.y > 720 - this->texture->getHeight())
    {
        this->velocity.y = -10;
//        this->pos.y = 720 - this->texture->getHeight();
    }
}


void Player::render()
{
    for (Bullet* bullet : this->bullets)
    {
        bullet->render();
    }
    this->texture->renderTexture((int) this->pos.x, (int) this->pos.y, NULL, this->angle, NULL, SDL_FLIP_NONE);
}

void Player::setPos(int x, int y)
{
    this->pos.x = x;
    this->pos.y = y;
    this->oldPos.x = this->pos.x;
    this->oldPos.y = this->pos.y;
    this->center.x = this->pos.x + (this->texture->getWidth() / 2);
    this->center.y = this->pos.y + (this->texture->getHeight() / 2);
}

void Player::updateAngle()
{
    this->angle = (atan2(this->center.y - this->aim.y, this->center.x - this->aim.x) * (180 / PI)) - 90;
    if (this->angle < 0)
    {
        this->angle = 360 - (-angle);
    }
}

void Player::fireBullet()
{
    Bullet* bullet = new Bullet(this->center);
    bullet->setDestination(this->aim);
    bullet->setRenderer(this->texture->getRenderer());
    bullet->setTexture("assets/bullet.png");

    this->bullets.push_back(bullet);
}