
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <limits>
#include <cfloat>
#include <memory>
#include <ctime>

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Utils.h"
#include "Engine.h"
#include "BackgroundEntity.h"
#include "Entity.h"
#include "Enemy.h"
#include "Timer.h"

int main()
{
    Engine engine;

    // Timers
    Timer fpsTimer;
    int countedFrames = 0;
    float averageFPS = 0.0f;
    fpsTimer.start();

    Timer capTimer;

    Timer deltaTimer;
    deltaTimer.start();

    // Stats
    std::stringstream fpsText;
    auto fps = std::make_shared< Text >(engine.camera, 10, 10);
    engine.initText(fps, phraktal::levels::DEFAULT_FONT_SIZE);

    std::stringstream statsText;
    auto stats = std::make_shared< Text >(engine.camera, 10, 50);
    engine.initText(stats, phraktal::levels::DEFAULT_FONT_SIZE);

    // Textures
    auto background = std::make_shared< BackgroundEntity >(engine.camera, 0, 0);
    engine.initEntity(background, phraktal::assets::BG_PNG);

    // todo: HUD items

    // Keystates
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Entities
    auto player = std::make_shared< Player >(engine.camera, 0, 0);
    engine.initEntity(player, phraktal::assets::PLAYER_PNG);

    // Bullets
    std::vector< Bullet > bullets;

    // todo: temp
    bool leftMouseButtonState = false;
    int shotPower = 0;

    // Main loop
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        capTimer.start();
        /*
        if (fpsTimer->getTicks() - secondCounter > 3000)
        {
            secondCounter = fpsTimer->getTicks();
            std::srand((uint) std::time(0));
            int x = (std::rand() % phraktal::levels::LEVEL_WIDTH);
            int y = (std::rand() % phraktal::levels::LEVEL_HEIGHT);
            auto testEnemy = std::make_shared< Enemy >(camera);
            testEnemy->setRenderer(renderer);
            testEnemy->loadTexture(phraktal::assets::ENEMY_PNG);
            testEnemy->setPos(x, y);
            testEnemy->setCurrentTarget(player);
            testEnemy->toggleActive();
            grid->addEntity(testEnemy);
            mimics.push_back(testEnemy);
        }
         */
        player->handleKeystates(keystates);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                player->handleEvents(e);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonState = true;
                }
                else
                {
                    player->handleEvents(e);
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    x = x + (int) engine.camera.pos.x;
                    y = y + (int) engine.camera.pos.y;
                    auto bullet = std::make_shared< Bullet >(engine.camera, (int) player->getCenter().x, (int) player->getCenter().y, shotPower,
                                  Entity::Type::PLAYER_BULLET);
                    engine.initEntity(bullet, phraktal::assets::PLAYER_BULLET_PNG);
                    bullet->setDestination(x, y);

                    shotPower = 0;
                    leftMouseButtonState = false;
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_q)
                {

                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    auto enemy = std::make_shared< Enemy >(engine.camera, x + (int) engine.camera.pos.x, y + (int) engine.camera.pos.y);
                    engine.initEntity(enemy, phraktal::assets::ENEMY_PNG);
                    enemy->setCurrentTarget(player);
                    enemy->toggleActive();
                }
                if (e.key.keysym.sym == SDLK_x)
                {
                    engine.resetLevel();
                }
                if (e.key.keysym.sym == SDLK_o)
                {
                    // Reserved for debugging
                    std::srand((uint) std::time(0));
                    int x = (std::rand() % phraktal::levels::SCREEN_WIDTH);
                    int y = (std::rand() % phraktal::levels::SCREEN_HEIGHT);
                    auto testEnemy = std::make_shared< Enemy >(engine.camera, x, y);
                    engine.initEntity(testEnemy, phraktal::assets::ENEMY_PNG);
                    testEnemy->setCurrentTarget(player);
                    testEnemy->toggleActive();
//                    mimics.push_back(testEnemy);
                }
            }
            if (e.type == SDL_KEYUP)
            {
                //
            }
        }

        // FPS
        averageFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (averageFPS > 20000)
        {
            averageFPS = 0;
        }
        fpsText.str("");
        fpsText << "FPS: " << averageFPS;
        engine.updateText(fps, fpsText.str());

        // Stats
        statsText.str("");
        statsText << "X: " << player->getPos().x << std::endl;
        statsText << "Y: " << player->getPos().y << std::endl;
        statsText << "Shot Power: " << shotPower << std::endl;
        statsText << engine.getVectorStats();
        engine.updateText(stats, statsText.str());

        // Delta time
        float dTime = deltaTimer.getTicks() / 1000.f;

        // Charge shot if LMB is held
        if (leftMouseButtonState && shotPower < phraktal::levels::MAX_SHOT_POWER)
        {
            shotPower += 100;
        }

        /*
        if (player.hasMoved())
        {
            std::cout << "Moved" << std::endl;
            grid->updateMoveableEntity(player);
        }
         */

        // Update all entities
        engine.updateEntities(dTime);
        engine.resetGrid();

        // Bullet collision check
        /*
        for (unsigned int i = 0; i < bullets.size(); i++)
        {
            bullets[i]->update(dTime);

            if (bullets[i]->hasMoved())
            {
                grid->updateMoveableEntity(bullets[i]);
            }

            // Player bullet collision check
            if (bullets[i]->getType() == Type::PLAYER_BULLET)
            {
                for (unsigned int n = 0; n < mimics.size(); n++)
                {
                    if (mimics[n]->getType() == Type::ENEMY)
                    {
                        if (SDL_HasIntersection(&(bullets[i]->getRect()), &(mimics[n]->getRect())))
                        {
                            grid->removeEntity(mimics[n]);
                            mimics.erase(mimics.begin() + n);
                            grid->removeEntity(bullets[i]);
                            bullets.erase(bullets.begin() + i);
                            goto next;
                        }
                    }
                }
            }
            // Enemy bullet collision check
            else if (bullets[i]->getType() == Type::ENEMY_BULLET)
            {
                if (SDL_HasIntersection(&(bullets[i]->getRect()), &(player->getRect())))
                {
                    std::cout << "HIT" << std::endl;
                    goto next;
                }
            }

            // Delete bullet if out of level
            if (bullets[i]->isInLevel())
            {
                grid->removeEntity(bullets[i]);
                bullets.erase(bullets.begin() + i);
            }
            next:;
        }
         */

        // Camera
        engine.camera.update();

        //todo: update grid
        deltaTimer.start();

        // Render textures
        engine.setDrawColor(0x00, 0x00, 0x00, 0xFF);
        engine.rendererClear();

        engine.renderEntity(background);

        engine.renderEntities();

        // Stats
        engine.renderEntityStatic(fps);
        engine.renderEntityStatic(stats);


//        engine.renderEntity(player);


        //todo: render grid
        engine.rendererPresent();

        // FPS
        countedFrames++;
        Uint32 frameTicks = capTimer.getTicks();
        if (frameTicks < phraktal::levels::SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(phraktal::levels::SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }
}