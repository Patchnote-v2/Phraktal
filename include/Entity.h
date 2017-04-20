#ifndef PHRAKTAL_ENTITY_H
#define PHRAKTAL_ENTITY_H

#include "TextureW.h"
#include "Camera.h"

class Entity
{
public:
    enum class Type
    {
        SIMPLE_ENTITY,
        MOVEABLE_ENTITY,
        BACKGROUND_ENTITY,
        TEXT,
        PLAYER,
        ENEMY,
        PLAYER_BULLET,
        ENEMY_BULLET,
        POWERUP
    };

    Entity(Camera &camera, int x, int y);
    virtual ~Entity()
    {};

    virtual void loadTexture(std::string name);

    std::unique_ptr< SDL_Rect > getRect();
    virtual void setPos(int x, int y);
    Vector2 getPos() const;
    Vector2 getCenter() const;
    Entity::Type getType() const;
    int getAngle() const;

    bool isInLevel();

    virtual bool isColliding(std::shared_ptr< Entity > m2);
    virtual void update(float);
    virtual void render();

    friend class Engine;

protected:
    Camera &camera;
    std::unique_ptr< TextureW > texture;
    Vector2 pos;
    Vector2 center;

    Vector2 aim;
    float angle;

    Entity::Type type;

    void updateCenter();
    void updateAngle();
};

#endif //PHRAKTAL_ENTITY_H
