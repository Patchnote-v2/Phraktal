#ifndef BULLET_H
#define BULLET_H

#include "mimic.h"
#include <memory>

#define PI 3.14159265

const std::string BULLET_PNG = "assets/bullet.png";

class Bullet : public Mimic
{
public:
    Type type;
    Bullet(Vector2 pos, std::shared_ptr< Camera > camera) : Mimic(camera), type(Type::BULLET) {this->pos = pos;};
    ~Bullet() {};

    void setDestination(Vector2 destination);

    void setDestination(int x, int y);
    bool inFrame();
    void update(float dTime) override;
    void render() override;

private:
    const int MAX_SPEED = 900;
};
#endif