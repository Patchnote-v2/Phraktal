#include "Grid.h"

Grid::Grid(int width, int height, int cellSize)
        : width(width), height(height), cellSize(cellSize)
{
    for (int w = 0; w < this->width / cellSize; w++)
    {
        std::vector< std::set< std::shared_ptr<  Entity > > > entityRow;
        std::vector< SDL_Rect > rectRow;

        for (int h = 0; h < this->height / cellSize; h++)
        {
            std::set< std::shared_ptr< Entity > > set;
            entityRow.push_back(set);

            SDL_Rect rect = {w * cellSize, h * cellSize, cellSize, cellSize};
            rectRow.push_back(rect);
        }
        this->grid.push_back(entityRow);
        this->rects.push_back(rectRow);
    }
}


void Grid::initTextures(std::shared_ptr< SDL_Renderer > renderer)
{
    SDL_Color color{255, 255, 255, 255};
    for (int w = 0; w < this->width / cellSize; w++)
    {
        std::vector< std::unique_ptr< TextureW > > textureVect;

        for (int h = 0; h < this->height / cellSize; h++)
        {
            std::unique_ptr< TextureW > texture(new TextureW());
            texture->setRenderer(renderer);
            texture->setFont(phraktal::assets::DEFAULT_FONT, 16);
            texture->loadTextureFromText("0", color);
            textureVect.push_back(std::move(texture));
        }
        this->stats.push_back(std::move(textureVect));
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

void Grid::addEntity(std::shared_ptr< Entity > entity)
{
    auto rect = entity->getRect();

    int x = std::max(0, rect->x / this->cellSize);
    int y = std::max(0, rect->y / this->cellSize);
    int x2 = phraktal::levels::LEVEL_WIDTH / this->cellSize < (rect->x + rect->w) / this->cellSize
             ? phraktal::levels::LEVEL_WIDTH / this->cellSize
             : (rect->x + rect->w) / this->cellSize;
    int y2 = phraktal::levels::LEVEL_HEIGHT / this->cellSize < (rect->y + rect->h) / this->cellSize
             ? phraktal::levels::LEVEL_HEIGHT / this->cellSize
             : (rect->y + rect->h) / this->cellSize;

    for (int col = x; col <= x2 && col < this->grid.size(); col++)
    {
        for (int row = y; row <= y2 && row < this->grid[col].size(); row++)
        {
            this->grid[col][row].insert(entity);
        }
    }
}

void Grid::removeEntity(std::shared_ptr< Entity > entity)
{
    for (int col = 0; col < this->grid.size(); col++)
    {
        for (int row = 0; row < this->grid[col].size(); row++)
        {
            auto search = this->grid[col][row].find(entity);
            if (search != this->grid[col][row].end())
            {
                this->grid[col][row].erase(entity);
            }
        }
    }
}

std::set< std::shared_ptr< Entity > > Grid::query(std::shared_ptr< Entity > entity)
{
    std::set< std::shared_ptr< Entity > > found;

    auto rect = entity->getRect();

    int x = std::max(0, rect->x / this->cellSize);
    int y = std::max(0, rect->y / this->cellSize);
    int x2 = phraktal::levels::LEVEL_WIDTH / this->cellSize < (rect->x + rect->w) / this->cellSize
             ? phraktal::levels::LEVEL_WIDTH / this->cellSize
             : (rect->x + rect->w) / this->cellSize;
    int y2 = phraktal::levels::LEVEL_HEIGHT / this->cellSize < (rect->y + rect->h) / this->cellSize
             ? phraktal::levels::LEVEL_HEIGHT / this->cellSize
             : (rect->y + rect->h) / this->cellSize;

    for (int col = x; col <= x2 && col < this->grid.size(); col++)
    {
        for (int row = y; row <= y2 && row < this->grid[col].size(); row++)
        {
            if (this->grid[col][row].find(entity) != this->grid[col][row].end())
            {
                found.insert(this->grid[col][row].begin(), this->grid[col][row].end());
            }
        }
    }

    return found;
}

