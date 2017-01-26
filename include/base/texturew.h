#ifndef TEXTUREW_H
#define TEXTUREW_H

#include <stdlib.h>
#include <iostream>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "log.h"
#include "utils.h"

class TextureW
{
public:
    TextureW();
    ~TextureW();
    bool setRenderer(std::shared_ptr< SDL_Renderer > ren);
    bool isRendererSet();
    std::shared_ptr< SDL_Renderer > getRenderer();
    void clearRenderer();
    bool loadTexture(std::string filePath);
    bool loadTextureFromText(std::string text, SDL_Color color, int wrapped = 0);
    void clearTexture();
    int getWidth();
    int getHeight();
    bool setFont(std::string fontFile, int fontSize);
    bool isFontSet();
    void clearFont();
    void setFontColor(SDL_Color color);
    void renderTexture(int x, int y, std::shared_ptr< SDL_Rect > clip = NULL, double angle = 0.0,
                       std::unique_ptr< SDL_Point > center = NULL,
                       SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    std::shared_ptr< SDL_Renderer > renderer;
    std::unique_ptr< TTF_Font, phraktal::utils::SDL_Deleter > font;
    std::unique_ptr< SDL_Texture, phraktal::utils::SDL_Deleter > texture;
    int w;
    int h;
};

#endif
