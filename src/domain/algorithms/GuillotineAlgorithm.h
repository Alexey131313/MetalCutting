#pragma once

#include "ICuttingAlgorithm.h"

class GuillotineAlgorithm : public ICuttingAlgorithm
{
public:
    CuttingResult calculate(const CuttingRequest& request) override;
};