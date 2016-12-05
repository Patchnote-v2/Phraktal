#ifndef QUADTREE_H
#define QUADTREE_H

#include "SDL2/SDL.h"

#include <vector>
#include <algorithm>
#include <memory>
#include "vector2.h"
#include "mimic.h"
#include "texturew.h"

template <typename T>
class Quadtree : public std::enable_shared_from_this<Quadtree<T>>
{
     static const int MAX_LEVEL = 3;

public:
    std::unique_ptr< SDL_Rect > boundary;
    int level;
    std::vector< std::shared_ptr< Quadtree<T> > > quads;

    std::vector< std::shared_ptr<T> > items;

    Quadtree(std::unique_ptr< SDL_Rect > boundary, int level);

    void start();
    void start(std::weak_ptr< Quadtree<T> > root,
               std::weak_ptr< Quadtree<T> > parent);
    void clear();
    bool insert(std::shared_ptr< T > item);
    void update();
    bool remove(std::shared_ptr< T > item);
    void render(std::shared_ptr< SDL_Renderer > renderer);
    std::vector< std::shared_ptr<T> > queryArea(SDL_Rect* area);
    bool isLeaf();
    std::vector< std::shared_ptr<T> > getItems();

private:
    static const int MAX_ITEMS = 4;
    std::weak_ptr< Quadtree<T> > parent;
    std::weak_ptr< Quadtree<T> > root;

    TextureW* count;

    void divide();
    void undivide();
    bool belongs(std::shared_ptr< T > item);
    int getCount();
    bool isEmpty();
};

template <typename T>
Quadtree<T>::Quadtree(std::unique_ptr< SDL_Rect > boundary,
                      int level)
{
    this->boundary = std::move(boundary);
    this->level = level;

    this->count = new TextureW();
    this->count->setFont("assets/sample.ttf", 16);
//    this->northWest = nullptr;
//    this->northEast = nullptr;
//    this->southWest = nullptr;
//    this->southEast = nullptr;
}

template  <typename T>
void Quadtree<T>::start()
{
    this->root = this->shared_from_this();
}

template <typename T>
void Quadtree<T>::start(std::weak_ptr< Quadtree > root, std::weak_ptr< Quadtree > parent)
{
    this->root = root;
    this->parent = parent;
}

