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

bool Level::hasMimic(std::shared_ptr< Mimic > mimic)
{
    return false;
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
    texture->setRenderer(renderer);
    texture->loadTexture(phraktal::assets::VOID_PNG);
    this->tileTextures.push_back(std::move(texture));
}

void Level::render() const
{
    for (auto row = this->levelMap.begin(); row != this->levelMap.end(); row++)
    {
        for (auto col = row->begin(); col != row->end(); col++)
        {

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
