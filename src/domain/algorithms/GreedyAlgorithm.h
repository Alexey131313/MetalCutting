#pragma once

#include "ICuttingAlgorithm.h"

class GreedyAlgorithm : public ICuttingAlgorithm
{
public:
    CuttingResult calculate(const CuttingRequest& request) override;
};