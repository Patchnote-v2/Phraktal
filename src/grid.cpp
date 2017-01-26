#include <sstream>
#include "base/grid.h"

Grid::Grid(int width, int height, int cellSize, std::shared_ptr< SDL_Renderer > renderer) : width(width), height(height), cellSize(cellSize)
{
    SDL_Color color{255, 255, 255, 255};
    for (int w = 0; w < this->width / cellSize; w++)
    {
        std::vector< std::set< std::shared_ptr< Mimic > > > vector;
        std::vector< std::unique_ptr< SDL_Rect > > vectorRects;
        std::vector< std::unique_ptr< TextureW > > textureVect;

        for (int h = 0; h < this->height / cellSize; h++)
        {
            std::set< std::shared_ptr< Mimic > > set;
            vector.push_back(set);

            std::unique_ptr< SDL_Rect > rect(new SDL_Rect{w * cellSize, h * cellSize, cellSize, cellSize});
            vectorRects.push_back(std::move(rect));

            std::unique_ptr< TextureW > texture(new TextureW());
            texture->setRenderer(renderer);
            texture->setFont(phraktal::assets::DEFAULT_FONT, 16);
            texture->loadTextureFromText("0", color);
            textureVect.push_back(std::move(texture));
        }
        this->grid.push_back(vector);
        this->rects.push_back(std::move(vectorRects));
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

void Grid::removeMimic(std::shared_ptr< Mimic > mimic)
{
    for (int col = 0; col < this->grid.size(); col++)
    {
        for (int row = 0; row < this->grid[col].size(); row++)
        {
            auto search = this->grid[col][row].find(mimic);
            if (search != this->grid[col][row].end())
            {
                this->grid[col][row].erase(mimic);
            }
        }
    }
}

void Grid::updateMimic(std::shared_ptr< Mimic > mimic)
{
    auto rect = mimic->getRect();

    rect->x = (int) mimic->getOldPos()->x;
    rect->y = (int) mimic->getOldPos()->y;

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
            this->grid[col][row].erase(mimic);
            this->addMimic(mimic);
        }
    }
}

void Grid::render(std::shared_ptr< SDL_Renderer > renderer, std::shared_ptr< Camera > camera)
{
    int width = 0, height = 0;
    std::stringstream text;
    text.str("");
    SDL_Color color{255, 255, 255, 255};

    for (auto &w : this->rects)
    {
        for (auto &h : w)
        {
            h->x -= camera->pos.x;
            h->y -= camera->pos.y;
            if (!this->grid[width][height].empty())
            {
                SDL_RenderDrawRect(renderer.get(), h.get());
                text.str("");
                text << this->grid[width][height].size();
                this->stats[width][height]->loadTextureFromText(text.str(), color, 0);
                this->stats[width][height]->renderTexture(h->x + 5, h->y + 5);
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
