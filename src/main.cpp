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
#include "Powerup.h"
#include "Entity.h"
#include "Enemy.h"
#include "Timer.h"

int main()
{
    Engine engine;

    // Timers
    Timer fpsTimer;
//    int countedFrames = 0;
//    float averageFPS = 0.0f;
    fpsTimer.start();

    Timer capTimer;

    Timer deltaTimer;
    deltaTimer.start();

    /*
    std::stringstream fpsText;
    auto fps = std::make_shared< Text >(engine.camera, 10, 10);
    engine.initText(fps, phraktal::levels::DEFAULT_FONT_SIZE);
     */

    // Stats
    std::stringstream statsText;
    auto stats = std::make_shared< Text >(engine.camera, 10, 110);
    engine.initText(stats, phraktal::levels::DEFAULT_FONT_SIZE);

    // Background
    auto background = std::make_shared< BackgroundEntity >(engine.camera, 0, 0);
    engine.initEntity(background, phraktal::assets::BG_PNG);

    // Coin count
    auto coinCountGraphic = std::make_shared< Entity >(engine.camera, 10, 10);
    engine.initHud(coinCountGraphic, phraktal::assets::COIN_COUNT_GRAPHIC_PNG);

    auto coinCount = std::make_shared< Text >(engine.camera, 110, 20);
    engine.initText(coinCount, 100);

    // todo: HUD items?

    // Shot power bar rectangles
    SDL_Rect* filled = new SDL_Rect{phraktal::levels::SCREEN_WIDTH - 311, 11, 0, 35};
    SDL_Rect* barOutline = new SDL_Rect{phraktal::levels::SCREEN_WIDTH - 312, 10, 301, 37};

    // Keystates
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Player
    auto player = std::make_shared< Player >(engine.camera, 100, 100);
    engine.initEntity(player, phraktal::assets::PLAYER_PNG);

    // Main loop
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        capTimer.start();
        player->handleKeystates(keystates);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else
            {
                engine.handleEvents(e);
            }
        }

        // FPS
        /*
        averageFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (averageFPS > 20000)
        {
            averageFPS = 0;
        }
        fpsText.str("");
        fpsText << "FPS: " << averageFPS;
        engine.updateText(fps, fpsText.str());
         */

        // Stats
        statsText.str("");
        statsText << engine.getStats();
        engine.updateText(stats, statsText.str());

        // Delta time
        float dTime = deltaTimer.getTicks() / 1000.f;

        // Update all entities
        engine.updateEntities(dTime);
        engine.resetGrid();

        // Update coin count
        engine.updateText(coinCount, std::to_string(player->getCoinCount()));

        // Camera
        engine.camera.update();

        deltaTimer.start();

        // Render textures
        engine.setDrawColor(0x00, 0x00, 0x00, 0xFF);
        engine.rendererClear();
        engine.renderEntity(background);
        engine.renderEntities();

        {
            int percentage = (int) (((float) player->getShotCooldown() / (float) player->getMaxShotCooldownTime()) * 100.f);

            // Player shot cooldown bar width
            if (player->getShotCooldown() != 0)
            {
                filled->w = percentage * 3;
            }

            auto color = player->getChargeBarColor();

            engine.setDrawColor(color->r, color->g, color->b, color->a);
        }
        engine.renderRectangleFilled(*filled);
        engine.setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
        engine.renderRectangleOutline(*barOutline);

        engine.rendererPresent();

        // FPS
//        countedFrames++;
        Uint32 frameTicks = capTimer.getTicks();
        if (frameTicks < phraktal::levels::SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(phraktal::levels::SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }
}