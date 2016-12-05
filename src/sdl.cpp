#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <limits>
#include <cfloat>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Data structures and utilities
#include "log.h"
#include "texturew.h"
#include "timer.h"
#include "vector2.h"
#include "quadtree.h"
#include "utils.h"

// Game objects
#include "mimic.h"
#include "player.h"
#include "enemy.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int SQUARE_SIZE = 100;

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
    std::unique_ptr< SDL_Window, phraktal::utils::SDL_Deleter > win(SDL_CreateWindow("TopDown", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), d);
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
    fpsCounter->setFont("assets/sample.ttf", 16);

    std::stringstream statsText;
    std::unique_ptr<TextureW> stats(new TextureW());
    stats->setRenderer(renderer);
    stats->setFont("assets/sample.ttf", 16);

    // Textures
    std::map< std::string, std::unique_ptr<TextureW> > images;
    SDL_Color color = { 255, 255, 255, 255};

    // Texture map
    images.insert(std::map< std::string, std::unique_ptr<TextureW> >::value_type("fpsCounter", std::move(fpsCounter)));
    images.insert(std::map< std::string, std::unique_ptr<TextureW> >::value_type("stats", std::move(stats)));

    // Collision detection
    std::unique_ptr< SDL_Rect > windowBoundary(new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
    std::shared_ptr< Quadtree<Mimic> > quadtree(new Quadtree<Mimic>(std::move(windowBoundary), 0));
    quadtree->start();

    // Timers
    std::unique_ptr<Timer> fpsTimer(new Timer());
    int countedFrames = 0;
    float averageFPS = 0.f;
    fpsTimer->start();

    std::unique_ptr<Timer> capTimer(new Timer());

    std::unique_ptr<Timer> deltaTimer(new Timer());
    deltaTimer->start();

    // Keystates
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Mimics

    std::vector< std::shared_ptr< Mimic > > mimics;
    std::shared_ptr<Player> player(new Player());
    player->setRenderer(renderer);
    player->setTexture(PLAYER_PNG);
    player->setPos(538, 250);
    mimics.push_back(player);
    quadtree->insert(player);

    /*
    Enemy* enemy2 = new Enemy();
    enemy2->setRenderer(renderer);
    enemy2->setTexture(ENEMY_PNG);
    enemy2->setPos(555, 555);
    quadtree.insert(enemy2);

    Enemy* enemy3 = new Enemy();
    enemy3->setRenderer(renderer);
    enemy3->setTexture(ENEMY_PNG);
    enemy3->setPos(455, 455);
    quadtree.insert(enemy3);

    Enemy* enemy4 = new Enemy();
    enemy4->setRenderer(renderer);
    enemy4->setTexture(ENEMY_PNG);
    enemy4->setPos(355, 355);
    quadtree.insert(enemy4);
     */

    // Main loop
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        capTimer->start();
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
                player->handleEvents(e);
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_q)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    std::shared_ptr< Enemy > enemy(new Enemy());
                    enemy->setRenderer(renderer);
                    enemy->setTexture(ENEMY_PNG);
                    enemy->setPos(x, y);
                    mimics.push_back(enemy);
                    quadtree->insert(enemy);
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
        images["fpsCounter"]->loadTextureFromText(timeText.str().c_str(), color, 100);

        // Stats
        statsText.str("");
        statsText << "X: " << player->getPos()->x << std::endl;
        statsText <<  "Y: " << player->getPos()->y << std::endl;
        statsText << "oX: " << player->getOldPos()->x << std::endl;
        statsText <<  "oY: " << player->getOldPos()->y << std::endl;
        images["stats"]->loadTextureFromText(statsText.str().c_str(), color, 250);

        // Delta time and updates
        float dTime = deltaTimer->getTicks() / 1000.f;
        for (auto mimic : mimics)
        {
            mimic->update(dTime);
        }
        quadtree->update();
        deltaTimer->start();


        // Render textures
        SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer.get());
        images["fpsCounter"]->renderTexture(10, 10);
        images["stats"]->renderTexture(10, images["fpsCounter"]->getHeight() + 20);
        for (auto mimic : mimics)
        {
            mimic->render();
        }
        SDL_SetRenderDrawColor(renderer.get(), 0xAA, 0xAA, 0xAA, 0xAA);
        quadtree->render(renderer);
        SDL_RenderPresent(renderer.get());

        // FPS
        countedFrames++;
        int frameTicks = capTimer->getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

//    for (auto image : images)
//    {
//        image.second->clearTexture();
//    }
    quitSDL();
}