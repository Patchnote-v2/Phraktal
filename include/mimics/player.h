#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>

#include "bullet.h"
#include "camera.h"

const int MAX_X_VEL = 512;
const int MAX_Y_VEL = 512;

class Player : public Mimic
{
public:
    Type type;
    Player(std::shared_ptr< Camera > camera);
    ~Player() {};

    void setPos(int x, int y);

    Type getType();

    void handleEvents(SDL_Event& e) override;
    void handleKeystates(const Uint8* keystates);
    void update(float dTime) override;
    void render() override;
private:
    int accelerationSpeed;
    std::map< SDL_Scancode, bool > keystates;
    void updateCenter();
    void updateAngle();
};
#endif