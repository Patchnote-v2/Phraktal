#include "base/grid.h"

Grid::Grid(int width, int height, int cellSize) : width(width), height(height), cellSize(cellSize)
{
    std::cout << "Init grid" << std::endl;
    for (int w = 0; w < this->width / cellSize; w++)
    {
        std::vector< std::set< std::shared_ptr< Mimic > > > vector;

        std::vector< std::unique_ptr< SDL_Rect > > vectorRects;

        for (int h = 0; h < this->height / cellSize; h++)
        {
            std::set< std::shared_ptr< Mimic > > set;
            vector.push_back(set);

            std::unique_ptr< SDL_Rect > rect(new SDL_Rect{w * cellSize, h * cellSize, cellSize, cellSize});
            vectorRects.push_back(std::move(rect));
        }
        this->grid.push_back(vector);
        this->rects.push_back(std::move(vectorRects));
    }
}

void Grid::clear()
{
    for (unsigned int w = 0; w < this->grid.size(); w++)
    {
        for (unsigned int h = 0; h < this->grid[w].size(); h++)
        {
            grid[w][h].clear();
        }
    }
}

void Grid::addMimic(std::shared_ptr< Mimic > mimic)
{
    auto rect = mimic->getRect();

    int x = std::max(0, rect->x / this->cellSize);
    int y = std::max(0, rect->y / this->cellSize);
    int x2 = phraktal::levels::LEVEL_WIDTH < (rect->x + rect->w) / this->cellSize
             ? phraktal::levels::LEVEL_WIDTH
             : (rect->x + rect->w) / this->cellSize;
    int y2 = phraktal::levels::LEVEL_HEIGHT < (rect->y + rect->h) / this->cellSize
             ? phraktal::levels::LEVEL_HEIGHT
             : (rect->y + rect->h) / this->cellSize;

    for (int col = x; col <= x2 && col < this->grid.size(); col++)
    {
        for (int row = y; row <= y2 && row < this->grid[col].size(); row++)
        {
            this->grid[col][row].insert(mimic);
        }
    }
}

void Grid::render(std::shared_ptr< SDL_Renderer > renderer, std::shared_ptr< Camera > camera)
{
    int width = 0, height = 0;

    for (auto &w : this->rects)
    {
        for (auto &h : w)
        {
            h->x -= camera->pos.x;
            h->y -= camera->pos.y;
            if (this->grid[width][height].empty())
            {
                SDL_RenderFillRect(renderer.get(), h.get());
            }
            else
            {
                SDL_RenderDrawRect(renderer.get(), h.get());
            }
            h->x += camera->pos.x;
            h->y += camera->pos.y;
            height++;
        }
        width++;
        height = 0;
    }
}

std::set< Mimic > Grid::query(Mimic mimic)
{
    return std::set< Mimic >();
}