template <typename T>
void Quadtree<T>::clear()
{
    if (!this->isLeaf())
    {
        for (auto quad : quads)
        {
            quad->clear();
        }
    }
    this->quads.clear();
    this->quads.shrink_to_fit();
    this->items.clear();
    this->items.shrink_to_fit();
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
bool Quadtree<T>::insert(std::shared_ptr< T > item)
{
    std::cout << "Inserting: " << item << std::endl;
    if (this->belongs(item))
    {
        std::cout << "Belongs: " << item << std::endl;
        // IF IS IN QUAD ALREADY
        for (auto vectorItem : this->items)
        {
            if (vectorItem == item)
            {
                return false;
            }
        }

        // IF IS AS DEEP AS CAN GO
        if (this->level == this->MAX_LEVEL)
        {
            this->items.push_back(item);
            return true;
        }
        // IF IS LEAF AND CAN HOLD MORE
        else if (this->isLeaf() && this->items.size() < MAX_ITEMS)
        {
            this->items.push_back(item);
            return true;
        }
        // IF IS NOT LEAF
        else if (!this->isLeaf())
        {
            for (auto i : this->items)
            {
                if (item == i)
                {
                    return false;
                }
            }
            bool inserted = false;
            for (auto quad : this->quads)
            {
                if (quad->insert(item))
                {
                    inserted = true;
                }
            }
            return inserted;
        }
        // IF ISN'T AS DEEP AS CAN GO AND CANNOT HOLD MORE AND IS A LEAF
        else
        {
            std::cout << "Too many items" << std::endl;
            this->divide();

            // Reinsert
            for (auto quad : this->quads)
            {
                // New item
                quad->insert(item);
                for (auto itemVector : this->items)
                {
                    // Current items
                    quad->insert(itemVector);
                }
            }
            this->items.clear();
            this->items.shrink_to_fit();
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
    // If is leaf and is empty
    if (isEmpty())
    {
        this->clear();
        return;
    }

    // If any items have moved
    std::shared_ptr< T > temp;
    for (auto item : this->items)
    {
        if (item->hasMoved())
        {
            std::cout << item << " has moved." << std::endl;
            temp = item;
            if (this->remove(item))
            {
                this->root.lock()->insert(temp);
            }
        }
    }

    // Update subnodes
    if (!this->isLeaf())
    {
        for (auto quad : quads)
        {
            quad->update();
        }
        if (this->getCount() < MAX_ITEMS)
        {
            this->undivide();
        }
    }
}

template <typename T>
bool Quadtree<T>::remove(std::shared_ptr< T > item)
{
    for (unsigned int i = 0; i < this->items.size(); i++)
    {
        if (this->items[i] == item)
        {
            if (!this->isLeaf())
            {
                for (auto quad : this->quads)
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
void Quadtree<T>::render(std::shared_ptr< SDL_Renderer > renderer)
{
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    std::stringstream numItemsText;
    this->count->setRenderer(renderer);

    SDL_RenderDrawRect(renderer.get(), this->boundary.get());
    if (!this->isLeaf())
    {
        for (unsigned int i = 0; i < this->quads.size(); i++)
        {
            this->quads[i]->render(renderer);
        }
    }
    else
    {
        numItemsText << this->getCount();
        this->count->loadTextureFromText(numItemsText.str().c_str(), color);
        this->count->renderTexture(this->boundary->x + (this->level * 5), this->boundary->y + (this->level * 5));
    }
}

template <typename T>
void Quadtree<T>::divide()
{
    int childWidth = this->boundary->w / 2;
    int childHeight = this->boundary->h / 2;

    std::unique_ptr< SDL_Rect > northWestBoundary(new SDL_Rect{});
    northWestBoundary->x = this->boundary->x;
    northWestBoundary->y = this->boundary->y;
    northWestBoundary->w = childWidth;
    northWestBoundary->h = childHeight;

    std::unique_ptr< SDL_Rect > northEastBoundary(new SDL_Rect{});
    northEastBoundary->x = this->boundary->x + childWidth;
    northEastBoundary->y = this->boundary->y;
    northEastBoundary->w = childWidth;
    northEastBoundary->h = childHeight;

    std::unique_ptr< SDL_Rect > southWestBoundary(new SDL_Rect{});
    southWestBoundary->x = this->boundary->x;
    southWestBoundary->y = this->boundary->y + childHeight;
    southWestBoundary->w = childWidth;
    southWestBoundary->h = childHeight;

    std::unique_ptr< SDL_Rect > southEastBoundary(new SDL_Rect{});
    southEastBoundary->x = this->boundary->x + childWidth;
    southEastBoundary->y = this->boundary->y + childHeight;
    southEastBoundary->w = childWidth;
    southEastBoundary->h = childHeight;

    this->quads.push_back
            (std::make_shared<Quadtree<T>>
                     (std::move(northWestBoundary),
                      this->level + 1)
            );
    this->quads.push_back
            (std::make_shared<Quadtree<T>>
                     (std::move(northEastBoundary),
                      this->level + 1)
            );
    this->quads.push_back
            (std::make_shared<Quadtree<T>>
                     (std::move(southWestBoundary),
                      this->level + 1)
            );
    this->quads.push_back
            (std::make_shared<Quadtree<T>>
                     (std::move(southEastBoundary),
                      this->level + 1)
            );
    for (auto quad : this->quads)
    {
        quad->start(this->root.lock(), std::weak_ptr<Quadtree<T> >(this->shared_from_this()));
    }
}

template <typename T>
void Quadtree<T>::undivide()
{
    std::vector< std::shared_ptr<T> > temp;
    std::vector< std::shared_ptr<T> > allItems;

    for (auto quad : this->quads)
    {
        temp = getItems();
        allItems.insert(allItems.end(), temp.begin(), temp.end());
        this->quads.clear();
        this->quads.shrink_to_fit();
    }
    for (auto item : allItems)
    {
        this->insert(item);
    }
}

template <typename T>
bool Quadtree<T>::belongs(std::shared_ptr< T > item)
{
    return SDL_HasIntersection(item->getRect().get(), this->boundary.get());
}

template <typename T>
std::vector< std::shared_ptr<T> > Quadtree<T>::getItems()
{
    std::vector< std::shared_ptr<T> > temp;
    std::vector< std::shared_ptr<T> > allItems;
    if (!this->isLeaf())
    {
        for (auto quad : this->quads)
        {
            temp = quad->getItems();
            allItems.insert(allItems.end(), temp.begin(), temp.end());
        }
        return allItems;
    }
    else
    {
        return this->items;
    }
}

template <typename T>
int Quadtree<T>::getCount()
{
    int total = 0;
    if (!this->isLeaf())
    {
        for (auto quad : this->quads)
        {
            total += quad->getCount();
        }
        return total;
    }
    else
    {
        return (int) this->items.size();
    }
}

template <typename T>
bool Quadtree<T>::isLeaf()
{
    return this->quads.empty();
}

template <typename T>
bool Quadtree<T>::isEmpty()
{
    if (this->items.empty())
    {
        return this->isLeaf();
    }
    else
    {
        return false;
    }
}

#endif