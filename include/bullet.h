#ifndef BULLET_H
#define BULLET_H

#include "mimic.h"
#include <memory>

#define PI 3.14159265

const std::string BULLET_PNG = "assets/bullet.png";

class Bullet : public Mimic
{
public:
    Bullet(Vector2 pos);
    ~Bullet() {};

    void setDestination(Vector2 destination);
    bool inFrame();
    void update(float dTime) override;
    void render() override;

private:
    const int MAX_SPEED = 900;
};
#endif