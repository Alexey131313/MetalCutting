#pragma once

#include "../domain/results/CuttingResult.h"
#include "../domain/entities/Sheet.h"

class CuttingValidator
{
public:
    static bool validate(const CuttingResult& result, const Sheet& sheet);

private:

    static bool validateInsideSheet(const CuttingResult& result, const Sheet& sheet);
    static bool validateIntersections(const CuttingResult& result);
    static bool intersects(const Placement& a, const Placement& b);
};