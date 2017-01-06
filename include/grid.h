#ifndef TOPDOWN_GRID_H
#define TOPDOWN_GRID_H

#include <vector>

#include "mimics/mimic.h"

class Grid
{
public:
    std::vector< std::shared_ptr< Mimic > > tiles;
    Grid();
};

#endif //TOPDOWN_GRID_H
