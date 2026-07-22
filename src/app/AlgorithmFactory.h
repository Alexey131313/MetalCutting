#pragma once

#include <memory>
#include "../domain/algorithms/ICuttingAlgorithm.h"

enum class AlgorithmType
{
    Greedy,
    Guillotine,
    MaxRects
};

class AlgorithmFactory
{
public:
    static std::unique_ptr<ICuttingAlgorithm> create(AlgorithmType type);
};