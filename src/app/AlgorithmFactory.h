#pragma once

enum class AlgorithmType
{
    Greedy,
    MaxRects
};

#pragma once

#include <memory>

#include "../domain/algorithms/ICuttingAlgorithm.h"

class AlgorithmFactory
{
public:

    static std::unique_ptr<ICuttingAlgorithm>
    create(AlgorithmType type);
};