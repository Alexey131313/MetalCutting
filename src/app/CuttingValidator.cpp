#include "CuttingValidator.h"

bool CuttingValidator::validate(const CuttingResult& result, const Sheet& sheet)
{
    return validateInsideSheet(result, sheet) && validateIntersections(result);
}

bool CuttingValidator::validateInsideSheet(const CuttingResult& result, const Sheet& sheet)
{
    for (const auto& placement : result.placements)
    {
        if (placement.rect.x < 0)
            return false;

        if (placement.rect.y < 0)
            return false;

        if (placement.rect.x + placement.rect.width > sheet.width)
            return false;

        if (placement.rect.y + placement.rect.height > sheet.height)
            return false;
    }
    return true;
}

bool CuttingValidator::validateIntersections(const CuttingResult& result)
{
    const auto& placements = result.placements;

    for (size_t i = 0; i < placements.size(); ++i)
    {
        for (size_t j = i + 1; j < placements.size(); ++j)
        {
            if (intersects(placements[i], placements[j]))
            {
                return false;
            }
        }
    }
    return true;
}

bool CuttingValidator::intersects(const Placement& a, const Placement& b)
{
    return !(a.rect.x + a.rect.width <= b.rect.x || b.rect.x + b.rect.width <= a.rect.x || a.rect.y + a.rect.height <= b.rect.y || b.rect.y + b.rect.height <= a.rect.y);
}