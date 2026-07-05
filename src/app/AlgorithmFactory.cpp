#include "AlgorithmFactory.h"

#include "../domain/algorithms/GreedyAlgorithm.h"

std::unique_ptr<ICuttingAlgorithm>
AlgorithmFactory::create(
    AlgorithmType type)
{
    switch (type)
    {
    case AlgorithmType::Greedy:
        return std::make_unique<GreedyAlgorithm>();

    default:
        return nullptr;
    }
}