#pragma once
#include <vector>
#include "../../domain/entities/Part.h"
#include "../../domain/entities/Placement.h"
#include "PartSummary.h"

class SummaryBuilder
{
public:
    static std::vector<PartSummary> build(const std::vector<Part>& parts, const std::vector<Placement>& placements);
    static double calculateUsedLength(const std::vector<Placement>& placements);
};