#include "texturew.h"

TextureW::TextureW()
{
    this->renderer = nullptr;
    this->font = nullptr;
    this->texture = nullptr;
    this->tW = 0;
    this->tH = 0;
}
TextureW::~TextureW()
{
    this->renderer = nullptr;
    if (this->font != nullptr)
    {
        TTF_CloseFont(this->font);
        this->font = nullptr;
    }
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
        this->tW = 0;
        this->tH = 0;
    }
}

bool TextureW::setRenderer(SDL_Renderer* ren)
{
    if (ren == nullptr && this->isRendererSet())
    {
        Log::logSDLError(std::cout, "TextureW::setRenderer()");
        return true;
    }
    else
    {
        this->renderer = ren;
        return false;
    }
}

bool TextureW::isRendererSet()
{
    return !(this->renderer == nullptr);
}

SDL_Renderer* TextureW::getRenderer()
{
    if (this->isRendererSet())
    {
        return this->renderer;
    }
    else
    {
        return nullptr;
    }
}

void TextureW::clearRenderer()
{
    if (this->isRendererSet())
    {
        this->renderer = nullptr;
    }
}

bool TextureW::loadTexture(std::string filePath)
{    
    if (!this->isRendererSet())
    {
        Log::logSDLError(std::cout, "Renderer not set");
        return false;
    }

    SDL_Surface* surface = IMG_Load(filePath.c_str());

    if (surface == nullptr)
    {
        Log::logSDLError(std::cout, "Invalid filePath, unable to load texture");
        return false;
    }
    else
    {
        this->clearTexture();
        this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
        if (this->texture == nullptr)
        {
            Log::logSDLError(std::cout, "Unable to create texture from surface");
        }
        else
        {
            this->tW = surface->w;
            this->tH = surface->h;
        }

        SDL_FreeSurface(surface);
    }
    
    return this->texture != nullptr;
}

bool TextureW::loadTextureFromText(std::string text, SDL_Color color, int wrapped)
{
    SDL_Surface* surface;
    if (wrapped != 0)
    {
        surface = TTF_RenderText_Blended_Wrapped(this->font, text.c_str(), color, wrapped);
    }
    else
    {
        surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
    }

    if (surface == nullptr)
    {
        Log::logSDLError(std::cout, "Unable to create surface from text");
        SDL_FreeSurface(surface);
        return false;
    }
    else
    {
        this->clearTexture();
        this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
        if (this->texture == nullptr)
        {
            Log::logSDLError(std::cout, "Unable to create texture from text");
        }
        else
        {
            this->tW = surface->w;
            this->tH = surface->h;
        }

        SDL_FreeSurface(surface);
    }

    return this->texture != nullptr;
}

void TextureW::clearTexture()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
        this->tW = 0;
        this->tH = 0;
    }
}

int TextureW::getWidth()
{
    return this->tW;
}

int TextureW::getHeight()
{
    return this->tH;
}

bool TextureW::setFont(std::string fontFile, int fontSize)
{
    if (!this->isFontSet())
    {
        this->clearFont();
        this->font = TTF_OpenFont(fontFile.c_str(), fontSize);
        return this->font != nullptr;
    }
    else
    {
        return false;
    }
}

bool TextureW::isFontSet()
{
    return (this->font != nullptr);
}

void TextureW::clearFont()
{
    if (this->isFontSet())
    {
        TTF_CloseFont(this->font);
        this->font = nullptr;
    }
}

// x, y, clip, angle, center, flip
void TextureW::renderTexture(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect destination = {x, y, tW, tH};

    if (clip != NULL)
    {
        destination.w = clip->w;
        destination.h = clip->h;
    }

    SDL_RenderCopyEx(this->renderer, this->texture, clip, &destination, angle, center, flip);
}
