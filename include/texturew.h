#ifndef TEXTUREW_H
#define TEXTUREW_H

#include <stdlib.h>
#include <iostream>

#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "log.h"

class TextureW
{
public:
    TextureW();
    ~TextureW();
    bool setRenderer(SDL_Renderer* ren);
    bool isRendererSet();
    SDL_Renderer* getRenderer();
    void clearRenderer();
    bool loadTexture(std::string filePath);
    bool loadTextureFromText(std::string text, SDL_Color color, int wrapped = 0);
    void clearTexture();
    int getWidth();
    int getHeight();
    bool setFont(std::string fontFile, int fontSize);
    bool isFontSet();
    void clearFont();
    void setFontColor(SDL_Color  color);
    void renderTexture(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE); 

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* texture;
    int tW;
    int tH;
};

#endif
