#ifndef TOPDOWN_LEVEL_H
#define TOPDOWN_LEVEL_H

#include <memory>
#include <vector>

#include "mimic.h"

class Mimic;

class Level
{
public:
    std::vector< std::vector< int > > levelMap;
    std::vector< std::unique_ptr< TextureW > > tileTextures;

    Level();

//    void addMimic(std::shared_ptr< Mimic > mimic);
//    bool hasMimic(std::shared_ptr< Mimic > mimic);
//    void removeMimic(std::shared_ptr< Mimic > mimic);

    void loadFromFile(const std::string level, const std::shared_ptr< SDL_Renderer > renderer);
    void render() const;

    int getWidth() const;
    int getHeight() const;

private:
    std::vector< std::weak_ptr< Mimic > > mimics;
    std::shared_ptr< SDL_Renderer > renderer;
    int width;
    int height;
};

#endif
