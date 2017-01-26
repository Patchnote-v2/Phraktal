#ifndef TOPDOWN_HUD_H
#define TOPDOWN_HUD_H

#include <base/texturew.h>
#include <vector>

class HudItem
{
public:
    HudItem(std::shared_ptr< SDL_Renderer > renderer, int x, int y, int totalClips);
    void setLevel(int level);
    void incrementLevel();
    void render();

private:
    std::vector< std::shared_ptr< SDL_Rect > > clips;
    std::shared_ptr< SDL_Renderer > renderer;
    std::unique_ptr< TextureW > texture;
    int x, y;
    int totalClips;
    int numClip;
};


#endif //TOPDOWN_HUD_H
