#include "GreedyAlgorithm.h"

#include "../entities/ExpandedPart.h"
#include "../entities/FreeRectangle.h"

#include "PartExpander.h"
#include "PartSorter.h"

#include <vector>

CuttingResult GreedyAlgorithm::calculate(
    const CuttingRequest& request)
{
    CuttingResult result;

    auto parts =
        PartExpander::expand(
            request.parts);

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
        bool placed = false;

        for (size_t i = 0;
             i < freeRectangles.size();
             ++i)
        {
            auto& freeRect =
                freeRectangles[i];

            auto r =
                freeRect.rect;

            bool normalFits =
                part.width <= r.width &&
                part.height <= r.height;

            bool rotatedFits =
                part.allowRotation &&
                part.height <= r.width &&
                part.width <= r.height;

            if (!normalFits && !rotatedFits)
                continue;

            double placedWidth;
            double placedHeight;
            bool rotated;

            if (normalFits)
            {
                placedWidth = part.width;
                placedHeight = part.height;

                rotated = false;
            } else
            {
                placedWidth = part.height;
                placedHeight = part.width;

                rotated = true;
            }

            Placement placement;

            placement.partId =
                part.partId;

            placement.rotated = rotated;

            placement.rect =
                {
                    r.x,
                    r.y,
                    placedWidth,
                    placedHeight
                };

            result.placements
                .push_back(placement);

            result.producedCount++;

            result.usedArea +=
                part.width *
                part.height;

            double rightWidth =
                r.width -
                placedWidth;

            double rightHeight =
                placedHeight;

            double bottomWidth =
                r.width;

            double bottomHeight =
                r.height -
                placedHeight;

            freeRectangles.erase(
                freeRectangles.begin() + i);

            if (rightWidth > 0 &&
                rightHeight > 0)
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

            if (bottomWidth > 0 &&
                bottomHeight > 0)
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

            placed = true;

            break;
        }

        if (!placed)
        {
            result.unproducedCount++;
        }
    }

    const double sheetArea = request.sheet.width * request.sheet.height;
    result.wasteArea = sheetArea - result.usedArea;

    return result;
}