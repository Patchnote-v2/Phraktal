#ifndef PHRAKTAL_GRID_H
#define PHRAKTAL_GRID_H

#include <vector>
#include <set>
#include <sstream>

#include "SDL2/SDL.h"

#include "MoveableEntity.h"

class Grid
{
public:
    std::vector< std::vector< std::set< std::shared_ptr< Entity > > > > grid;
    std::vector< std::vector< SDL_Rect > > rects;
    std::vector< std::vector< std::unique_ptr< TextureW > > > stats;

    Grid(const int width, const int height, const int cellSize);
    void initTextures(std::shared_ptr< SDL_Renderer > renderer);
    void clear();
    void addEntity(std::shared_ptr< Entity > entity);
    void removeEntity(std::shared_ptr< Entity > entity);
    std::set< std::shared_ptr< Entity > > query(std::shared_ptr< Entity > mimic);

private:
    int width, height;
    int cellSize;
};

#endif //PHRAKTAL_GRID_H