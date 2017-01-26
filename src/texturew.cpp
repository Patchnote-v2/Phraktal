#include "base/texturew.h"

TextureW::TextureW()
{
    this->renderer.reset();
    this->w = 0;
    this->h = 0;
}

TextureW::~TextureW()
{
    this->clearRenderer();
    this->clearFont();
    this->clearTexture();
}

bool TextureW::setRenderer(std::shared_ptr< SDL_Renderer > ren)
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

std::shared_ptr< SDL_Renderer > TextureW::getRenderer()
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
        this->texture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), surface));
        if (this->texture == nullptr)
        {
            Log::logSDLError(std::cout, "Unable to create texture from surface");
        }
        else
        {
            this->w = surface->w;
            this->h = surface->h;
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
        surface = TTF_RenderText_Blended_Wrapped(this->font.get(), text.c_str(), color, wrapped);
    }
    else
    {
        surface = TTF_RenderText_Solid(this->font.get(), text.c_str(), color);
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
        this->texture.reset(SDL_CreateTextureFromSurface(this->renderer.get(), surface));
        if (this->texture == nullptr)
        {
            Log::logSDLError(std::cout, "Unable to create texture from text");
        }
        else
        {
            this->w = surface->w;
            this->h = surface->h;
        }

        SDL_FreeSurface(surface);
    }

    return this->texture != nullptr;
}

void TextureW::clearTexture()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture.get());
        this->texture = nullptr;
        this->w = 0;
        this->h = 0;
    }
}

int TextureW::getWidth()
{
    return this->w;
}

int TextureW::getHeight()
{
    return this->h;
}

bool TextureW::setFont(std::string fontFile, int fontSize)
{
    if (!this->isFontSet())
    {
        this->font.reset(TTF_OpenFont(fontFile.c_str(), fontSize));
        if (this->font != nullptr)
        {
            return this->font != nullptr;
        }
        else
        {
            std::cout << "Unable to load font: " << fontFile << std::endl;
            return false;
        }
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
        TTF_CloseFont(this->font.get());
        this->font = nullptr;
    }
}

// x, y, clip, angle, center, flip
void TextureW::renderTexture(int x, int y, std::shared_ptr< SDL_Rect > clip, double angle,
                             std::unique_ptr< SDL_Point > center, SDL_RendererFlip flip)
{
    SDL_Rect destination = {x, y, this->w, this->h};

    if (clip != NULL)
    {
        destination.w = clip->w;
        destination.h = clip->h;
    }

    SDL_RenderCopyEx(this->renderer.get(), this->texture.get(), clip.get(), &destination, angle, center.get(), flip);
}
