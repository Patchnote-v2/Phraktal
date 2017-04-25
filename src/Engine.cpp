#include <sstream>
#include <ctime>
#include <Coin.h>
#include "Engine.h"

/*
 * Public functions
 */

Engine::Engine() :
        camera(0, 0, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT),
        enemiesKilled(0),
        timesHit(0),
        grid(phraktal::levels::LEVEL_WIDTH, phraktal::levels::LEVEL_HEIGHT, phraktal::levels::TILE_SIZE)
{
    // Initialize SDL, IMG, and TTF libraries
    if (this->init() == 1)
    {
        return;
    }

    // Initialize window and set to class member
    phraktal::utils::SDL_Deleter d;
    std::unique_ptr <SDL_Window, phraktal::utils::SDL_Deleter> window(
            SDL_CreateWindow(
                    "Phraktal", 100, 100, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
            d);

    if (window == nullptr)
    {
        Log::log(std::cout, "CreateWindow");
        SDL_Quit();
        return;
    }
    else
    {
        this->window = std::move(window);
    }

    // Initialize renderer and set to class member
    std::shared_ptr< SDL_Renderer > renderer(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED), d);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        Log::log(std::cout, "CreateRenderer");
        SDL_Quit();
        return;
    }
    else
    {
        this->renderer = renderer;
    }

    this->grid.initTextures(this->renderer);
    this->grid.clear();
}

void Engine::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(this->renderer.get(), r, g, b, a);
}

void Engine::rendererClear()
{
    SDL_RenderClear(this->renderer.get());
}

void Engine::rendererPresent()
{
    SDL_RenderPresent(this->renderer.get());
}

void Engine::resetLevel()
{
    this->grid.clear();
    this->entities.clear();
    this->entities.shrink_to_fit();
    this->bullets.clear();
    this->bullets.shrink_to_fit();
    this->entities.push_back(this->player);
    this->grid.addEntity(this->player);
    this->player->setPos(0, 0);
}

void Engine::resetGrid()
{
    this->grid.clear();
    for (auto entity : this->entities)
    {
        this->grid.addEntity(entity);
    }
    for (auto bullet : this->bullets)
    {
        this->grid.addEntity(bullet);
    }
}

void Engine::initText(std::shared_ptr< Text > text, int fontSize)
{
    // todo: ability to select different fonts, and colors
    text->texture = std::unique_ptr< TextureW >(new TextureW());
    text->texture->setRenderer(this->renderer);
    text->texture->setFont(phraktal::assets::DEFAULT_FONT, fontSize);
    SDL_Color color = {255, 255, 255, 255};
    text->texture->loadTextureFromText(text->text, color);
    this->text.push_back(text);
}

void Engine::initHud(std::shared_ptr< Entity > hud, std::string textureName)
{
    hud->texture = std::unique_ptr< TextureW >(new TextureW());
    hud->texture->setRenderer(this->renderer);
    hud->loadTexture(textureName);
    this->hud.push_back(hud);
}

void Engine::updateText(std::shared_ptr< Text > text, std::string textStr)
{
    SDL_Color color = {255, 255, 255, 255};
    text->updateText(textStr);
    text->texture->loadTextureFromText(textStr, color);
}

