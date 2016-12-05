#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <iostream>
#include <vector>

#include "bullet.h"

#define PI 3.14159265

const int MAX_SPEED = 10;

const std::string PLAYER_PNG = "assets/player.png";

class Player : public Mimic
{
public:
    Type type;
    std::vector<Bullet*> bullets;
    Player() : Mimic(), type(Type::PLAYER) {};
    ~Player() {};

    void setPos(int x, int y);

    void handleEvents(SDL_Event& e) override;
    void handleKeystates(const Uint8* keystates);
    void update(float dTime) override;
    void render() override;
private:

    void updateAngle();
    void fireBullet();
};
#endif