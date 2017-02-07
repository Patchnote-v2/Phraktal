#include <sstream>
#include "Engine.h"

/*
 * Public functions
 */

Engine::Engine() :
        camera(0, 0, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT),
        grid(phraktal::levels::LEVEL_WIDTH, phraktal::levels::LEVEL_HEIGHT, phraktal::levels::TILE_SIZE),
        hitCounter(0)
{
    // Initialize SDL, IMG, and TTF libraries
    if (this->init() == 1)
    {
        return;
    }

    // Initialize window and set to class member
    phraktal::utils::SDL_Deleter d;
    std::unique_ptr <SDL_Window, phraktal::utils::SDL_Deleter> window(
            SDL_CreateWindow(
                    "Phraktal", 100, 100, phraktal::levels::SCREEN_WIDTH, phraktal::levels::SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
            d);

    if (window == nullptr)
    {
        Log::log(std::cout, "CreateWindow");
        SDL_Quit();
        return;
    }
    else
    {
        this->window = std::move(window);
    }

    // Initialize renderer and set to class member
    std::shared_ptr< SDL_Renderer > renderer(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED), d);// | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        Log::log(std::cout, "CreateRenderer");
        SDL_Quit();
        return;
    }
    else
    {
        this->renderer = renderer;
    }

    this->grid.initTextures(this->renderer);
    this->grid.clear();
}

void Engine::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(this->renderer.get(), r, g, b, a);
}

void Engine::rendererClear()
{
    SDL_RenderClear(this->renderer.get());
}

void Engine::rendererPresent()
{
    SDL_RenderPresent(this->renderer.get());
}

void Engine::resetLevel()
{
    this->grid.clear();
    this->entities.clear();
    this->entities.shrink_to_fit();
    this->bullets.clear();
    this->bullets.shrink_to_fit();
    this->entities.push_back(this->player);
    this->grid.addEntity(this->player);
    this->player->setPos(0, 0);
}

void Engine::resetGrid()
{
    this->grid.clear();
    for (auto entity : this->entities)
    {
        this->grid.addEntity(entity);
    }
    for (auto bullet : this->bullets)
    {
        this->grid.addEntity(bullet);
    }
}

void Engine::initText(std::shared_ptr< Text > text, int fontSize)
{
    // todo: ability to select different fonts, and colors
    text->texture = std::unique_ptr< TextureW >(new TextureW());
    text->texture->setRenderer(this->renderer);
    text->texture->setFont(phraktal::assets::DEFAULT_FONT, fontSize);
    SDL_Color color = {255, 255, 255, 255};
    text->texture->loadTextureFromText(text->text, color);
}

void Engine::updateText(std::shared_ptr< Text > text, std::string textStr)
{
    SDL_Color color = {255, 255, 255, 255};
    text->updateText(textStr);
    text->texture->loadTextureFromText(textStr, color);
}

void Engine::updateEntities(float dTime)
{
    for (auto entity : this->entities)
    {
        entity->update(dTime);

        // Autofire enemy bullets
        if (entity->getType() == Entity::Type::ENEMY)
        {
            auto derived = std::dynamic_pointer_cast< Enemy >(entity);
            if ((int) derived->shotCooldown > 3)
            {
                auto bullet = std::make_shared< Bullet >(camera, (int) derived->getCenter().x, (int) derived->getCenter().y, 200, Entity::Type::ENEMY_BULLET);
                this->initEntity(bullet, phraktal::assets::ENEMY_BULLET_PNG);
                bullet->setDestination((int) this->player->getCenter().x, (int) this->player->getCenter().y);

                derived->shotCooldown = 0;

                this->bullets.push_back(bullet);
                this->grid.addEntity(bullet);
            }

            auto found = this->grid.query(entity);
            if (found.size())
            {
                for (auto k = found.begin(); k != found.end(); k++)
                {
                    for (auto l = k; l != found.end(); l++)
                    {
                        if (k == l)
                        {
                            continue;
                        }
                        else
                        {
                            if ((l->get()->getType() == Entity::Type::ENEMY ||
                                    l->get()->getType() == Entity::Type::PLAYER_BULLET))
                            {
                                if (k->get()->isColliding(*l))
                                {
                                    this->hitCounter++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < this->bullets.size(); i++)
    {
        this->bullets[i]->update(dTime);
        if (!this->bullets[i]->isInLevel())
        {
            this->bullets.erase(this->bullets.begin() + i);
        }
    }
}



void Engine::renderEntities()
{
    for (auto entity : this->entities)
    {
        this->renderEntity(entity);
    }
    this->grid.render(this->renderer, this->camera);
    for (auto bullet : this->bullets)
    {
        this->renderEntity(bullet);
    }
    this->renderEntity(this->player);
}

std::string Engine::getVectorStats() const
{
    std::stringstream string;
    string << "Entities: " << this->entities.size() << std::endl;
    string << "Bullets: " << this->bullets.size() << std::endl;
    string << "Hits: " << this->hitCounter << std::endl;
    return string.str();
}

/*
 * Private functions
 */

int Engine::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        Log::log(std::cout, "SDL_Init");
        return 1;
    }

    // Initialize IMG
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        Log::log(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    // Initialize TTF
    if (TTF_Init() != 0)
    {
        Log::log(std::cout, "TTF_Init");
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    return 0;
}

void Engine::quitSDL()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

std::shared_ptr< SDL_Renderer > Engine::getRen() const
{
    return this->renderer;
}