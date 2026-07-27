#include "AlgorithmFactory.h"
#include "../domain/algorithms/GreedyAlgorithm.h"
#include "../domain/algorithms/GuillotineAlgorithm.h"
#include "../domain/algorithms/MaxRectsAlgorithm.h"
#include <stdexcept>

std::unique_ptr<ICuttingAlgorithm> AlgorithmFactory::create(AlgorithmType type)
{
    switch (type)
    {
    case AlgorithmType::Greedy:
        return std::make_unique<GreedyAlgorithm>();
    case AlgorithmType::Guillotine:
        return std::make_unique<GuillotineAlgorithm>();
    case AlgorithmType::MaxRects:
        return std::make_unique<MaxRectsAlgorithm>();
    default:
        throw std::invalid_argument("Unsupported algorithm type");
    }
}