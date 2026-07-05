#include "PartExpander.h"

std::vector<ExpandedPart>
PartExpander::expand(const std::vector<Part>& parts)
{
    std::vector<ExpandedPart> result;

    for (const auto& part : parts)
    {
        for (int i = 0; i < part.quantity; ++i)
        {
            result.push_back(
                {
                    part.id,
                    part.width,
                    part.height,
                    part.allowRotation
                });
        }
    }

    return result;
}