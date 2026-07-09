#include "MaxRectsAlgorithm.h"

#include "PartExpander.h"
#include "PartSorter.h"

#include <algorithm>
#include <limits>

CuttingResult
MaxRectsAlgorithm::calculate(
    const CuttingRequest& request)
{
    CuttingResult result;

    freeRectangles_.clear();

    freeRectangles_.push_back(
        {
            {
                0,
                0,
                request.sheet.width,
                request.sheet.height
            }
        });

    auto parts =
        PartExpander::expand(
            request.parts);

    PartSorter::sortByArea(
        parts);

    for (const auto& part : parts)
    {
        int freeRectIndex = -1;

        auto placement =
            findBestPosition(
                part,
                freeRectIndex);

        if (!placement.has_value())
        {
            result.unproducedCount++;
            continue;
        }

        result.placements.push_back(
            placement.value());

        result.producedCount++;

        result.usedArea +=
            part.width *
            part.height;

        splitFreeRectangles(
            placement.value());

        pruneFreeRectangles();
    }

    const double sheetArea =
        request.sheet.width *
        request.sheet.height;

    result.wasteArea =
        sheetArea -
        result.usedArea;

    return result;
}

std::optional<Placement>
MaxRectsAlgorithm::findBestPosition(
    const ExpandedPart& part,
    int& freeRectIndex)
{
    double bestScore =
        std::numeric_limits<double>::max();

    std::optional<Placement>
        bestPlacement;

    freeRectIndex = -1;

    for (size_t i = 0;
         i < freeRectangles_.size();
         ++i)
    {
        const auto& r =
            freeRectangles_[i].rect;

        bool normalFits =
            part.width <= r.width &&
            part.height <= r.height;

        bool rotatedFits =
            part.allowRotation &&
            part.height <= r.width &&
            part.width <= r.height;

        if (!normalFits &&
            !rotatedFits)
        {
            continue;
        }

        if (normalFits)
        {
            double waste =
                r.width * r.height -
                part.width * part.height;

            if (waste < bestScore)
            {
                bestScore =
                    waste;

                freeRectIndex =
                    static_cast<int>(i);

                Placement p;

                p.partId =
                    part.partId;

                p.rotated =
                    false;

                p.rect =
                    {
                        r.x,
                        r.y,
                        part.width,
                        part.height
                    };

                bestPlacement = p;
            }
        }

        if (rotatedFits)
        {
            double waste =
                r.width * r.height -
                part.width * part.height;

            if (waste < bestScore)
            {
                bestScore =
                    waste;

                freeRectIndex =
                    static_cast<int>(i);

                Placement p;

                p.partId =
                    part.partId;

                p.rotated =
                    true;

                p.rect =
                    {
                        r.x,
                        r.y,
                        part.height,
                        part.width
                    };

                bestPlacement = p;
            }
        }
    }

    return bestPlacement;
}

void MaxRectsAlgorithm::splitFreeRectangles(
    const Placement& placement)
{
    std::vector<FreeRectangle>
        newFreeRects;

    for (const auto& freeRect :
         freeRectangles_)
    {
        if (!intersects(
                freeRect.rect,
                placement.rect))
        {
            newFreeRects.push_back(
                freeRect);

            continue;
        }

        const auto& r =
            freeRect.rect;

        const auto& p =
            placement.rect;

        double rRight =
            r.x + r.width;

        double rBottom =
            r.y + r.height;

        double pRight =
            p.x + p.width;

        double pBottom =
            p.y + p.height;

        if (p.y > r.y)
        {
            newFreeRects.push_back(
                {
                    {
                        r.x,
                        r.y,
                        r.width,
                        p.y - r.y
                    }
                });
        }

        if (pBottom < rBottom)
        {
            newFreeRects.push_back(
                {
                    {
                        r.x,
                        pBottom,
                        r.width,
                        rBottom - pBottom
                    }
                });
        }

        if (p.x > r.x)
        {
            newFreeRects.push_back(
                {
                    {
                        r.x,
                        r.y,
                        p.x - r.x,
                        r.height
                    }
                });
        }

        if (pRight < rRight)
        {
            newFreeRects.push_back(
                {
                    {
                        pRight,
                        r.y,
                        rRight - pRight,
                        r.height
                    }
                });
        }
    }

    freeRectangles_ =
        std::move(
            newFreeRects);
}

void MaxRectsAlgorithm::pruneFreeRectangles()
{
    for (size_t i = 0;
         i < freeRectangles_.size();
         ++i)
    {
        for (size_t j = i + 1;
             j < freeRectangles_.size();)
        {
            const auto& a =
                freeRectangles_[i].rect;

            const auto& b =
                freeRectangles_[j].rect;

            if (contains(a, b))
            {
                freeRectangles_.erase(
                    freeRectangles_.begin() + j);

                continue;
            }

            if (contains(b, a))
            {
                freeRectangles_.erase(
                    freeRectangles_.begin() + i);

                --i;
                break;
            }

            ++j;
        }
    }
}

bool MaxRectsAlgorithm::intersects(
    const Rectangle& a,
    const Rectangle& b) const
{
    return !(
        a.x + a.width <= b.x ||
        b.x + b.width <= a.x ||

                a.y + a.height <= b.y ||
        b.y + b.height <= a.y
        );
}

bool MaxRectsAlgorithm::contains(
    const Rectangle& a,
    const Rectangle& b) const
{
    return
        b.x >= a.x &&
        b.y >= a.y &&

                b.x + b.width <=
            a.x + a.width &&

                b.y + b.height <=
            a.y + a.height;
}