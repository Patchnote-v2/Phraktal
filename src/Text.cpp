#include "Text.h"

Text::Text(Camera &camera, int x, int y) :
        Entity(camera, x, y),
        text()
{}

Text::Text(Camera &camera, std::string text, int x, int y) :
        Entity(camera, x, y),
        text(text)
{}

std::string Text::getText() const
{
    return this->text;
}

/*
 * Protected methods
 */

void Text::updateText(std::string text)
{
    this->text = text;
}
