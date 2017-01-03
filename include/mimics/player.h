#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>

#include "bullet.h"
#include "camera.h"

const int MAX_SPEED = 255;

class Player : public Mimic
{
public:
    Type type;
    Player(std::shared_ptr< Camera > camera) : Mimic(camera), type(Type::PLAYER) {};
    ~Player() {};

    void setPos(int x, int y);

    void handleEvents(SDL_Event& e) override;
    void handleKeystates(const Uint8* keystates);
    void update(float dTime) override;
    void render() override;
private:
    void updateCenter();
    void updateAngle();
};
#endif