void Engine::handleEvents(SDL_Event& e)
{
    if (e.type == SDL_MOUSEMOTION)
    {
        player->handleEvents(e);
    }
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        player->handleEvents(e);
    }
    if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (this->player->canFire())
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x = x + (int) this->camera.pos.x;
                y = y + (int) this->camera.pos.y;
                switch (this->player->getPowerupType())
                {
                    case Powerup::PowerupType::NONE:
                    {
                        auto singleBullet = std::make_shared< Bullet >(this->camera, (int) player->getCenter().x,
                                                                       (int) player->getCenter().y,
                                                                       phraktal::levels::MAX_REGULAR_SHOT_SPEED,
                                                                       Entity::Type::PLAYER_BULLET,
                                                                      Powerup::PowerupType::NONE);
                        this->initEntity(singleBullet, phraktal::assets::PLAYER_BULLET_PNG);
                        singleBullet->setDestination(x, y);
                        break;
                    }

                    case Powerup::PowerupType::SPREAD:
                    {
                        auto bullet1 = std::make_shared< Bullet >(this->camera, (int) player->getCenter().x,
                                                                  (int) player->getCenter().y,
                                                                  phraktal::levels::MAX_SPREAD_SHOT_SPEED,
                                                                  Entity::Type::PLAYER_BULLET,
                                                                  Powerup::PowerupType::SPREAD);
                        auto bullet2 = std::make_shared< Bullet >(this->camera, (int) player->getCenter().x,
                                                                  (int) player->getCenter().y,
                                                                  phraktal::levels::MAX_SPREAD_SHOT_SPEED,
                                                                  Entity::Type::PLAYER_BULLET,
                                                                  Powerup::PowerupType::SPREAD);
                        auto bullet3 = std::make_shared< Bullet >(this->camera, (int) player->getCenter().x,
                                                                  (int) player->getCenter().y,
                                                                  phraktal::levels::MAX_SPREAD_SHOT_SPEED,
                                                                  Entity::Type::PLAYER_BULLET,
                                                                  Powerup::PowerupType::SPREAD);
                        this->initEntity(bullet1, phraktal::assets::SPREAD_PLAYER_BULLET_PNG);
                        this->initEntity(bullet2, phraktal::assets::SPREAD_PLAYER_BULLET_PNG);
                        this->initEntity(bullet3, phraktal::assets::SPREAD_PLAYER_BULLET_PNG);
                        bullet1->setVelocityFromAngle(this->player->getAngle());
                        bullet2->setVelocityFromAngle(this->player->getAngle() - 5);
                        bullet3->setVelocityFromAngle(this->player->getAngle() + 5);
                        break;
                    }

                    case Powerup::PowerupType::LARGE:
                    {
                        auto largeBullet = std::make_shared< Bullet >(this->camera, (int) player->getCenter().x,
                                                                      (int) player->getCenter().y,
                                                                      phraktal::levels::MAX_LARGE_SHOT_SPEED,
                                                                      Entity::Type::PLAYER_BULLET,
                                                                      Powerup::PowerupType::LARGE);
                        this->initEntity(largeBullet, phraktal::assets::LARGE_PLAYER_BULLET_PNG);
                        largeBullet->setDestination(x, y);

                        break;
                    }
                }

                this->player->resetShotCooldown();
            }
        }
    }
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_q)
        {

            int x, y;
            SDL_GetMouseState(&x, &y);
            auto enemy = std::make_shared< Enemy >(this->camera, x + (int) this->camera.pos.x, y + (int) this->camera.pos.y);
            this->initEntity(enemy, phraktal::assets::ENEMY_PNG);
            enemy->setMaxSpeed(phraktal::levels::MAX_ENEMY_SPEED);
            enemy->setCurrentTarget(player);
            enemy->toggleActive();
        }
        if (e.key.keysym.sym == SDLK_c)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto coin = std::make_shared< Coin >(this->camera, x + (int) this->camera.pos.x, y + (int) this->camera.pos.y);
            this->initEntity(coin, phraktal::assets::COIN_PNG);
        }
        if (e.key.keysym.sym == SDLK_x)
        {
            this->resetLevel();
        }
        if (e.key.keysym.sym == SDLK_o)
        {
            // Reserved for debugging
            std::srand((uint) std::time(0));
            int x = (std::rand() % phraktal::levels::SCREEN_WIDTH);
            int y = (std::rand() % phraktal::levels::SCREEN_HEIGHT);
            auto enemy = std::make_shared< Enemy >(this->camera, x, y);
            this->initEntity(enemy, phraktal::assets::ENEMY_PNG);
            enemy->setMaxSpeed(phraktal::levels::MAX_ENEMY_SPEED);
            enemy->setCurrentTarget(player);
            enemy->toggleActive();
        }
        if (e.key.keysym.sym == SDLK_1)
        {
            // Spread powerup
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto powerup = std::make_shared< Powerup >(this->camera, x + (int) this->camera.pos.x,
                                                       y + (int) this->camera.pos.y, Powerup::PowerupType::SPREAD);
            this->initEntity(powerup, phraktal::assets::SPREAD_POWERUP_PNG);
        }
        if (e.key.keysym.sym == SDLK_2)
        {
            // Large bullet powerup
            int x, y;
            SDL_GetMouseState(&x, &y);
            auto powerup = std::make_shared< Powerup >(this->camera, x + (int) this->camera.pos.x,
                                                      y + (int) this->camera.pos.y, Powerup::PowerupType::LARGE);
            this->initEntity(powerup, phraktal::assets::LARGE_BULLET_POWERUP_PNG);
        }
        if (e.key.keysym.sym == SDLK_3)
        {
            this->player->removePowerup();
        }
    }
}

