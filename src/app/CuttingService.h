#pragma once

#include "../domain/requests/CuttingRequest.h"
#include "../domain/results/CuttingResult.h"
#include "AlgorithmFactory.h"

class CuttingService
{
public:
    CuttingResult execute(const CuttingRequest& request, AlgorithmType algorithmType);
};