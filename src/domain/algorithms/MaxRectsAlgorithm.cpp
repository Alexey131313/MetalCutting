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

    std::sort(
        parts.begin(),
        parts.end(),
        [](const ExpandedPart& a,
           const ExpandedPart& b)
        {
            return
                std::max(a.width, a.height)
                >
                std::max(b.width, b.height);
        });

    for (const auto& part : parts)
    {
        int freeRectIndex = -1;

        auto placement =
            findBestPosition(
                part);

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

        removeZeroRectangles();

        mergeFreeRectangles();
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
    const ExpandedPart& part)
{
    double bestAreaFit =
        std::numeric_limits<double>::max();

    double bestShortSideFit =
        std::numeric_limits<double>::max();

    std::optional<Placement>
        bestPlacement;

    for (const auto& freeRect :
         freeRectangles_)
    {
        const auto& r =
            freeRect.rect;

        auto evaluate =
            [&](double width,
                double height,
                bool rotated)
        {
            if (width > r.width ||
                height > r.height)
            {
                return;
            }

            double areaFit =
                r.width * r.height -
                width * height;

            double shortSideFit =
                std::min(
                    r.width - width,
                    r.height - height);

            if (
                areaFit < bestAreaFit ||
                (
                    areaFit == bestAreaFit &&
                    shortSideFit <
                        bestShortSideFit
                    )
                )
            {
                bestAreaFit =
                    areaFit;

                bestShortSideFit =
                    shortSideFit;

                Placement placement;

                placement.partId =
                    part.partId;

                placement.rotated =
                    rotated;

                placement.rect =
                    {
                        r.x,
                        r.y,
                        width,
                        height
                    };

                bestPlacement =
                    placement;
            }
        };

        evaluate(
            part.width,
            part.height,
            false);

        if (part.allowRotation)
        {
            evaluate(
                part.height,
                part.width,
                true);
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

    std::sort(
        freeRectangles_.begin(),
        freeRectangles_.end(),
        [](const FreeRectangle& a,
           const FreeRectangle& b)
        {
            if (a.rect.x != b.rect.x)
                return a.rect.x < b.rect.x;

            if (a.rect.y != b.rect.y)
                return a.rect.y < b.rect.y;

            if (a.rect.width != b.rect.width)
                return a.rect.width < b.rect.width;

            return a.rect.height < b.rect.height;
        });

    freeRectangles_.erase(
        std::unique(
            freeRectangles_.begin(),
            freeRectangles_.end(),
            [](const FreeRectangle& a,
               const FreeRectangle& b)
            {
                return
                    a.rect.x == b.rect.x &&
                    a.rect.y == b.rect.y &&
                    a.rect.width == b.rect.width &&
                    a.rect.height == b.rect.height;
            }),
        freeRectangles_.end());
}

void MaxRectsAlgorithm::removeZeroRectangles()
{
    freeRectangles_.erase(
        std::remove_if(
            freeRectangles_.begin(),
            freeRectangles_.end(),
            [](const FreeRectangle& r)
            {
                return
                    r.rect.width <= 0 ||
                    r.rect.height <= 0;
            }),
        freeRectangles_.end());
}

void MaxRectsAlgorithm::mergeFreeRectangles()
{
    bool merged;

    do
    {
        merged = false;

        for (size_t i = 0;
             i < freeRectangles_.size();
             ++i)
        {
            for (size_t j = i + 1;
                 j < freeRectangles_.size();
                 ++j)
            {
                auto& a =
                    freeRectangles_[i].rect;

                auto& b =
                    freeRectangles_[j].rect;

                bool horizontalMerge =
                    a.y == b.y &&
                    a.height == b.height &&
                    (
                        a.x + a.width == b.x ||
                        b.x + b.width == a.x
                        );

                if (horizontalMerge)
                {
                    Rectangle mergedRect;

                    mergedRect.x =
                        std::min(a.x, b.x);

                    mergedRect.y =
                        a.y;

                    mergedRect.width =
                        a.width + b.width;

                    mergedRect.height =
                        a.height;

                    freeRectangles_[i].rect =
                        mergedRect;

                    freeRectangles_.erase(
                        freeRectangles_.begin()
                        + j);

                    merged = true;

                    break;
                }

                bool verticalMerge =
                    a.x == b.x &&
                    a.width == b.width &&
                    (
                        a.y + a.height == b.y ||
                        b.y + b.height == a.y
                        );

                if (verticalMerge)
                {
                    Rectangle mergedRect;

                    mergedRect.x =
                        a.x;

                    mergedRect.y =
                        std::min(a.y, b.y);

                    mergedRect.width =
                        a.width;

                    mergedRect.height =
                        a.height + b.height;

                    freeRectangles_[i].rect =
                        mergedRect;

                    freeRectangles_.erase(
                        freeRectangles_.begin()
                        + j);

                    merged = true;

                    break;
                }
            }

            if (merged)
            {
                break;
            }
        }

    } while (merged);
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