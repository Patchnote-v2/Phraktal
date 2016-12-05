#ifndef TOPDOWN_UTILS_H
#define TOPDOWN_UTILS_H

#include <iostream>
#include <cfloat>
#include <memory>

#include "SDL2/SDL.h"

namespace phraktal
{
    namespace utils
    {
        struct SDL_Deleter
        {
            void operator()(int i) const {std::cout << "Success" << i << std::endl;}
            void operator()(SDL_Window* win) const {SDL_DestroyWindow(win);}
            void operator()(SDL_Renderer* ren) const {SDL_DestroyRenderer(ren);}
            void operator()(SDL_Texture* tex) const {SDL_DestroyTexture(tex);}
            void operator()(TTF_Font* font) const {TTF_CloseFont(font);}
        };


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
    }
}

#endif