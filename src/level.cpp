#include <fstream>
#include <cstring>
#include "level.h"

Level::Level()
{
    //
}

/*
void Level::addMimic(std::shared_ptr< Mimic > mimic)
{
    this->mimics.push_back(std::weak_ptr< Mimic >(mimic));
}

void Level::removeMimic(std::shared_ptr< Mimic > mimic)
{
    this->mimics.push_back(std::weak_ptr< Mimic >(mimic));
}
 */

void Level::loadFromFile(const std::string level, const std::shared_ptr< SDL_Renderer > renderer)
{
    std::ifstream file(level);

    std::vector< int > row;
    std::string line;
    for (int i = 0; std::getline(file, line); i++)
    {
        char *token = std::strtok(strdup(line.c_str()), " ");
        while (token != NULL)
        {
            row.push_back((int) std::stoi(token, 0, 16));
            token = strtok(NULL, " ");
        }
        this->levelMap.push_back(row);
        row.clear();
        row.shrink_to_fit();
    }

    file.close();

    std::unique_ptr< TextureW > texture(new TextureW());

    // Void
    texture->setRenderer(renderer);
    texture->loadTexture(phraktal::assets::VOID_PNG);
    this->tileTextures.push_back(std::move(texture));
    texture.reset(new TextureW());

    // Grass
    texture->setRenderer(renderer);
    texture->loadTexture(phraktal::assets::GRASS_PNG);
    this->tileTextures.push_back(std::move(texture));
    texture.reset(new TextureW());

    // Stone
    texture->setRenderer(renderer);
    texture->loadTexture(phraktal::assets::STONE_PNG);
    this->tileTextures.push_back(std::move(texture));
    texture.reset(new TextureW());
}

void Level::render(const std::shared_ptr<Camera> camera) const
{
    long x, y;
    std::unique_ptr<SDL_Rect> rect(new SDL_Rect);
    for (auto row = this->levelMap.begin(); row != this->levelMap.end(); row++)
    {
        x = row - this->levelMap.begin();
        for (auto col = row->begin(); col != row->end(); col++)
        {
            y = col - row->begin();
            rect->x = (int) (x * phraktal::levels::TILE_SIZE);
            rect->y = (int) (y * phraktal::levels::TILE_SIZE);
            rect->w = phraktal::levels::TILE_SIZE;
            rect->h = phraktal::levels::TILE_SIZE;
            if (SDL_HasIntersection(camera->getRect().get(), rect.get()))
            {
//                this->tileTextures[*col]->renderTexture((int) x * phraktal::levels::TILE_SIZE - (int) camera->pos.x, (int) y * phraktal::levels::TILE_SIZE - (int) camera->pos.y);
            }
        }
    }
}

int Level::getWidth() const
{
    return width;
}

int Level::getHeight() const
{
    return height;
}