void Engine::updateEntities(float dTime)
{
    for (auto entity : this->entities)
    {
        entity->update(dTime);

        // Autofire enemy bullets
        if (entity->getType() == Entity::Type::ENEMY)
        {
            auto derived = std::dynamic_pointer_cast< Enemy >(entity);
            if ((int) derived->shotCooldown > 3)
            {
                auto bullet = std::make_shared< Bullet >(camera, (int) derived->getCenter().x,
                                                         (int) derived->getCenter().y, 200, Entity::Type::ENEMY_BULLET, Powerup::PowerupType::NONE);
                this->initEntity(bullet, phraktal::assets::ENEMY_BULLET_PNG);
                bullet->setDestination((int) this->player->getCenter().x, (int) this->player->getCenter().y);

                derived->shotCooldown = 0;

                this->bullets.push_back(bullet);
                this->grid.addEntity(bullet);
            }
        }
    }

    restart:;
    for (auto entity : this->entities)
    {
        auto found = this->grid.query(entity);
        if (found.size() > 1)
        {
            for (auto k = found.begin(); k != found.end(); k++)
            {
                for (auto l = k; l != found.end(); l++)
                {
                    if (k == l)
                    {
                        continue;
                    }
                    else
                    {
                        // Player's bullets
                        if ((l->get()->getType() == Entity::Type::PLAYER_BULLET) &&
                             (k->get()->getType() == Entity::Type::ENEMY))
                        {
                            // k = enemy
                            if (k->get()->isColliding(*l))
                            {
                                auto enemy = std::dynamic_pointer_cast< Enemy >(*k);
                                auto bullet = std::dynamic_pointer_cast< Bullet >(*l);
                                enemy->setHealth(enemy->getHealth() - bullet->getDamage());
                                if (enemy->getHealth() <= 0)
                                {
                                    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *k),
                                                         this->entities.end());
                                    this->grid.removeEntity(*k);
                                    auto coin = std::make_shared< Coin >(this->camera, k->get()->pos.x, k->get()->pos.y);
                                    this->initEntity(coin, phraktal::assets::COIN_PNG);
                                    enemiesKilled++;
                                }
                                this->bullets.erase(std::remove(this->bullets.begin(), this->bullets.end(), *l),
                                                    this->bullets.end());
                                this->grid.removeEntity(*l);
                                goto restart;
                            }
                        }
                        else if ((l->get()->getType() == Entity::Type::ENEMY) &&
                                  (k->get()->getType() == Entity::Type::PLAYER_BULLET))
                        {
                            // l = enemy
                            if (k->get()->isColliding(*l))
                            {
                                auto enemy = std::dynamic_pointer_cast< Enemy >(*l);
                                auto bullet = std::dynamic_pointer_cast< Bullet >(*k);
                                enemy->setHealth(enemy->getHealth() - bullet->getDamage());
                                if (enemy->getHealth() <= 0)
                                {
                                    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *l),
                                                         this->entities.end());
                                    this->grid.removeEntity(*l);
                                    auto coin = std::make_shared< Coin >(this->camera, l->get()->pos.x, l->get()->pos.y);
                                    this->initEntity(coin, phraktal::assets::COIN_PNG);
                                    enemiesKilled++;
                                }
                                this->bullets.erase(std::remove(this->bullets.begin(), this->bullets.end(), *k),
                                                    this->bullets.end());
                                this->grid.removeEntity(*k);
                                goto restart;
                            }
                        }

                            // Enemy's bullets
                        else if (((l->get()->getType() == Entity::Type::ENEMY_BULLET) &&
                                  (k->get()->getType() == Entity::Type::PLAYER)))
                        {
                            if (k->get()->isColliding(*l))
                            {
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *l),
                                                     this->entities.end());
                                this->bullets.erase(std::remove(this->bullets.begin(), this->bullets.end(), *l),
                                                    this->bullets.end());
                                this->grid.removeEntity(*l);
                                timesHit++;
                                goto restart;
                            }
                        }
                        else if (((l->get()->getType() == Entity::Type::PLAYER) &&
                                  (k->get()->getType() == Entity::Type::ENEMY_BULLET)))
                        {
                            if (k->get()->isColliding(*l))
                            {
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *k),
                                                     this->entities.end());
                                this->bullets.erase(std::remove(this->bullets.begin(), this->bullets.end(), *k),
                                                    this->bullets.end());
                                this->grid.removeEntity(*k);
                                timesHit++;
                                goto restart;
                            }
                        }

                            // Coin
                        else if (l->get()->getType() == Entity::Type::COIN)
                        {
                            if (l->get()->isColliding(this->player))
                            {
                                this->player->addCoins(1);
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *l), this->entities.end());
                                this->grid.removeEntity(*l);
                                goto restart;
                            }
                        }
                        else if (k->get()->getType() == Entity::Type::COIN)
                        {
                            if (k->get()->isColliding(this->player))
                            {
                                this->player->addCoins(1);
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *k), this->entities.end());
                                this->grid.removeEntity(*k);
                                goto restart;
                            }
                        }

                            // Powerup
                        else if (l->get()->getType() == Entity::Type::POWERUP)
                        {
                            if (l->get()->isColliding(this->player))
                            {
                                this->player->setPowerupType(
                                        std::dynamic_pointer_cast< Powerup >(*l)
                                                ->getPowerupType()
                                                            );
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *l), this->entities.end());
                                this->grid.removeEntity(*l);
                                goto restart;
                            }
                        }
                        else if (k->get()->getType() == Entity::Type::POWERUP)
                        {
                            if (k->get()->isColliding(this->player))
                            {
                                this->player->setPowerupType(
                                        std::dynamic_pointer_cast< Powerup >(*k)
                                                ->getPowerupType()
                                                            );
                                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), *k), this->entities.end());
                                this->grid.removeEntity(*k);
                                goto restart;
                            }
                        }
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < this->bullets.size(); i++)
    {
        this->bullets[i]->update(dTime);
        if (!this->bullets[i]->isInLevel())
        {
            this->bullets.erase(this->bullets.begin() + i);
        }
    }
}

