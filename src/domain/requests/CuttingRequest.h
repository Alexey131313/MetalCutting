#pragma once

#include <vector>

#include "../entities/Sheet.h"
#include "../entities/Part.h"

struct CuttingRequest
{
    Sheet sheet;

    std::vector<Part> parts;
};