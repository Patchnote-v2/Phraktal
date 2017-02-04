#ifndef TOPDOWN_TEXT_H_H
#define TOPDOWN_TEXT_H_H

#include "Entity.h"

class Text : public Entity
{
public:
    Text(Camera &camera, int x, int y);
    Text(Camera &camera, std::string text, int x, int y);

    std::string getText() const;

    friend class Engine;

protected:
    std::string text;
    void updateText(std::string text);
};

#endif //TOPDOWN_TEXT_H_H
