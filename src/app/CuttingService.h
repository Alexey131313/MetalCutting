#pragma once

#include "../domain/requests/CuttingRequest.h"
#include "../domain/results/CuttingResult.h"

class CuttingService
{
public:

    CuttingResult execute(
        const CuttingRequest& request);
};