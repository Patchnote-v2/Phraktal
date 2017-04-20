#ifndef PHRAKTAL_UTILS_H
#define PHRAKTAL_UTILS_H

#include <string>

#include "SDL2/SDL.h"
#include "SDL_ttf.h"

namespace phraktal
{
    namespace levels
    {
        const float PI = 3.1415926;

        const Uint32 SCREEN_FPS = 60;
        const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

        // Level numbers
        const int LEVEL_WIDTH = 1920;
//        const int LEVEL_WIDTH = 1080;
        const int LEVEL_HEIGHT = 1080;
//        const int LEVEL_HEIGHT = 480;
        const int SCREEN_WIDTH = 1280;
        const int SCREEN_HEIGHT = 720;
        const int TILE_SIZE = 100;

        const int DEFAULT_FONT_SIZE = 18;

        const int MAX_SHOT_POWER = 2000;

        const std::string LEVEL1_TXT = "assets/levels/level1.txt";
    }

    namespace assets
    {
        const std::string DEFAULT_FONT = "assets/images/sample.ttf";

        const std::string BG_PNG = "assets/images/bg.png";
        const std::string POWERBAR_PNG = "assets/images/powerbar.png";
        const std::string PLAYER_PNG = "assets/images/player.png";
        const std::string ENEMY_PNG = "assets/images/enemy.png";
        const std::string PLAYER_BULLET_PNG = "assets/images/playerbullet.png";
        const std::string ENEMY_BULLET_PNG = "assets/images/enemybullet.png";

        const std::string SPREAD_POWERUP_PNG = "assets/images/spreadpowerup.png";

        const std::string VOID_PNG = "assets/images/void.png";
        const std::string GRASS_PNG = "assets/images/grass.png";
        const std::string STONE_PNG = "assets/images/stone.png";
    }

    namespace tiles
    {
        const int NUM_TILES = 0x03;
        const int VOID = 0x00;
        const int GRASS = 0x01;
        const int STONE = 0x02;
    }

    namespace utils
    {
        struct SDL_Deleter
        {
            void operator()(SDL_Window* win) const
            { SDL_DestroyWindow(win); }

            void operator()(SDL_Renderer* ren) const
            { SDL_DestroyRenderer(ren); }

            void operator()(SDL_Texture* tex) const
            { SDL_DestroyTexture(tex); }

            void operator()(TTF_Font* font) const
            { TTF_CloseFont(font); }
        };

        /*
        inline bool almostEqual(float A, float B, float maxDiff, float maxRelDiff = FLT_EPSILON)
        {
            // Check if the numbers are really close -- needed
            // when comparing numbers near zero.
            float diff = fabs(A - B);
            if (diff <= maxDiff)
                return true;

            A = fabs(A);
            B = fabs(B);
            float largest = (B > A) ? B : A;

            if (diff <= largest * maxRelDiff)
            {
                return true;
            }

            return false;
        }
         */
        // todo: old code for destination navigation
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
    }
}

#endif //PHRAKTAL_UTILS_H
