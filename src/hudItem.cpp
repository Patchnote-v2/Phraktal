#include "hudItem.h"

HudItem::HudItem(std::shared_ptr< SDL_Renderer > renderer, int x, int y, int totalClips)
        : renderer(renderer), x(x), y(y), totalClips(totalClips), numClip(0)
{
    texture = std::unique_ptr< TextureW >(new TextureW());
    texture->setRenderer(renderer);
    texture->loadTexture(phraktal::assets::POWERBAR_PNG);

    for (int i = 0; i < totalClips; i++)
    {
        auto rect = std::make_shared< SDL_Rect >();
        rect->x = 0;
        rect->y = i * 50;
        rect->w = 150;
        rect->h = 50;

        clips.push_back(rect);
    }
}

void HudItem::setLevel(int level)
{
    this->numClip = level;
}

void HudItem::incrementLevel()
{
    if (this->numClip < totalClips - 1)
    {
        this->numClip++;
    }
}

void HudItem::render()
{
    this->texture->renderTexture(this->x, this->y, clips[numClip]);
}
