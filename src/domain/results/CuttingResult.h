#pragma once

#include <vector>

#include "../entities/Placement.h"

struct CuttingResult
{
    std::vector<Placement> placements;

    int producedCount{};

    int unproducedCount{};

    double usedArea{};

    double wasteArea{};
};