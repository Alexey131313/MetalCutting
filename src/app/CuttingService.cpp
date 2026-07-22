#include "CuttingService.h"
#include "AlgorithmFactory.h"
#include "CuttingValidator.h"
#include <stdexcept>

CuttingResult CuttingService::execute(const CuttingRequest& request, AlgorithmType algorithmType)
{
    auto algorithm = AlgorithmFactory::create(algorithmType);
    auto result = algorithm->calculate(request);
    bool valid = CuttingValidator::validate(result, request.sheet);

    if (!valid)
    {
        throw std::runtime_error("Invalid cutting result");
    }
    return result;
}

