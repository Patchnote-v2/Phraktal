#ifndef TOPDOWN_GRID_H
#define TOPDOWN_GRID_H

#include <vector>
#include <set>

#include <SDL.h>
#include <MoveableEntity.h>

namespace phraktal
{
    class Grid
    {
    public:
        std::vector< std::vector< std::set< const Entity& > > > grid;
        std::vector< std::vector< SDL_Rect > > rects;

        Grid(const int width, const int height, const int cellSize);
        void clear();
        void addEntity(Entity const& entity);
        void removeEntity(Entity const& entity);
        void updateMoveableEntity(MoveableEntity const& mimic);
        std::set< Entity > query(Entity mimic);

    private:
        int width, height;
        int cellSize;
    };
}
#endif
