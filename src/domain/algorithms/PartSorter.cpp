#include "PartSorter.h"

#include <algorithm>

void PartSorter::sortByArea(
    std::vector<ExpandedPart>& parts)
{
    std::sort(
        parts.begin(),
        parts.end(),
        [](const ExpandedPart& a,
           const ExpandedPart& b)
        {
            return a.width * a.height >
                   b.width * b.height;
        });
}