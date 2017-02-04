#ifndef TOPDOWN_GRID_H
#define TOPDOWN_GRID_H

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

    void render(std::shared_ptr< SDL_Renderer > renderer, Camera &camera);

private:
    int width, height;
    int cellSize;
};

#endif //TOPDOWN_GRID_H