#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "vector2.h"
#include "mimic.h"
#include "texturew.h"

template <typename T>
class Quadtree
{
     static const int MAX_LEVEL = 3;

public:
    SDL_Rect* boundary;
    int level;
    std::vector< Quadtree* > quads;

    std::vector< T* > items;

    Quadtree(SDL_Rect* boundary, int level, Quadtree* root, Quadtree* parent);

    std::vector< T*> clear();
    bool insert(T* item);
    void update();
    bool remove(T* item);
    void render(SDL_Renderer* renderer);
    std::vector< T* > queryArea(SDL_Rect* area);
    bool isLeaf();

private:
    static const int MAX_ITEMS = 4;
    Quadtree* parent;
    Quadtree* root;

    TextureW* count;

    void subdivide();
    bool belongs(T* item);
    int getCount();
};

template <typename T>
Quadtree<T>::Quadtree(SDL_Rect* boundary, int level, Quadtree* root, Quadtree* parent)// : boundary(boundary)
{
    for (int i = 0; i < 4; i++)
    {
        this->quads.push_back(nullptr);
    }
    this->boundary = boundary;
    this->root = root == nullptr ? this : root;
    this->parent = parent == nullptr ? this : root;
    this->level = level;

    this->count = new TextureW();
    this->count->setFont("assets/sample.ttf", 16);
//    this->northWest = nullptr;
//    this->northEast = nullptr;
//    this->southWest = nullptr;
//    this->southEast = nullptr;
}

template <typename T>
std::vector< T* > Quadtree<T>::clear()
{
    if (this->quads[0] != nullptr)
    {
        for (Quadtree* quad : quads)
        {
            quad->clear();
        }
    }
    for (int i = 0; i < this->items.size(); i ++)
    {
        this->items[i] = nullptr;
    }
    std::vector< T* > temp = this->items;
    temp.clear();
    return this->items;
//    northWest->clear();
//    northWest = nullptr;
//    northEast->clear();
//    northEast = nullptr;
//    southWest->clear();
//    southWest = nullptr;
//    southEast->clear();
//    southEast = nullptr;
}

template <typename T>
bool Quadtree<T>::insert(T* item)
{
    std::cout << "Inserting" << std::endl;
    if (this->belongs(item))
    {
        // IF IS NOT IN QUAD
        for (T* vectorItem : this->items)
        {
            if (vectorItem == item)
            {
                std::cout << "Already in quad" << std::endl;
                return false;
            }
        }

        // IF IS AS DEEP AS CAN GO
        if (this->level == this->MAX_LEVEL)
        {
            std::cout << "Item belongs MAX LEVEL" << std::endl;
            this->items.push_back(item);
            return true;
        }
        // IF IS LEAF AND CAN HOLD MORE
        else if (this->quads[0] == nullptr && this->items.size() < MAX_ITEMS)
        {
            std::cout << "Item belongs/is leaf" << std::endl;
            this->items.push_back(item);
            return true;
        }
        // IF IS NOT LEAF
        else if (this->quads[0] != nullptr)
        {
            for (T* i : this->items)
            {
                if (item == i)
                {
                    return false;
                }
            }
            for (Quadtree* quad : this->quads)
            {
                if (quad->insert(item))
                {
                    return true;
                }
            }
            return false;
        }
        // IF ISN'T AS DEEP AS CAN GO AND CANNOT HOLD MORE AND IS A LEAF
        else
        {
            std::cout << "Too many items" << std::endl;
            this->subdivide();

            // Reinsert
            for (Quadtree* quad : this->quads)
            {
                // New item
                quad->insert(item);
                for (T* itemVector : this->items)
                {
                    // Current items
                    quad->insert(itemVector);
                }
            }
            this->items.clear();
            return true;
        }
    }
    else
    {
        return false;
    }
}

template <typename T>
void Quadtree<T>::update()
{
//    std::cout << "Update: " << this->level << std::endl;
    if (this->getCount() > MAX_ITEMS)
    {

    }
    if (this->quads[0] == nullptr && this->items.empty())
    {
        this->clear();
        return;
    }
    T* temp;
    for (T* item : this->items)
    {
        if (item->hasMoved())
        {
            std::cout << &item << " has moved." << std::endl;
            temp = item;
            if (this->remove(item))
            {
                this->parent->insert(temp);
            }
        }
    }
    if (this->quads[0] != nullptr)
    {
        for (Quadtree* quad : quads)
        {
            quad->update();
        }
    }
}

template <typename T>
bool Quadtree<T>::remove(T* item)
{
    for (int i = 0; i < this->items.size(); i++)
    {
        if (this->items[i] == item)
        {
            if (this->quads[0] != nullptr)
            {
                for (Quadtree* quad : this->quads)
                {
                    quad->remove(this->items[i]);
                }
            }
            this->items.erase(this->items.begin() + i);
            return true;
        }
    }
    return false;
}

template <typename T>
void Quadtree<T>::render(SDL_Renderer* renderer)
{
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    std::stringstream numItemsText;
    this->count->setRenderer(renderer);

    numItemsText << this->items.size();
    this->count->loadTextureFromText(numItemsText.str().c_str(), color);
    this->count->renderTexture(this->boundary->x + (this->level * 5), this->boundary->y + (this->level * 5));
    SDL_RenderDrawRect(renderer, this->boundary);
    if (this->quads[0] != nullptr)
    {
        for (int i = 0; i < this->quads.size(); i++)
        {
            this->quads[i]->render(renderer);
        }
    }
}

template <typename T>
void Quadtree<T>::subdivide()
{
    int childWidth = this->boundary->w / 2;
    int childHeight = this->boundary->h / 2;

    SDL_Rect* northWestBoundary = new SDL_Rect();
    northWestBoundary->x = this->boundary->x;
    northWestBoundary->y = this->boundary->y;
    northWestBoundary->w = childWidth;
    northWestBoundary->h = childHeight;

    SDL_Rect* northEastBoundary = new SDL_Rect();
    northEastBoundary->x = this->boundary->x + childWidth;
    northEastBoundary->y = this->boundary->y;
    northEastBoundary->w = childWidth;
    northEastBoundary->h = childHeight;

    SDL_Rect* southWestBoundary = new SDL_Rect();
    southWestBoundary->x = this->boundary->x;
    southWestBoundary->y = this->boundary->y + childHeight;
    southWestBoundary->w = childWidth;
    southWestBoundary->h = childHeight;

    SDL_Rect* southEastBoundary = new SDL_Rect();
    southEastBoundary->x = this->boundary->x + childWidth;
    southEastBoundary->y = this->boundary->y + childHeight;
    southEastBoundary->w = childWidth;
    southEastBoundary->h = childHeight;

    this->quads[0] = new Quadtree(northWestBoundary, this->level + 1, this->root, this);
    this->quads[1] = new Quadtree(northEastBoundary, this->level + 1, this->root, this);
    this->quads[2] = new Quadtree(southWestBoundary, this->level + 1, this->root, this);
    this->quads[3] = new Quadtree(southEastBoundary, this->level + 1, this->root, this);
}

template <typename T>
bool Quadtree<T>::belongs(T* item)
{
    return SDL_HasIntersection(item->getRect(), this->boundary);
}

template <typename T>
int Quadtree<T>::getCount()
{
    int total;
    if (this->quads[0] != nullptr)
    {
        for (Quadtree* quad : this->quads)
        {
            total += quad->getCount();
        }
        return total;
    }
    else
    {
        return this->items.size();
    }
}

template <typename T>
bool Quadtree<T>::isLeaf()
{
    return this->quads[0] == nullptr;
}

#endif