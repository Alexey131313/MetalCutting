#include "CuttingRenderer.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsTextItem>
#include <QPen>

void CuttingRenderer::render(
    QGraphicsScene* scene,
    const Sheet& sheet,
    const CuttingResult& result)
{
    scene->clear();

    constexpr int margin = 20;

    scene->setSceneRect(-margin, -margin, sheet.width + margin * 2, sheet.height + margin * 2);

    for (int x = 0; x <= sheet.width; x += 10)
    {
        scene->addLine(x, 0, x, sheet.height, QPen(Qt::lightGray));
    }
    for (int y = 0; y <= sheet.height; y += 10)
    {
        scene->addLine(0, y, sheet.width, y, QPen(Qt::lightGray));
    }

    scene->addRect(
        0,
        0,
        sheet.width,
        sheet.height,
        QPen(Qt::black, 2));

    auto sheetText =
        scene->addText(
            QString("%1 x %2")
                .arg(sheet.width)
                .arg(sheet.height));
    sheetText->setPos(5, 5);

    for (const auto& placement :
         result.placements)
    {
        scene->addRect(
            placement.rect.x,
            placement.rect.y,
            placement.rect.width,
            placement.rect.height,
            QPen(Qt::blue),
            QBrush(QColor(200, 220, 255)));

        QString textItem;
        if (placement.rotated) {
            textItem = QString("%1(R)\n(%2,%3)")
            .arg(placement.partId)
                .arg(placement.rect.x)
                .arg(placement.rect.y);
        } else {
            textItem = QString("%1\n(%2,%3)")
            .arg(placement.partId)
                .arg(placement.rect.x)
                .arg(placement.rect.y);
        }
        auto text =
                scene->addText(textItem);

        text->setPos(
            placement.rect.x + 5,
            placement.rect.y + 5);
    }
}