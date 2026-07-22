#include "GuillotineAlgorithm.h"
#include "../entities/ExpandedPart.h"
#include "../entities/FreeRectangle.h"
#include "PartExpander.h"
#include "PartSorter.h"
#include <vector>
#include <limits>

CuttingResult GuillotineAlgorithm::calculate(const CuttingRequest& request)
{
    CuttingResult result;
    auto parts = PartExpander::expand(request.parts);
    PartSorter::sortByArea(parts);
    std::vector<FreeRectangle> freeRectangles;

    freeRectangles.push_back(
        {
            {
                0,
                0,
                request.sheet.width,
                request.sheet.height
            }
        });

    for (const auto& part : parts)
    {
        int bestIndex = -1;
        bool bestRotated = false;
        double bestWaste = std::numeric_limits<double>::max();

        for (size_t i = 0; i < freeRectangles.size(); ++i)
        {
            const auto& r = freeRectangles[i].rect;
            bool normalFits = part.width <= r.width && part.height <= r.height;
            bool rotatedFits = part.allowRotation && part.height <= r.width && part.width <= r.height;
            if (!normalFits && !rotatedFits)
            {
                continue;
            }
            double areaWaste = r.width * r.height - part.width * part.height;
            if (areaWaste < bestWaste)
            {
                bestWaste = areaWaste;
                bestIndex = static_cast<int>(i);
                bestRotated = (!normalFits && rotatedFits);
            }
        }
        if (bestIndex == -1)
        {
            result.unproducedCount++;
            continue;
        }

        auto freeRect = freeRectangles[bestIndex];
        auto r = freeRect.rect;
        double placedWidth = bestRotated ? part.height : part.width;
        double placedHeight = bestRotated ? part.width : part.height;
        Placement placement;
        placement.partId = part.partId;
        placement.rotated = bestRotated;
        placement.rect =
            {
                r.x,
                r.y,
                placedWidth,
                placedHeight
            };

        result.placements.push_back(placement);
        result.producedCount++;
        result.usedArea += part.width * part.height;
        freeRectangles.erase(freeRectangles.begin() + bestIndex);
        double rightWidth = r.width - placedWidth;
        double rightHeight = placedHeight;

        if (rightWidth > 0 && rightHeight > 0)
        {
            freeRectangles.push_back(
                {
                    {
                        r.x + placedWidth,
                        r.y,
                        rightWidth,
                        rightHeight
                    }
                });
        }

        double bottomWidth = r.width;
        double bottomHeight = r.height - placedHeight;

        if (bottomWidth > 0 && bottomHeight > 0)
        {
            freeRectangles.push_back(
                {
                    {
                        r.x,
                        r.y + placedHeight,
                        bottomWidth,
                        bottomHeight
                    }
                });
        }
    }
    const double sheetArea = request.sheet.width * request.sheet.height;
    result.wasteArea = sheetArea - result.usedArea;
    return result;
}