#ifndef TOPDOWN_GRID_H
#define TOPDOWN_GRID_H

#include <vector>
#include <set>

#include "base/utils.h"
#include "mimics/mimic.h"

class Grid
{
public:
    std::vector< std::vector< std::set< std::shared_ptr< Mimic > > > > grid;
    std::vector< std::vector< std::unique_ptr< SDL_Rect > > > rects;
    Grid(int width, int height, int cellSize);

    void clear();
    void addMimic(std::shared_ptr< Mimic > mimic);
    void render(std::shared_ptr< SDL_Renderer > renderer, std::shared_ptr< Camera > camera);
    std::set< Mimic > query(Mimic mimic);

private:
    int width, height;
    int cellSize;
};

#endif
