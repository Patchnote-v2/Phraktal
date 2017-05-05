#ifndef PHRAKTAL_ENGINE_H
#define PHRAKTAL_ENGINE_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <ctime>
#include "Camera.h"

#include "Text.h"
#include "Entity.h"
#include "Grid.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Coin.h"
#include "Log.h"

class Engine
{
public:
    Engine();
    ~Engine(){};

    Camera camera;

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void rendererClear();
    void rendererPresent();
    void resetLevel();
    void resetGrid();

    template<typename T>
    void initEntity(std::shared_ptr< T > entity, std::string textureName);
    void initText(std::shared_ptr< Text > text, int fontSize);
    void initHud(std::shared_ptr< Entity > hud, std::string textureName);

    void handleEvents(SDL_Event& e);

    void updateText(std::shared_ptr< Text > text, std::string textStr);
    void updateEntities(float dTime);

    void renderEntities();

    template<typename T>
    void renderEntity(std::shared_ptr< T > entity);
    template<typename T>
    void renderEntityStatic(std::shared_ptr< T > entity);

    void renderRectangleOutline(SDL_Rect rect);
    void renderRectangleFilled(SDL_Rect rect);
    void renderBar(std::shared_ptr< Bar > bar);

    std::string getStats() const;

private:
    bool shiftHeld;
    int enemiesKilled;
    int timesHit;

    std::unique_ptr< SDL_Window, phraktal::utils::SDL_Deleter > window;
    std::shared_ptr< SDL_Renderer > renderer;
    Grid grid;

    std::shared_ptr< Player > player;
    std::vector< std::shared_ptr< Entity > > entities;
    std::vector< std::shared_ptr< Bullet > > bullets;
    std::vector< std::shared_ptr< Text > > text;
    std::vector< std::shared_ptr< Entity > > hud;

    int init();
    void quitSDL();
};

template<typename T>
void Engine::initEntity(std::shared_ptr< T > entity, std::string textureName)
{
    entity->texture = std::unique_ptr< TextureW >(new TextureW());
    entity->texture->setRenderer(this->renderer);
    entity->loadTexture(textureName);

    auto type = entity->getType();

    if (type == Entity::Type::PLAYER)
    {
        this->player = std::dynamic_pointer_cast< Player >(entity);
        this->entities.push_back(entity);
    }
    else if (type == Entity::Type::POWERUP ||
            type == Entity::Type::ENEMY ||
            type == Entity::Type::COIN)
    {
        this->entities.push_back(entity);
    }

    else if (type == Entity::Type::PLAYER_BULLET ||
            type == Entity::Type::ENEMY_BULLET)
    {
        this->bullets.push_back(std::dynamic_pointer_cast< Bullet >(entity));
    }
    else if (type == Entity::Type::BACKGROUND_ENTITY)
    {
//        entity->aim.x = entity->center.x;
        return;
    }

    this->grid.addEntity(entity);
}

template<typename T>
void Engine::renderEntity(std::shared_ptr< T > entity)
{
    if (SDL_HasIntersection(entity->getRect().get(), entity->camera.getRect().get()))
    {
        entity->texture->renderTexture((int) entity->pos.x - (int) entity->camera.pos.x,
                                       (int) entity->pos.y - (int) entity->camera.pos.y,
                                       NULL,
                                       entity->angle,
                                       NULL,
                                       SDL_FLIP_NONE);
    }
}

template<typename T>
void Engine::renderEntityStatic(std::shared_ptr< T > entity)
{
//     todo: change to HUD items/text?  is this a good way of doing this?
    entity->texture->renderTexture((int) entity->pos.x,
                                   (int) entity->pos.y,
                                   NULL,
                                   entity->angle,
                                   NULL,
                                   SDL_FLIP_NONE);
}

#endif //PHRAKTAL_ENGINE_H
