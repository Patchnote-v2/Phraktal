#ifndef BULLET_H
#define BULLET_H

#include "mimic.h"
#include <memory>

#define PI 3.14159265

const std::string BULLET_PNG = "assets/bullet.png";

class Bullet : public Mimic
{
public:
    Bullet(Vector2 pos, std::shared_ptr< Camera > camera) : Mimic(camera), maxSpeed(100) {this->pos = pos;};
    ~Bullet() {};

    void setDestination(Vector2 destination);
    void setDestination(int x, int y);
    void setShotPower(int shotPower);
    bool isInLevel();
    void update(float dTime) override;
    void render() override;

private:
    int maxSpeed;
};
#endif