void Engine::renderEntities()
{
    for (auto entity : this->entities)
    {
        this->renderEntity(entity);
        if (entity->getType() == Entity::Type::ENEMY)
        {
            auto enemy = std::dynamic_pointer_cast< Enemy >(entity);
            auto color = enemy->getHealthColor();
            this->setDrawColor(color->r, color->g, color->b, color->a);
            this->renderRectangleOutline(*(enemy->healthBarOutline));
            this->renderRectangleFilled(*(enemy->healthBarFilled));
        }
    }
    for (auto bullet : this->bullets)
    {
        this->renderEntity(bullet);
    }
    this->renderEntity(this->player);
    for (auto hud : this->hud)
    {
        this->renderEntityStatic(hud);
    }
    for (auto text : this->text)
    {
        this->renderEntityStatic(text);
    }
}

void Engine::renderRectangleOutline(SDL_Rect rect)
{
    SDL_RenderDrawRect(this->renderer.get(), &rect);
}

void Engine::renderRectangleFilled(SDL_Rect rect)
{
    SDL_RenderFillRect(this->renderer.get(), &rect);
}

std::string Engine::getStats() const
{
    std::stringstream string;
    string << "Entities: " << this->entities.size() << std::endl;
    string << "Bullets: " << this->bullets.size() << std::endl;
    string << "Killed: " << this->enemiesKilled << std::endl;
    string << "Hit: " << this->timesHit << std::endl;
    return string.str();
}

/*
 * Private functions
 */

int Engine::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        Log::log(std::cout, "SDL_Init");
        return 1;
    }

    // Initialize IMG
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        Log::log(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    // Initialize TTF
    if (TTF_Init() != 0)
    {
        Log::log(std::cout, "TTF_Init");
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    return 0;
}

void Engine::quitSDL()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
