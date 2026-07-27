#include "SummaryBuilder.h"
#include <algorithm>
#include <unordered_map>

double SummaryBuilder::calculateUsedLength(const std::vector<Placement>& placements)
{
    double usedLength = 0.0;
    for (const auto& placement : placements)
    {
        usedLength = std::max(usedLength, placement.rect.y + placement.rect.height);
    }
    return usedLength;
}
std::vector<PartSummary> SummaryBuilder::build(const std::vector<Part>& parts, const std::vector<Placement>& placements)
{
    std::vector<PartSummary> summaries;
    summaries.reserve(parts.size());
    std::unordered_map<int, std::size_t> indexById;
    for (const auto& part : parts)
    {
        auto [it, inserted] = indexById.emplace(part.id, summaries.size());
        if (inserted)
        {
            summaries.push_back(
                {
                    part.id,
                    part.width,
                    part.height,
                    0,
                    0,
                    part.allowRotation
                });
        }

        auto& summary = summaries[it->second];
        summary.requested += part.quantity;
        summary.width = part.width;
        summary.height = part.height;
        summary.allowRotation = summary.allowRotation && part.allowRotation;
    }
    for (const auto& placement : placements)
    {
        auto it = indexById.find(placement.partId);
        if (it != indexById.end())
            summaries[it->second].produced++;
    }
    std::sort(summaries.begin(), summaries.end(), [](const PartSummary& a, const PartSummary& b)
        {
            bool aProduced = a.produced > 0;
            bool bProduced = b.produced > 0;
            if (aProduced != bProduced)
                return aProduced > bProduced;
            double aArea = a.width * a.height;
            double bArea = b.width * b.height;
            if (aArea != bArea)
                return aArea > bArea;
            return a.id < b.id;
        });
    return summaries;
}