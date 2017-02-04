#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>

#include "MoveableEntity.h"

const int MAX_X_VEL = 512;
const int MAX_Y_VEL = 512;

namespace phraktal
{
    class Player : public MoveableEntity
    {
    public:
        Player(Camera& camera, int x, int y);
        ~Player() {};

        void handleEvents(SDL_Event& e);
        void handleKeystates(const Uint8* keystates);
        void update(float dTime) override;
    private:
        float accelerationSpeed;
        float decelerationSpeed;
        std::map< SDL_Scancode, bool > keystates;
    };
}
#endif