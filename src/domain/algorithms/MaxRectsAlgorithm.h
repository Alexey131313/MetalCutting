#pragma once

#include "ICuttingAlgorithm.h"

#include "../entities/ExpandedPart.h"
#include "../entities/FreeRectangle.h"

#include <optional>
#include <vector>

class MaxRectsAlgorithm :
                          public ICuttingAlgorithm
{
public:

    CuttingResult calculate(
        const CuttingRequest& request) override;

private:

    std::optional<Placement>
    findBestPosition(
        const ExpandedPart& part,
        int& freeRectIndex);

    void splitFreeRectangles(
        const Placement& placement);

    void pruneFreeRectangles();

    bool intersects(
        const Rectangle& a,
        const Rectangle& b) const;

    bool contains(
        const Rectangle& a,
        const Rectangle& b) const;

private:

    std::vector<FreeRectangle>
        freeRectangles_;
};