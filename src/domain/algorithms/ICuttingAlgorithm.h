#pragma once

#include "../requests/CuttingRequest.h"
#include "../results/CuttingResult.h"

class ICuttingAlgorithm
{
public:
    virtual ~ICuttingAlgorithm() = default;
    virtual CuttingResult calculate(const CuttingRequest& request) = 0;
};