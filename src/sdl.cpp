#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <limits>
#include <cfloat>
#include <memory>
#include <level.h>
#include <ctime>

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Data structures and utilities
#include "base/log.h"
#include "base/texturew.h"
#include "base/timer.h"
#include "base/vector2.h"
#include "base/grid.h"
#include "base/utils.h"

// Game objects
#include "mimics/mimic.h"
#include "camera.h"
#include "mimics/player.h"
#include "mimics/enemy.h"


int init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        Log::logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        Log::logSDLError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0)
    {
        Log::logSDLError(std::cout, "TTF_Init");
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    return 0;
}

void quitSDL()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

int main()
{
    if (init() == 1)
    {
        return 1;
    }

//    std::unique_ptr<int, phraktal::utils::SDL_Deleter> test(new int(1), phraktal::utils::SDL_Deleter());

    phraktal::utils::SDL_Deleter d;
    std::unique_ptr< SDL_Window, phraktal::utils::SDL_Deleter > win(SDL_CreateWindow("TopDown", 100, 100, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT, SDL_WINDOW_SHOWN), d);
    if (win == nullptr)
    {
        Log::logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    // Renderer
    std::shared_ptr< SDL_Renderer > renderer(SDL_CreateRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED), d);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        Log::logSDLError(std::cout, "CreateRenderer");
        SDL_Quit();
        return 1;
    }


    // Stats
    std::stringstream timeText;
    std::unique_ptr<TextureW> fpsCounter(new TextureW());
    fpsCounter->setRenderer(renderer);
    fpsCounter->setFont(phraktal::assets::DEFAULT_FONT, 16);

    std::stringstream statsText;
    std::unique_ptr<TextureW> stats(new TextureW());
    stats->setRenderer(renderer);
    stats->setFont(phraktal::assets::DEFAULT_FONT, 16);

    // Textures
    std::map< std::string, std::unique_ptr<TextureW> > images;
    SDL_Color color = { 255, 255, 255, 255};

    std::unique_ptr< TextureW > bg(new TextureW());
    bg->setRenderer(renderer);
    bg->loadTexture(phraktal::assets::BG_PNG);

    // Texture std::map
    images.insert(std::map< std::string, std::unique_ptr< TextureW > >::value_type("fpsCounter", std::move(fpsCounter)));
    images.insert(std::map< std::string, std::unique_ptr< TextureW > >::value_type("stats", std::move(stats)));
    images.insert(std::map< std::string, std::unique_ptr< TextureW > >::value_type("bg", std::move(bg)));

    std::vector< std::unique_ptr< TextureW > > widthT;
    std::vector< std::unique_ptr< TextureW > > heightT;

    // Grid index textures
    for (int w = 0; w < phraktal::levels::LEVEL_WIDTH / phraktal::levels::TILE_SIZE2; w++)
    {
        std::unique_ptr< TextureW > wTex(new TextureW());
        wTex->setRenderer(renderer);
        if (wTex->setFont(phraktal::assets::DEFAULT_FONT, 16))
        {
            wTex->loadTextureFromText(std::to_string(w), color);
            widthT.push_back(std::move(wTex));
        }
    }
    for (int h = 0; h < phraktal::levels::LEVEL_HEIGHT / phraktal::levels::TILE_SIZE2; h++)
    {
        std::unique_ptr< TextureW > hTex(new TextureW());
        hTex->setRenderer(renderer);
        if (hTex->setFont(phraktal::assets::DEFAULT_FONT, 16))
        {
            hTex->loadTextureFromText(std::to_string(h), color);
            heightT.push_back(std::move(hTex));
        }
    }
    // Collision detection
    std::unique_ptr< Grid > grid(new Grid(phraktal::levels::LEVEL_WIDTH, phraktal::levels::LEVEL_HEIGHT, phraktal::levels::TILE_SIZE2, renderer));


    // Timers
    std::unique_ptr<Timer> fpsTimer(new Timer());
    int countedFrames = 0;
    float averageFPS = 0.f;
    Uint32 secondCounter = 0;
    fpsTimer->start();

    std::unique_ptr<Timer> capTimer(new Timer());

    std::unique_ptr<Timer> deltaTimer(new Timer());
    deltaTimer->start();

    // Keystates
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Levels
    auto level1 = std::make_shared< Level >();
    level1->loadFromFile("assets/levels/1.txt", renderer);

    // Mimics
    std::vector< std::shared_ptr< Mimic > > mimics;
    auto camera = std::make_shared< Camera >(0, 0, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT);
    auto player = std::make_shared< Player >(camera);
    player->setRenderer(renderer);
    player->setTexture(phraktal::assets::PLAYER_PNG);
    player->setPos(550, 250);
    mimics.push_back(player);
    grid->addMimic(player);
    // Bullets
    std::vector< std::shared_ptr< Bullet > > bullets;

    // todo: temp
    bool leftMouseButtonState = false;
    int shotPower = 0;

    // Main loop
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        capTimer->start();
        /*
        if (fpsTimer->getTicks() - secondCounter > 3000)
        {
            secondCounter = fpsTimer->getTicks();
            std::srand((uint) std::time(0));
            int x = (std::rand() % phraktal::levels::LEVEL_WIDTH);
            int y = (std::rand() % phraktal::levels::LEVEL_HEIGHT);
            auto testEnemy = std::make_shared< Enemy >(camera);
            testEnemy->setRenderer(renderer);
            testEnemy->setTexture(phraktal::assets::ENEMY_PNG);
            testEnemy->setPos(x, y);
            testEnemy->setCurrentTarget(player);
            testEnemy->toggleActive();
            grid->addMimic(testEnemy);
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
                    x = x + (int) camera->pos.x;
                    y = y + (int) camera->pos.y;
                    auto bullet = std::make_shared< Bullet >(*(player->getCenter().get()), camera);
                    bullet->setDestination(x, y);
                    bullet->setRenderer(renderer);
                    bullet->setTexture(phraktal::assets::PLAYER_BULLET_PNG);
                    bullet->setType(Type::PLAYER_BULLET);
                    bullet->setShotPower(shotPower);

                    grid->addMimic(bullet);
                    bullets.push_back(bullet);

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
                    auto enemy = std::make_shared< Enemy >(camera);
                    enemy->setRenderer(renderer);
                    enemy->setTexture(phraktal::assets::ENEMY_PNG);
                    enemy->setPos(x + (int) camera->pos.x, y + (int) camera->pos.y);
                    enemy->setCurrentTarget(player);
                    enemy->toggleActive();
                    grid->addMimic(enemy);
                    mimics.push_back(enemy);
                }
                if (e.key.keysym.sym == SDLK_x)
                {
                    mimics.clear();
                    mimics.shrink_to_fit();
                    mimics.push_back(player);
                    grid->clear();
                    grid->addMimic(player);
                }
                if (e.key.keysym.sym == SDLK_m)
                {
                    grid->removeMimic(player);
                }
                if (e.key.keysym.sym == SDLK_o)
                {
                    // Reserved for debugging
                    std::srand((uint) std::time(0));
                    int x = (std::rand() % phraktal::levels::SCREEN_WIDTH);
                    int y = (std::rand() % phraktal::levels::SCREEN_HEIGHT);
                    auto testEnemy = std::make_shared< Enemy >(camera);
                    testEnemy->setRenderer(renderer);
                    testEnemy->setTexture(phraktal::assets::ENEMY_PNG);
                    testEnemy->setPos(x, y);
                    testEnemy->setCurrentTarget(player);
                    testEnemy->toggleActive();
                    mimics.push_back(testEnemy);
                    grid->addMimic(testEnemy);

                }
            }
            if (e.type == SDL_KEYUP)
            {
                //
            }
        }

        // FPS
        averageFPS = countedFrames / (fpsTimer->getTicks() / 1000.f);
        if (averageFPS > 20000)
        {
            averageFPS = 0;
        }
        timeText.str("");
        timeText << "FPS: " << averageFPS;
        images["fpsCounter"]->loadTextureFromText(timeText.str(), color, 0);

        // Stats
        statsText.str("");
        statsText << "X: " << player->getPos()->x << std::endl;
        statsText <<  "Y: " << player->getPos()->y << std::endl;
        statsText << "oX: " << player->getOldPos()->x << std::endl;
        statsText <<  "oY: " << player->getOldPos()->y << std::endl;
        statsText << "Shot Power: " << shotPower << std::endl;
        statsText << "Num bullets: " << bullets.size() << std::endl;
        images["stats"]->loadTextureFromText(statsText.str(), color, 250);

        // Delta time
        float dTime = deltaTimer->getTicks() / 1000.f;

        // Charge shot if LMB is held
        if (leftMouseButtonState && shotPower < phraktal::levels::MAX_SHOT_POWER)
        {
            shotPower += 10;
        }

        if (player->hasMoved())
        {
            std::cout << "Moved" << std::endl;
            grid->updateMimic(player);
        }

        // Update all mimics
        for (auto mimic : mimics)
        {
            mimic->update(dTime);

            // Autofire enemy bullets
            if (mimic->getType() == Type::ENEMY)
            {
                auto derived = std::dynamic_pointer_cast<Enemy>(mimic);
                if ((int) derived->shotCooldown > 3)
                {
                    derived->shotCooldown = 0;
                    auto bullet = std::make_shared< Bullet >(*(derived->getCenter().get()), camera);
                    bullet->setDestination((int) player->getCenter()->x, (int) player->getCenter()->y);
                    bullet->setRenderer(renderer);
                    bullet->setTexture(phraktal::assets::ENEMY_BULLET_PNG);
                    bullet->setType(Type::ENEMY_BULLET);

                    bullets.push_back(bullet);
                    grid->addMimic(bullet);
                }
            }
        }

        // Bullet collision check
        for (unsigned int i = 0; i < bullets.size(); i++)
        {
            bullets[i]->update(dTime);

            if (bullets[i]->hasMoved())
            {
                grid->updateMimic(bullets[i]);
            }

            // Player bullet collision check
            if (bullets[i]->getType() == Type::PLAYER_BULLET)
            {
                for (unsigned int n = 0; n < mimics.size(); n++)
                {
                    if (mimics[n]->getType() == Type::ENEMY)
                    {
                        if (SDL_HasIntersection(bullets[i]->getRect().get(), mimics[n]->getRect().get()))
                        {
                            grid->removeMimic(mimics[n]);
                            mimics.erase(mimics.begin() + n);
                            grid->removeMimic(bullets[i]);
                            bullets.erase(bullets.begin() + i);
                            goto next;
                        }
                    }
                }
            }
            // Enemy bullet collision check
            else if (bullets[i]->getType() == Type::ENEMY_BULLET)
            {
                if (SDL_HasIntersection(bullets[i]->getRect().get(), player->getRect().get()))
                {
                    std::cout << "HIT" << std::endl;
                    goto next;
                }
            }

            // Delete bullet if out of level
            if (bullets[i]->isInLevel())
            {
                grid->removeMimic(bullets[i]);
                bullets.erase(bullets.begin() + i);
            }
            next:;
        }

        // Camera
        camera->update();

        //todo: update grid
        deltaTimer->start();

        // Render textures
        SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer.get());
        images["bg"]->renderTexture(0, 0, camera->getRect());

        // Grid
        grid->render(renderer, camera);

        // Grid indexes
        auto rect = camera->getRect();
        for (int w = 0; w < phraktal::levels::LEVEL_WIDTH / phraktal::levels::TILE_SIZE2; w++)
        {
            widthT[w]->renderTexture(w * phraktal::levels::TILE_SIZE2 + 5 - rect->x, 0 + 5);
        }
        for (int h = 0; h < phraktal::levels::LEVEL_HEIGHT / phraktal::levels::TILE_SIZE2; h++)
        {
            heightT[h]->renderTexture(0 + 5, h * phraktal::levels::TILE_SIZE2 + 5 - rect->y);
        }

        // Stats
        images["fpsCounter"]->renderTexture(10, 10);
        images["stats"]->renderTexture(10, images["fpsCounter"]->getHeight() + 20);

        // Render all mimics
        for (auto mimic : mimics)
        {
            mimic->render();
        }

        // Render all bullets
        for (unsigned int i = 0; i < bullets.size(); i++)
        {
            bullets[i]->render();
        }
        //todo: render grid
        SDL_RenderPresent(renderer.get());

        // FPS
        countedFrames++;
        Uint32 frameTicks = capTimer->getTicks();
        if (frameTicks < phraktal::levels::SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(phraktal::levels::SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

//    for (auto image : images)
//    {
//        image.second->clearTexture();
//    }
    quitSDL();
}