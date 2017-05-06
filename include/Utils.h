#ifndef PHRAKTAL_UTILS_H
#define PHRAKTAL_UTILS_H

#include <string>

#include "SDL2/SDL.h"
#include "SDL_ttf.h"

namespace phraktal
{
    namespace levels
    {
        const float PI = 3.1415926f;

        const Uint32 SCREEN_FPS = 60;
        const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

        // Level numbers
        const int LEVEL_WIDTH = 1920;
        const int LEVEL_HEIGHT = 1080;
        const int SCREEN_WIDTH = 1280;
        const int SCREEN_HEIGHT = 720;
        const int TILE_SIZE = 100;

        const int DEFAULT_FONT_SIZE = 18;

        const int MAX_REGULAR_ENEMY_SPEED = 100;
        const int MAX_SPREAD_ENEMY_SPEED = 75;
        const int MAX_LARGE_ENEMY_SPEED = 50;

        const int MAX_PLAYER_REGULAR_SHOT_SPEED = 1000;
        const int MAX_PLAYER_SPREAD_SHOT_SPEED = 2000;
        const int MAX_PLAYER_LARGE_SHOT_SPEED = 500;

        const int MAX_ENEMY_REGULAR_SHOT_SPEED = 1000;
        const int MAX_ENEMY_SPREAD_SHOT_SPEED = 1250;
        const int MAX_ENEMY_LARGE_SHOT_SPEED = 500;

        const int REGULAR_PLAYER_SHOT_COOLDOWN = 250;
        const int SPREAD_PLAYER_SHOT_COOLDOWN = 250;
        const int LARGE_PLAYER_SHOT_COOLDOWN = 750;

        const int REGULAR_ENEMY_SHOT_COOLDOWN = 2000;
        const int SPREAD_ENEMY_SHOT_COOLDOWN = 2500;
        const int LARGE_ENEMY_SHOT_COOLDOWN = 3000;

        const int REGULAR_PLAYER_SHOT_DAMAGE = 2000;
        const int SPREAD_PLAYER_SHOT_DAMAGE = 1000;
        const int LARGE_PLAYER_SHOT_DAMAGE = 6000;

        const int REGULAR_ENEMY_SHOT_DAMAGE = 2000;
        const int SPREAD_ENEMY_SHOT_DAMAGE = 1000;
        const int LARGE_ENEMY_SHOT_DAMAGE = 6000;

        const int ENEMY_REGULAR_HEALTH = 6000;
        const int ENEMY_SPREAD_HEALTH = 3000;
        const int ENEMY_LARGE_HEALTH = 10000;

        const float PERCENT_TO_COLOR_CONVERSION = .392f;
        const float HALF_PERCENT_TO_COLOR_CONVERSION = .196f;

        const std::string ONE_TXT = "assets/levels/UpgradeMenu.json";
    }

    namespace assets
    {
        const std::string DEFAULT_FONT = "assets/images/sample.ttf";

        const std::string BG_PNG = "assets/images/bg.png";
        const std::string PLAYER_PNG = "assets/images/player.png";

        const std::string REGULAR_ENEMY_PNG = "assets/images/enemy.png";
        const std::string SPREAD_ENEMY_PNG = "assets/images/spreadenemy.png";
        const std::string LARGE_ENEMY_PNG = "assets/images/largeenemy.png";

        const std::string REGULAR_PLAYER_BULLET_PNG = "assets/images/playerbullet.png";
        const std::string SPREAD_PLAYER_BULLET_PNG = "assets/images/spreadplayerbullet.png";
        const std::string LARGE_PLAYER_BULLET_PNG = "assets/images/largeplayerbullet.png";

        const std::string REGULAR_ENEMY_BULLET_PNG = "assets/images/enemybullet.png";
        const std::string SPREAD_ENEMY_BULLET_PNG = "assets/images/spreadenemybullet.png";
        const std::string LARGE_ENEMY_BULLET_PNG = "assets/images/largeenemybullet.png";

        const std::string SPREAD_POWERUP_PNG = "assets/images/spreadpowerup.png";
        const std::string LARGE_BULLET_POWERUP_PNG = "assets/images/largebulletpowerup.png";

        const std::string COIN_PNG = "assets/images/coin.png";
        const std::string COIN_COUNT_GRAPHIC_PNG = "assets/images/coincountgraphic.png";

//        const std::string VOID_PNG = "assets/images/void.png";
//        const std::string GRASS_PNG = "assets/images/grass.png";
//        const std::string STONE_PNG = "assets/images/stone.png";
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

#endif //PHRAKTAL_UTILS_H
